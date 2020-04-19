#pragma once

#include "../../Network/Services/HTTPClientService.h"
#include "../../Network/Services/DownloadFileService.h"
#include "../../System/Services/ApplicationDataService.h"
#include "../../System/Services/IniFileService.h"
#include "../../System/Services/TimestampFolderService.h"
#include "../../System/Services/TimeZoneService.h"
#include "../../System/Services/TimerKillerService.h"
#include "../../Utils\Patterns\PublisherSubscriber\Subscriber.h"
#include "../../Model/IAgent.h"

#include <string>
#include <map>
#include <boost/thread.hpp>
#include <boost/asio.hpp>

namespace desktop { namespace core { 
	
	namespace model
	{
		struct Credentials;
	}
	
	namespace agent {
	
	namespace cup = core::utils::patterns;
	
	class SyncVideoAgent : public model::IAgent
	{
	public:
		SyncVideoAgent(std::unique_ptr<service::IDownloadFileService> downloadService = std::make_unique<service::DownloadFileService>(),
						std::unique_ptr<service::HTTPClientService> clientService = std::make_unique<service::HTTPClientService>(),
						std::unique_ptr<service::ApplicationDataService> applicationService = std::make_unique<service::ApplicationDataService>(),
						std::unique_ptr<service::IniFileService> iniFileService = std::make_unique<service::IniFileService>(),
						std::unique_ptr<service::TimestampFolderService> timestampFolderService = std::make_unique<service::TimestampFolderService>(),
						std::unique_ptr<service::TimeZoneService> timeZoneService = std::make_unique<service::TimeZoneService>(),
						std::unique_ptr<service::TimerKillerService> timerKillerService = std::make_unique<service::TimerKillerService>());
		~SyncVideoAgent();

		void getVideos(std::map<std::string, std::string>& videos, const std::string& timestamp, unsigned int page) const;
		void execute();
	private:
		void armTimer(unsigned int seconds = 60);
		std::string getLastUpdateTimestamp() const;
		void setLastUpdateTimestamp() const;
		void setLastUpdateTimestamp(const std::string&) const;
		std::string formatFileName(const std::string& timestamp, const std::string& fileName) const;
	private:
		std::unique_ptr<service::IniFileService> m_iniFileService;

		boost::asio::io_service		m_ioService;
		std::unique_ptr<boost::asio::deadline_timer>	m_timer;
		boost::thread				m_backgroundThread;
		std::string					m_outFolder;
		bool						m_enabled = false;
		unsigned int				m_seconds;
		bool						m_saveLocalTime;

		std::unique_ptr<service::IDownloadFileService>		m_downloadService;
		std::unique_ptr<service::HTTPClientService>			m_clientService;
		std::unique_ptr<model::Credentials>					m_credentials;
		std::unique_ptr<service::ApplicationDataService>	m_applicationService;
		std::unique_ptr<service::TimestampFolderService>	m_timestampFolderService;
		std::unique_ptr<service::TimeZoneService>			m_timeZoneService;
		std::unique_ptr<service::TimerKillerService>		m_timerKillerService;

		cup::Subscriber m_subscriber;
	};
}}}