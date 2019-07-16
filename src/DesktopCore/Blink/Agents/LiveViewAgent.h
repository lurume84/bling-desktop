#pragma once

#include "../../Network/Services/HTTPClientService.h"
#include "../../System/Services/ApplicationDataService.h"
#include "../../System/Services/IniFileService.h"
#include "../../System/Services/TimestampFolderService.h"
#include "../../System/Services/Process/CreateProcessService.h"
#include "../../System/Services/Process/TerminateProcessService.h"
#include "../../System/Services/TimeZoneService.h"
#include "../../Utils\Patterns\PublisherSubscriber\Subscriber.h"
#include "../../Model/IAgent.h"

#include <string>
#include <map>
#include <cpprestsdk/cpprest/http_msg.h>

namespace web { namespace http { namespace experimental { namespace listener { class http_listener; } } } }

namespace desktop { namespace core { 
	
	namespace model
	{
		struct RTP;
	}
	
	namespace agent {
	
	namespace cup = core::utils::patterns;
	
	class LiveViewAgent : public model::IAgent
	{
	public:
		LiveViewAgent(std::unique_ptr<service::system::ICreateProcessService> createProcessService = std::make_unique<service::system::CreateProcessService>(),
						std::unique_ptr<service::system::TerminateProcessService> terminateProcessService = std::make_unique<service::system::TerminateProcessService>(),
						std::unique_ptr<service::HTTPClientService> clientService = std::make_unique<service::HTTPClientService>(),
						std::unique_ptr<service::ApplicationDataService> applicationService = std::make_unique<service::ApplicationDataService>(),
						std::unique_ptr<service::IniFileService> iniFileService = std::make_unique<service::IniFileService>(),
						std::unique_ptr<service::TimestampFolderService> timestampFolderService = std::make_unique<service::TimestampFolderService>(),
						std::unique_ptr<service::TimeZoneService> timeZoneService = std::make_unique<service::TimeZoneService>());
		~LiveViewAgent();

		void handlePOST(web::http::http_request);
		void handleGET(web::http::http_request) const;
		void handleDELETE(web::http::http_request);
	private:
		std::unique_ptr<service::IniFileService> m_iniFileService;
		bool						m_enabled;
		unsigned int				m_seconds;
		bool						m_saveLocalTime;
		std::string					m_endpoint;

		std::map<int, std::unique_ptr<model::system::ProcessInformation>> m_liveViews;

		std::unique_ptr<service::HTTPClientService> m_clientService;
		std::unique_ptr<model::RTP>			m_RTP;
		std::unique_ptr<service::ApplicationDataService> m_applicationService;
		std::unique_ptr<service::TimestampFolderService> m_timestampFolderService;
		std::unique_ptr<service::system::ICreateProcessService> m_createProcessService;
		std::unique_ptr<service::system::TerminateProcessService> m_terminateProcessService;
		std::unique_ptr<service::TimeZoneService>		m_timeZoneService;
		std::unique_ptr<web::http::experimental::listener::http_listener> m_listener;
		cup::Subscriber m_subscriber;

		std::string m_outFolder;
	};
}}}