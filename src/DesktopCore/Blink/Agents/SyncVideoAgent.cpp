#include "SyncVideoAgent.h"

#include "Utils\Patterns\PublisherSubscriber\Broker.h"
#include "../../Network/Events.h"
#include "..\..\Network\Model\Credentials.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <chrono>
#include <thread>

namespace desktop { namespace core { namespace agent {

	SyncVideoAgent::SyncVideoAgent(const std::string& outFolder,
									std::unique_ptr<service::IDownloadFileService> downloadService,
									std::unique_ptr<service::HTTPClientService> clientService,
									std::unique_ptr<service::ApplicationDataService> applicationService)
	: m_ioService()
	, m_timer(m_ioService, boost::posix_time::seconds(60))
	, m_downloadService(std::move(downloadService))
	, m_clientService(std::move(clientService))
	, m_applicationService(std::move(applicationService))
	, m_outFolder(outFolder)
	{
		m_subscriber.subscribe([this](const desktop::core::utils::patterns::Event& rawEvt)
		{
			const auto& evt = static_cast<const core::events::CredentialsEvent&>(rawEvt);

			m_credentials = std::make_unique<model::Credentials>(evt.m_credentials);

			if (!m_enabled)
			{
				m_enabled = true;
				armTimer(1);

				boost::thread t(boost::bind(&boost::asio::io_service::run, &m_ioService));
				m_backgroundThread.swap(t);
			}
		}, events::CREDENTIALS_EVENT);
	}

	SyncVideoAgent::~SyncVideoAgent()
	{
		m_enabled = false;
		m_timer.cancel();
		m_backgroundThread.join();
		m_ioService.reset();
	}

	std::string SyncVideoAgent::getLastUpdateTimestamp() const
	{
		std::string timestamp = "-999999999-01-01T00:00:00+00:00";

		auto documents = m_applicationService->getMyDocuments();

		if (boost::filesystem::exists(documents + "Download\\Videos\\last_update.txt"))
		{
			std::ifstream is(documents + "Download\\Videos\\last_update.txt");
			is >> timestamp;
			is.close();
		}

		return timestamp;
	}

	void SyncVideoAgent::setLastUpdateTimestamp() const
	{
		time_t rawtime;
		time(&rawtime);

		struct tm timeinfo;

		localtime_s(&timeinfo, &rawtime);

		auto documents = m_applicationService->getMyDocuments();

		std::ofstream os(documents + "Download\\Videos\\last_update.txt");
		os << timeinfo.tm_year + 1900 << "-" << std::setfill('0') << std::setw(2) << timeinfo.tm_mon + 1 << "-"
			<< std::setfill('0') << std::setw(2) << timeinfo.tm_mday
			<< "T" << std::setfill('0') << std::setw(2) << timeinfo.tm_hour << ":"
			<< std::setfill('0') << std::setw(2) << timeinfo.tm_min << ":"
			<< std::setfill('0') << std::setw(2) << timeinfo.tm_sec << "+00:00";
		os.close();
	}

	void SyncVideoAgent::setLastUpdateTimestamp(const std::string& timestamp) const
	{
		auto documents = m_applicationService->getMyDocuments();

		std::ofstream os(documents + "Download\\Videos\\last_update.txt");
		os << timestamp;
		os.close();
	}

	void SyncVideoAgent::execute()
	{
		if (m_enabled && m_credentials)
		{
			std::list<std::pair<std::string, std::string>> videos;

			getVideos(videos, "/api/v2/videos/changed?since=" + getLastUpdateTimestamp(), 1);

			auto documents = m_applicationService->getMyDocuments();

			std::map<std::string, std::string> requestHeaders;
			requestHeaders["token_auth"] = m_credentials->m_token;

			std::string months[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

			for (auto &video : videos)
			{
				std::stringstream ss(video.first);

				std::string year, month, day;
				getline(ss, year, '-'); getline(ss, month, '-'); getline(ss, day, 'T');

				int monthNumber = std::stoi(month);

				if (monthNumber <= 12)
				{
					month = months[monthNumber - 1];
				}

				auto fileName = boost::filesystem::path(video.second);

				auto folder = documents + "Download\\Videos\\" + year + "\\" + month + "\\" + day + "\\";
				auto target = folder + fileName.filename().string();

				if (!boost::filesystem::exists(target))
				{
					boost::filesystem::create_directories(folder);

					try
					{
						m_downloadService->download(m_credentials->m_host, video.second, requestHeaders, target);
						setLastUpdateTimestamp(video.first);

						std::this_thread::sleep_for(std::chrono::seconds{ 20 });
					}
					catch (...)
					{
						break;
					}
				}
				else
				{
					setLastUpdateTimestamp(video.first);
				}
			}
		}
	}

	void SyncVideoAgent::getVideos(std::list<std::pair<std::string, std::string>>& videos, const std::string& path, unsigned int page) const
	{
		std::map<std::string, std::string> requestHeaders, responseHeaders;
		std::string content;
		unsigned int status;

		requestHeaders["token_auth"] = m_credentials->m_token;

		std::stringstream ss;
		ss << path << "&page=" << page;

		if (m_clientService->send(m_credentials->m_host, m_credentials->m_port, ss.str(), requestHeaders, responseHeaders, content, status))
		{
			std::stringstream contentSS(content);

			try
			{
				boost::property_tree::ptree tree;
				boost::property_tree::json_parser::read_json(contentSS, tree);

				auto videosTag = tree.get_child("videos");

				if (videosTag.size() > 0)
				{
					for (auto &video : videosTag)
					{
						videos.push_back(std::make_pair(video.second.get_child("created_at").get_value<std::string>(), 
														video.second.get_child("address").get_value<std::string>()));
					}

					getVideos(videos, path, page + 1);
				}
			}
			catch (...)
			{
			
			}
		}
	}

	void SyncVideoAgent::armTimer(unsigned int seconds)
	{
		m_timer.expires_from_now(boost::posix_time::seconds(seconds));

		m_timer.async_wait([&](const boost::system::error_code& ec)
		{
			execute();
			armTimer();
		});
	}
}}}