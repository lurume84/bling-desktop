#pragma once

#include "../../System/Services/ApplicationDataService.h"
#include "../../System/Services/IniFileService.h"
#include "../../Model/IAgent.h"

#include <string>
#include <map>
#include <mutex>

namespace httplib
{
	class Server;
	struct Request;
	struct Response;
	class ContentReader;
}

namespace web { namespace http { namespace experimental { namespace listener { class http_listener; } } } }

namespace desktop { namespace core { 
	
	namespace agent {
	
	class FileServerAgent : public model::IAgent
	{
	public:
		FileServerAgent(std::unique_ptr<service::ApplicationDataService> applicationService = std::make_unique<service::ApplicationDataService>(),
						std::unique_ptr<service::IniFileService> iniFileService = std::make_unique<service::IniFileService>());
		~FileServerAgent();

		void handlePOST(const httplib::Request& req, httplib::Response& res, const httplib::ContentReader &content_reader);
		void handleGET(const httplib::Request& req, httplib::Response& res) const;
		void handleGETVersion(const httplib::Request& req, httplib::Response& res) const;
	private:
		std::unique_ptr<service::ApplicationDataService>					m_applicationService;
		std::unique_ptr<service::IniFileService>							m_iniFileService;
		std::string															m_endpoint;
		std::string															m_folder;

		std::mutex															m_mutex;
		std::unique_ptr<httplib::Server>									m_server;
	};
}}}