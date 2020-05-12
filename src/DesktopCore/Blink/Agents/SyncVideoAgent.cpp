#include "SyncVideoAgent.h"

#include "Utils\Patterns\PublisherSubscriber\Broker.h"
#include "../../Network/Events.h"
#include "..\..\Network\Model\Credentials.h"
#include "..\..\System\Services\IniFileService.h"
#include "System/Services/LogService.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp> 
#include <iostream>
#include <chrono>
#include <thread>

namespace desktop { namespace core { namespace agent {

	SyncVideoAgent::SyncVideoAgent(std::unique_ptr<service::IDownloadFileService> downloadService,
									std::unique_ptr<service::HTTPClientService> clientService,
									std::unique_ptr<service::ApplicationDataService> applicationService,
									std::unique_ptr<service::IniFileService> iniFileService,
									std::unique_ptr<service::TimestampFolderService> timestampFolderService,
									std::unique_ptr<service::TimeZoneService> timeZoneService,
									std::unique_ptr<service::TimerKillerService> timerKillerService)
	: m_ioService()
	, m_iniFileService(std::move(iniFileService))
	, m_downloadService(std::move(downloadService))
	, m_clientService(std::move(clientService))
	, m_applicationService(std::move(applicationService))
	, m_timestampFolderService(std::move(timestampFolderService))
	, m_timeZoneService(std::move(timeZoneService))
	, m_timerKillerService(std::move(timerKillerService))
	{
		auto documents = m_applicationService->getMyDocuments();

		{
			m_saveLocalTime = m_iniFileService->get<bool>(documents + "Blink.ini", "SyncVideo", "UseLocalTime", false);

			m_seconds = m_iniFileService->get<unsigned int>(documents + "Blink.ini", "SyncVideo", "Interval", 60);

			m_timer = std::make_unique<boost::asio::deadline_timer>(m_ioService, boost::posix_time::seconds(m_seconds));

			m_outFolder = m_iniFileService->get<std::string>(documents + "Blink.ini", "SyncVideo", "Output", documents + "Download\\Videos\\");

			boost::filesystem::create_directories(m_outFolder);
		}

		if(m_iniFileService->get<bool>(documents + "Blink.ini", "SyncVideo", "Enabled", true))
		{
			m_subscriber.subscribe([this](const desktop::core::utils::patterns::Event& rawEvt)
			{
				service::LogService::info("Video synchronization: User is logged on");

				const auto& evt = static_cast<const core::events::CredentialsEvent&>(rawEvt);

				m_credentials = std::make_unique<model::Credentials>(evt.m_credentials);

				auto documents = m_applicationService->getMyDocuments();
			
				if (!m_enabled)
				{
					service::LogService::info("Video synchronization is enabled");

					m_enabled = true;

					armTimer(1);

					boost::thread t(boost::bind(&boost::asio::io_service::run, &m_ioService));
					m_backgroundThread.swap(t);
				}
			}, events::CREDENTIALS_EVENT);

			service::LogService::info("Video synchronization every {} seconds in {}", m_seconds, m_outFolder);
		}
	}

	SyncVideoAgent::~SyncVideoAgent()
	{
		m_enabled = false;
		m_timerKillerService->kill();
		m_timer->cancel();
		m_backgroundThread.join();
		m_ioService.reset();
	}

	std::string SyncVideoAgent::getLastUpdateTimestamp() const
	{
		auto documents = m_applicationService->getMyDocuments();
		auto timestamp = m_iniFileService->get<std::string>(documents + "Blink.ini", "SyncVideo", "LastUpdate", "-999999999-01-01T00:00:00+00:00");

		return timestamp;
	}

	void SyncVideoAgent::setLastUpdateTimestamp() const
	{
		time_t rawtime;
		time(&rawtime);

		struct tm timeinfo;

		localtime_s(&timeinfo, &rawtime);

		std::stringstream ss;
		ss << timeinfo.tm_year + 1900 << "-" << std::setfill('0') << std::setw(2) << timeinfo.tm_mon + 1 << "-"
			<< std::setfill('0') << std::setw(2) << timeinfo.tm_mday
			<< "T" << std::setfill('0') << std::setw(2) << timeinfo.tm_hour << ":"
			<< std::setfill('0') << std::setw(2) << timeinfo.tm_min << ":"
			<< std::setfill('0') << std::setw(2) << timeinfo.tm_sec << "+00:00";
		
		setLastUpdateTimestamp(ss.str());
	}

