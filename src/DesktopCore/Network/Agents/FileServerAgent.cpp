#include "FileServerAgent.h"

#include "System/Services/LogService.h"

#include <locale>
#include <codecvt>
#include <string>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string/replace.hpp>

#include <httplib.h>

namespace desktop { namespace core { namespace agent {

	static const std::map<std::string, std::string> g_mimeTypes = { 
																	{".html", "text/html" },
																	{".htm", "text/html" },
																	{".js", "application/javascript" },
																	{".css", "text/css" },
																	{".gif", "image/gif" },
																	{".jpg", "image/jpeg" },
																	{".png", "image/png" },
																	{".svg", "image/svg+xml" },
																	{".pdf", "application/pdf" },
																	{".eot", "application/vnd.ms-fontobject" },
																	{".ttf", "application/x-font-ttf" },
																	{".woff", "application/font-woff" },
																	{".woff2", "application/font-woff2" }
																};

	namespace 
	{
		std::string getContentType(const std::string& extension)
		{
			auto value = g_mimeTypes.find(extension);

			if (value != g_mimeTypes.end())
			{
				return value->second;
			}
			else
			{
				return "text/html";
			}
		}
	}

	FileServerAgent::FileServerAgent(std::unique_ptr<service::ApplicationDataService> applicationService,
									std::unique_ptr<service::IniFileService> iniFileService)
	: m_iniFileService(std::move(iniFileService))
	, m_applicationService(std::move(applicationService))
	, m_server(std::make_unique<httplib::Server>())
	{
		auto documents = m_applicationService->getMyDocuments();
		
		m_folder = m_applicationService->getViewerFolder();

		auto host = m_iniFileService->get<std::string>(documents + "Bling.ini", "FileServer", "Host", "127.0.0.1");
		auto port = m_iniFileService->get<int>(documents + "Bling.ini", "FileServer", "Port", 9191);

		m_endpoint = "http://" + host + ":" + std::to_string(port);

		m_server->Get("/version", [this](const httplib::Request& req, httplib::Response& res) {handleGETVersion(req, res); });
		m_server->Get(".*", [this](const httplib::Request& req, httplib::Response& res) {handleGET(req, res); });
		m_server->Post(".*", [this](const httplib::Request& req, httplib::Response& res, const httplib::ContentReader &content_reader) {handlePOST(req, res, content_reader); });

		service::LogService::info("Listening FileServer at {}:{}", host, port);

		std::thread([=]() 
		{
			m_server->listen(host.c_str(), port);
		}).detach();
	}

	FileServerAgent::~FileServerAgent()
	{
		m_server->stop();
	}

	void FileServerAgent::handleGET(const httplib::Request& req, httplib::Response& res) const
	{
		using namespace web::http;

		auto body = req.path;

		if (body == "" || *body.rbegin() == '/')
		{
			body = "/index.html";
		}

		body = boost::replace_all_copy(body, "/", "\\");

		boost::filesystem::path path(m_folder + body);

		if (boost::filesystem::exists(path))
		{
			std::ifstream ifs(path.string(), std::ifstream::binary);
			std::string data = std::string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());

			res.status = 200;
			res.set_content(data, getContentType(path.extension().string()).c_str());
		}
		else
		{
			res.status = 404;
		}

		service::LogService::info("HTTP GET {} FileServer {}", res.status, body);
	}

	void FileServerAgent::handleGETVersion(const httplib::Request& req, httplib::Response& res) const
	{
		res.status = 200;
		res.set_content(m_applicationService->getApplicationVersion(), "text/html");

		service::LogService::info("HTTP GET {} FileServer {}", res.status, "version");
	}

	void FileServerAgent::handlePOST(const httplib::Request& req, httplib::Response& res, const httplib::ContentReader &content_reader)
	{
		using namespace web::http;

		auto body = req.path;

		body = boost::replace_all_copy(body, "/", "\\");

		boost::filesystem::path path(m_folder + body);

		if (path.extension() == ".json")
		{
			std::unique_lock<std::mutex> lock(m_mutex);

			std::ofstream f(path.string());
			content_reader([&](const char *data, size_t data_length)
			{
				f << std::string(data, data_length);
				return true;
			});

			f.close();

			res.status = 200;
			res.set_content("{}", "application/json");
		}
		else
		{
			res.status = 405;
		}

		service::LogService::info("HTTP POST {} FileServer {}", res.status, body);
	}
}}}