#include "FileServerAgent.h"

#include <locale>
#include <codecvt>
#include <string>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <cpprestsdk/cpprest/http_listener.h>
#include <cpprest\filestream.h>

namespace desktop { namespace core { namespace agent {

	static const std::map<std::wstring, std::wstring> g_mimeTypes = { 
																	{L".html", L"text/html" },
																	{L".htm", L"text/html" },
																	{L".js", L"application/javascript" },
																	{L".css", L"text/css" },
																	{L".gif", L"image/gif" },
																	{L".jpg", L"image/jpeg" },
																	{L".png", L"image/png" },
																	{L".svg", L"image/svg+xml" },
																	{L".pdf", L"application/pdf" },
																	{L".eot", L"application/vnd.ms-fontobject" },
																	{L".ttf", L"application/x-font-ttf" },
																	{L".woff", L"application/font-woff" },
																	{L".woff2", L"application/font-woff2" }
																};

	namespace 
	{
		std::wstring getContentType(const std::wstring& extension)
		{
			auto value = g_mimeTypes.find(extension);

			if (value != g_mimeTypes.end())
			{
				return value->second;
			}
			else
			{
				return L"text/html";
			}
		}
	}

	FileServerAgent::FileServerAgent(std::unique_ptr<service::ApplicationDataService> applicationService,
									std::unique_ptr<service::IniFileService> iniFileService)
	: m_iniFileService(std::move(iniFileService))
	, m_applicationService(std::move(applicationService))
	{
		auto documents = m_applicationService->getMyDocuments();
		
		m_folder = m_applicationService->getViewerFolder();

		m_endpoint = m_iniFileService->get<std::string>(documents + "Bling.ini", "FileServer", "Endpoint", "http://127.0.0.1:9191/");

		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring endpoint = converter.from_bytes(m_endpoint);

		auto uri = web::uri_builder(endpoint).to_uri();

		m_listener = std::make_unique<web::http::experimental::listener::http_listener>(uri);

		m_listener->support(web::http::methods::GET, std::bind(&FileServerAgent::handleGET, this, std::placeholders::_1));
		m_listener->support(web::http::methods::POST, std::bind(&FileServerAgent::handlePOST, this, std::placeholders::_1));

		m_listener->open();
	}

	FileServerAgent::~FileServerAgent()
	{
		
	}

	void FileServerAgent::handleGET(web::http::http_request request) const
	{
		using namespace web::http;

		auto bodyws = request.request_uri().path();

		std::string body(bodyws.begin(), bodyws.end());

		if (body == "" || *body.rbegin() == '/')
		{
			body = "/index.html";
		}

		body = boost::replace_all_copy(body, "/", "\\");

		boost::filesystem::path path(m_folder + body);

		if (boost::filesystem::exists(path))
		{
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
			std::wstring pathws = converter.from_bytes(path.string());

			concurrency::streams::fstream::open_istream(pathws, std::ios::in)
				.then([=](concurrency::streams::istream is)
			{
				web::http::http_response response(web::http::status_codes::OK);

				std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
				std::wstring extension = converter.from_bytes(path.extension().string());

				response.set_body(std::move(is), getContentType(extension));

				request.reply(response).then([](pplx::task<void> t) {});
			});
		}
		else
		{
			request.reply(status_codes::NotFound);
		}
	}

	void FileServerAgent::handlePOST(web::http::http_request request)
	{
		using namespace web::http;

		auto bodyws = request.request_uri().path();

		std::string body(bodyws.begin(), bodyws.end());

		body = boost::replace_all_copy(body, "/", "\\");

		boost::filesystem::path path(m_folder + body);

		if (path.extension() == ".json")
		{
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
			std::wstring pathws = converter.from_bytes(path.string());

			std::wstring payload = request.extract_string().get();

			std::unique_lock<std::mutex> lock(m_mutex);

			std::string out(payload.begin(), payload.end());

			std::ofstream f(path.string());
			f << out;
			f.close();

			request.reply(status_codes::OK, L"{}");
		}
		else
		{
			request.reply(status_codes::NotFound);
		}
	}
}}}