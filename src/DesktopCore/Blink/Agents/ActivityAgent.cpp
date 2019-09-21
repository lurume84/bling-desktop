#include "ActivityAgent.h"

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

namespace desktop { namespace core { namespace agent {

	ActivityAgent::ActivityAgent(std::unique_ptr<service::IActivityNotificationService> activityService,
									std::unique_ptr<service::HTTPClientService> clientService,
									std::unique_ptr<service::ApplicationDataService> applicationService,
									std::unique_ptr<service::IniFileService> iniFileService,
									std::unique_ptr<service::TimestampFolderService> timestampFolderService)
	: m_ioService()
	, m_iniFileService(std::move(iniFileService))
	, m_activityService(std::move(activityService))
	, m_clientService(std::move(clientService))
	, m_applicationService(std::move(applicationService))
	, m_timestampFolderService(std::move(timestampFolderService))
	{
		auto documents = m_applicationService->getMyDocuments();

		{
			m_seconds = m_iniFileService->get<unsigned int>(documents + "Blink.ini", "Activity", "Interval", 10);

			m_timer = std::make_unique<boost::asio::deadline_timer>(m_ioService, boost::posix_time::seconds(m_seconds));
		}

		if(m_iniFileService->get<bool>(documents + "Blink.ini", "Activity", "Enabled", true))
		{
			setLastUpdateTimestamp();

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

	ActivityAgent::~ActivityAgent()
	{
		m_enabled = false;
		m_timer->cancel();
		m_backgroundThread.join();
		m_ioService.reset();
	}

	std::string ActivityAgent::getLastUpdateTimestamp() const
	{
		auto documents = m_applicationService->getMyDocuments();
		auto timestamp = m_iniFileService->get<std::string>(documents + "Blink.ini", "Activity", "LastUpdate", "-999999999-01-01T00:00:00+00:00");

		return timestamp;
	}

	void ActivityAgent::setLastUpdateTimestamp() const
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

	void ActivityAgent::setLastUpdateTimestamp(const std::string& timestamp) const
	{
		auto documents = m_applicationService->getMyDocuments();

		m_iniFileService->set<std::string>(documents + "Blink.ini", "Activity", "LastUpdate", timestamp);
	}

	void ActivityAgent::execute()
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
				m_activityService->notify();
			}

			setLastUpdateTimestamp();
		}
	}

	void ActivityAgent::getVideos(std::map<std::string, std::string>& videos, const std::string& path, unsigned int page) const
	{
		std::map<std::string, std::string> requestHeaders, responseHeaders;
		std::string content;
		unsigned int status;

		requestHeaders["token_auth"] = m_credentials->m_token;

		std::stringstream ss;
		ss << "/api/v2/notification";// << "&page=" << page;

		if (m_clientService->post(m_credentials->m_host, m_credentials->m_port, ss.str(), requestHeaders, responseHeaders, content, status))
		{
			std::stringstream contentSS(content);

			try
			{
				boost::property_tree::ptree tree;
				boost::property_tree::json_parser::read_json(contentSS, tree);
				
				auto videosTag = tree.get_child("notification_recipient");

				if (videosTag.size() > 0)
				{
					for (auto &video : videosTag)
					{
						if (!video.second.get_child("deleted").get_value<bool>())
						{
							videos[video.second.get_child("created_at").get_value<std::string>()] = video.second.get_child("media").get_value<std::string>();
						}
					}
				}
			}
			catch (...)
			{
			
			}
		}
	}

	void ActivityAgent::armTimer(unsigned int seconds)
	{
		if (m_enabled)
		{
			m_timer->expires_from_now(boost::posix_time::seconds(seconds));

			m_timer->async_wait([&](const boost::system::error_code& ec)
			{
				execute();
				armTimer(m_seconds);
			});
		}
	}
}}}