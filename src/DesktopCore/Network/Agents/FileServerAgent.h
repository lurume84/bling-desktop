#pragma once

#include "../../System/Services/ApplicationDataService.h"
#include "../../System/Services/IniFileService.h"
#include "../../Model/IAgent.h"

#include <string>
#include <map>
#include <cpprestsdk/cpprest/http_msg.h>

namespace web { namespace http { namespace experimental { namespace listener { class http_listener; } } } }

namespace desktop { namespace core { 
	
	namespace agent {
	
	class FileServerAgent : public model::IAgent
	{
	public:
		FileServerAgent(std::unique_ptr<service::ApplicationDataService> applicationService = std::make_unique<service::ApplicationDataService>(),
						std::unique_ptr<service::IniFileService> iniFileService = std::make_unique<service::IniFileService>());
		~FileServerAgent();

		void handlePOST(web::http::http_request);
		void handleGET(web::http::http_request) const;

	private:
		std::unique_ptr<service::ApplicationDataService>					m_applicationService;
		std::unique_ptr<service::IniFileService>							m_iniFileService;
		std::string															m_endpoint;
		std::unique_ptr<web::http::experimental::listener::http_listener>	m_listener;
		std::string															m_folder;

		std::mutex															m_mutex;
	};
}}}