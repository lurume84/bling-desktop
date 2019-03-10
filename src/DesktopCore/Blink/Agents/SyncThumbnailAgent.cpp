#include "SyncThumbnailAgent.h"

#include "Utils\Patterns\PublisherSubscriber\Broker.h"
#include "../../Network/Events.h"
#include "..\..\Network\Model\Credentials.h"
#include "..\..\System\Services\IniFileService.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include <map>

namespace desktop { namespace core { namespace agent {

	SyncThumbnailAgent::SyncThumbnailAgent(std::unique_ptr<service::IDownloadFileService> downloadService,
									std::unique_ptr<service::HTTPClientService> clientService,
									std::unique_ptr<service::ApplicationDataService> applicationService,
									std::unique_ptr<service::IniFileService> iniFileService)
	: m_ioService()
	, m_iniFileService(std::move(iniFileService))
	, m_downloadService(std::move(downloadService))
	, m_clientService(std::move(clientService))
	, m_applicationService(std::move(applicationService))
	{
		auto documents = m_applicationService->getMyDocuments();

		{
			m_seconds = m_iniFileService->get<unsigned int>(documents + "Blink.ini", "SyncThumbnail", "Interval", 3600);

			m_timer = std::make_unique<boost::asio::deadline_timer>(m_ioService, boost::posix_time::seconds(m_seconds));

			m_outFolder = m_iniFileService->get<std::string>(documents + "Blink.ini", "SyncThumbnail", "Output", documents + "Download\\Thumbnails\\");

			boost::filesystem::create_directories(m_outFolder);
		}

		if(m_iniFileService->get<bool>(documents + "Blink.ini", "SyncThumbnail", "Enabled", false))
		{
			m_subscriber.subscribe([this](const desktop::core::utils::patterns::Event& rawEvt)
			{
				const auto& evt = static_cast<const core::events::CredentialsEvent&>(rawEvt);

				m_credentials = std::make_unique<model::Credentials>(evt.m_credentials);

				auto documents = m_applicationService->getMyDocuments();
			
				if (!m_enabled)
				{
					m_enabled = true;

					armTimer(1);

					boost::thread t(boost::bind(&boost::asio::io_service::run, &m_ioService));
					m_backgroundThread.swap(t);
				}
			}, events::CREDENTIALS_EVENT);
		}
	}

	SyncThumbnailAgent::~SyncThumbnailAgent()
	{
		m_enabled = false;
		m_timer->cancel();
		m_backgroundThread.join();
		m_ioService.reset();
	}

	std::string SyncThumbnailAgent::getLastUpdateTimestamp() const
	{
		auto documents = m_applicationService->getMyDocuments();
		auto timestamp = m_iniFileService->get<std::string>(documents + "Blink.ini", "SyncThumbnail", "LastUpdate", "-999999999-01-01T00:00:00+00:00");

		return timestamp;
	}

	void SyncThumbnailAgent::setLastUpdateTimestamp() const
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

	void SyncThumbnailAgent::setLastUpdateTimestamp(const std::string& timestamp) const
	{
		auto documents = m_applicationService->getMyDocuments();

		m_iniFileService->set<std::string>(documents + "Blink.ini", "SyncThumbnail", "LastUpdate", timestamp);
	}

	void SyncThumbnailAgent::execute()
	{
		if (m_enabled && m_credentials)
		{
			std::vector<std::pair<unsigned int, std::vector<unsigned int>>> networkInfo;

			getNetworkInfo(networkInfo);

			for (auto& network : networkInfo)
			{
				for (auto& camera : network.second)
				{
					getThumbnail(network.first, camera);
				}
			}
		}
	}

	void SyncThumbnailAgent::getThumbnail(unsigned int network, unsigned int camera) const
	{
		std::map<std::string, std::string> requestHeaders, responseHeaders;
		std::string content;
		unsigned int status;

		requestHeaders["token_auth"] = m_credentials->m_token;

		std::stringstream path;
		path << "/network/" << network << "/camera/" << camera << "/thumbnail";

		if (m_clientService->post(m_credentials->m_host, m_credentials->m_port, path.str(), requestHeaders, responseHeaders, content, status))
		{
			
		}
	}

	void SyncThumbnailAgent::getNetworkInfo(std::vector<std::pair<unsigned int, std::vector<unsigned int>>>& cameras) const
	{
		std::map<std::string, std::string> requestHeaders, responseHeaders;
		std::string content;
		unsigned int status;

		requestHeaders["token_auth"] = m_credentials->m_token;

		std::string path = "/api/v1/camera/usage";

		if (m_clientService->get(m_credentials->m_host, m_credentials->m_port, path, requestHeaders, responseHeaders, content, status))
		{
			std::stringstream contentSS(content);

			try
			{
				boost::property_tree::ptree tree;
				boost::property_tree::json_parser::read_json(contentSS, tree);
				
				auto networks = tree.get_child("networks");

				for (auto &net : networks)
				{
					auto id = net.second.get_child("network_id").get_value<unsigned int>();

					auto network = std::make_pair(id, std::vector<unsigned int>());

					auto cams = net.second.get_child("cameras");

					for (auto &camera : cams)
					{
						network.second.push_back(camera.second.get_child("id").get_value<unsigned int>());
					}

					cameras.push_back(network);
				}
			}
			catch (...)
			{
			
			}
		}
	}

	void SyncThumbnailAgent::armTimer(unsigned int seconds)
	{
		m_timer->expires_from_now(boost::posix_time::seconds(seconds));

		m_timer->async_wait([&](const boost::system::error_code& ec)
		{
			execute();
			armTimer(m_seconds);
		});
	}
}}}