	void SyncVideoAgent::setLastUpdateTimestamp(const std::string& timestamp) const
	{
		auto documents = m_applicationService->getMyDocuments();

		m_iniFileService->set<std::string>(documents + "Blink.ini", "SyncVideo", "LastUpdate", timestamp);
	}

	void SyncVideoAgent::execute()
	{
		if (m_enabled && m_credentials)
		{
			std::map<std::string, std::string> videos;

			std::string lastUpdate = getLastUpdateTimestamp();

			std::stringstream ss;
			ss << "/api/v1/accounts/" << m_credentials->m_account << "/media/changed?since=" << lastUpdate;

			getVideos(videos, ss.str(), 1);

			if (videos.size() > 0)
			{
				auto documents = m_applicationService->getMyDocuments();
				unsigned int sleep = m_iniFileService->get<unsigned int>(documents + "Blink.ini", "SyncVideo", "Sleep", 20);

				std::map<std::string, std::string> requestHeaders;
				requestHeaders["token_auth"] = m_credentials->m_token;

				for (auto &video : videos)
				{
					if (!m_enabled)
					{
						break;
					}

					auto folder = boost::filesystem::path(m_outFolder) / m_timestampFolderService->get(video.first);
					auto target = folder / formatFileName(video.first, video.second);

					if (!boost::filesystem::exists(target))
					{
						boost::filesystem::create_directories(folder);

						try
						{
							auto downloadFolder = m_downloadService->download(m_credentials->m_host, video.second, requestHeaders, target.string());

							if (downloadFolder != "")
							{
								service::LogService::info("Video {} downloaded at {}", video.second, downloadFolder);
							}
							else
							{
								service::LogService::error("Video {} could not be downloaded at {}", video.second, target.string());
							}

							setLastUpdateTimestamp(video.first);

							if (!m_timerKillerService->wait_for(std::chrono::seconds{ sleep }))
							{
								break;
							}
						}
						catch (...)
						{
							service::LogService::error("Video: An error occured while downloading video {} to {}", video.second, target.string());
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
	}

	void SyncVideoAgent::getVideos(std::map<std::string, std::string>& videos, const std::string& path, unsigned int page) const
	{
		std::map<std::string, std::string> requestHeaders, responseHeaders;
		std::string content;
		unsigned int status;

		requestHeaders["token_auth"] = m_credentials->m_token;

		std::stringstream ss;
		ss << path << "&page=" << page;

		if (m_enabled && m_clientService->get(m_credentials->m_host, m_credentials->m_port, ss.str(), requestHeaders, responseHeaders, content, status))
		{
			std::stringstream contentSS(content);

			try
			{
				boost::property_tree::ptree tree;
				boost::property_tree::json_parser::read_json(contentSS, tree);
				
				auto videosTag = tree.get_child("media");

				if (videosTag.size() > 0)
				{
					for (auto &video : videosTag)
					{
						if (!video.second.get_child("deleted").get_value<bool>())
						{
							videos[video.second.get_child("created_at").get_value<std::string>()] = video.second.get_child("media").get_value<std::string>();
						}
					}

					getVideos(videos, path, page + 1);
				}
			}
			catch (...)
			{
				service::LogService::error("Video synchronization: An error occurred while getting videos");
			}
		}
	}

	void SyncVideoAgent::armTimer(unsigned int seconds)
	{
		if(m_enabled)
		{
			m_timer->expires_from_now(boost::posix_time::seconds(seconds));

			m_timer->async_wait([&](const boost::system::error_code& ec)
			{
				execute();
				armTimer(m_seconds);

				service::LogService::error("Video synchronization: Waiting another {} seconds", m_seconds);
			});
		}
	}

	std::string SyncVideoAgent::formatFileName(const std::string& timestamp, const std::string& fileName) const
	{
		if (m_saveLocalTime)
		{
			return boost::replace_all_copy(m_timeZoneService->universalToLocal(timestamp), ":", "_") + ".mp4";
		}
		else
		{
			return boost::filesystem::path(boost::replace_all_copy(timestamp, ":", "_")).filename().string() + ".mp4";
		}
	}
}}}