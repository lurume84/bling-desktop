#include "LiveViewAgent.h"

#include "Utils\Patterns\PublisherSubscriber\Broker.h"
#include "../../Network/Events.h"
#include "..\..\Network\Model\Credentials.h"
#include "..\..\System\Services\IniFileService.h"
#include "System\Model\ExecutableFile.h"
#include "System\Model\ProcessInformation.h"

#include <boost/filesystem.hpp>
#include <iostream>
#include <chrono>
#include <locale>
#include <codecvt>
#include <string>
#include <thread>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <cpprest\http_listener.h>
#include <cpprest\filestream.h>
#include <boost/thread.hpp>

namespace desktop { namespace core { namespace agent {

	LiveViewAgent::LiveViewAgent(std::unique_ptr<service::system::ICreateProcessService> createProcessService, 
									std::unique_ptr<service::system::TerminateProcessService> terminateProcessService,
									std::unique_ptr<service::HTTPClientService> clientService,
									std::unique_ptr<service::ApplicationDataService> applicationService,
									std::unique_ptr<service::IniFileService> iniFileService,
									std::unique_ptr<service::TimestampFolderService> timestampFolderService,
									std::unique_ptr<service::TimeZoneService> timeZoneService)
	: m_iniFileService(std::move(iniFileService))
	, m_clientService(std::move(clientService))
	, m_applicationService(std::move(applicationService))
	, m_timestampFolderService(std::move(timestampFolderService))
	, m_createProcessService(std::move(createProcessService))
	, m_terminateProcessService(std::move(terminateProcessService))
	, m_timeZoneService(std::move(timeZoneService))
	{
		auto documents = m_applicationService->getMyDocuments();

		{
			m_seconds = m_iniFileService->get<unsigned int>(documents + "Blink.ini", "LiveView", "Interval", 10);

			m_outFolder = m_iniFileService->get<std::string>(documents + "Blink.ini", "LiveView", "Output", documents + "Download\\Videos\\");

			m_saveLocalTime = m_iniFileService->get<bool>(documents + "Blink.ini", "LiveView", "UseLocalTime", false);
		}

		m_endpoint = m_iniFileService->get<std::string>(documents + "Blink.ini", "LiveView", "Endpoint", "http://127.0.0.1:9191/live");

		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring endpoint = converter.from_bytes(m_endpoint);

		auto uri = web::uri_builder(endpoint).to_uri();

		m_listener = std::make_unique<web::http::experimental::listener::http_listener>(uri);

		m_listener->support(web::http::methods::GET, std::bind(&LiveViewAgent::handleGET, this, std::placeholders::_1));
		m_listener->support(web::http::methods::POST, std::bind(&LiveViewAgent::handlePOST, this, std::placeholders::_1));
		m_listener->support(web::http::methods::DEL, std::bind(&LiveViewAgent::handleDELETE, this, std::placeholders::_1));

		m_listener->open();
	}

	LiveViewAgent::~LiveViewAgent()
	{
		m_listener->close();

		m_enabled = false;

		for(auto& liveView : m_liveViews)
		{
			m_terminateProcessService->sigint(*liveView.second);
			m_liveViews.erase(liveView.first);
		}
	}

	void LiveViewAgent::handleGET(web::http::http_request request) const
	{
		using namespace web::http;

		auto bodyws = request.request_uri().path();

		std::string body(bodyws.begin(), bodyws.end());

		body = boost::replace_all_copy(body.substr(6), "/", "\\");

		boost::replace_all(body, "%20", " ");

		boost::filesystem::path path(m_outFolder + body);
		
		if(path.extension() == ".m3u8")
		{
			while(!boost::filesystem::exists(path))
			{
				boost::this_thread::sleep_for(boost::chrono::milliseconds(200));
			}
		}

		if (boost::filesystem::exists(path))
		{
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
			std::wstring pathws = converter.from_bytes(path.string());

			concurrency::streams::fstream::open_istream(pathws, std::ios::in)
				.then([=](concurrency::streams::istream is)
			{
				web::http::http_response response(web::http::status_codes::OK);

				response.headers().add(L"Content-Disposition", U("inline; filename = \"") + pathws + U("\""));
				response.set_body(std::move(is), U("application/x-mpegURL"));

				request.reply(response).then([](pplx::task<void> t) {});
			});
		}
		else
		{
			request.reply(status_codes::NotFound);
		}
	}

	void LiveViewAgent::handlePOST(web::http::http_request request)
	{
		using namespace web::http;

		auto payload = request.extract_json().get();

		std::wstring urlws = payload.at(L"url").as_string();

		std::string url(urlws.begin(), urlws.end());

		m_RTP = std::make_unique<model::RTP>(url);

		auto appFolder = m_applicationService->getApplicationFolder();

		time_t now;
		time(&now);

		std::string iso = boost::posix_time::to_iso_extended_string(boost::posix_time::from_time_t(now));

		std::string currentTime = iso.substr(0, iso.find_first_of(",")) + "+00:00";

		if (m_saveLocalTime)
		{
			currentTime = m_timeZoneService->universalToLocal(currentTime);
		}

		currentTime = boost::replace_all_copy(currentTime, ":", "_");

		std::stringstream folderSS; 
		folderSS << m_timestampFolderService->get(currentTime) << "LiveView_";
		folderSS << currentTime;

		auto folder = folderSS.str();

		auto absPath = m_outFolder + folder;

		boost::filesystem::create_directories(absPath);

		auto arguments = "-report -i \"" + m_RTP->m_url + "\" -c copy -vcodec copy -g 30 -hls_time 1 out.m3u8 -vcodec copy " + currentTime + ".mp4";

		model::system::ExecutableFile ffmpeg(model::system::ExecutableFile::Path(appFolder + "\\ffmpeg.exe"), model::system::ExecutableFile::Arguments(arguments));

		int camera_id = payload.at(L"camera_id").as_integer();

		std::wstringstream camera_idss;
		camera_idss << camera_id;

		auto process = m_createProcessService->create(ffmpeg, absPath);

		m_liveViews.insert(std::make_pair(camera_id, std::move(process)));

		boost::replace_all(folder, "\\", "/");

		std::string endpoint = m_endpoint + "/" + boost::replace_all_copy(folder, "\\", "/") + "/out.m3u8";

		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring endpointws = converter.from_bytes(endpoint);

		http_response response(status_codes::OK);
		response.headers().set_content_type(L"application/json");
		response.set_body(L"{\"camera_id\": " + camera_idss.str() + L", \"url\": \"" + endpointws + L"\"}");

		request.reply(response);
	}

	void LiveViewAgent::handleDELETE(web::http::http_request request)
	{
		using namespace web::http;

		auto payload = request.extract_json().get();

		int camera_id = payload.at(L"camera_id").as_integer();

		auto liveView = m_liveViews.find(camera_id);

		if (liveView != m_liveViews.end())
		{
			m_terminateProcessService->sigint(*liveView->second);

			m_liveViews.erase(camera_id);

			http_response response(status_codes::OK);
			response.headers().set_content_type(L"application/json");
			response.set_body(L"{}");
			
			request.reply(response);
		}
		else
		{
			request.reply(status_codes::NotFound);
		}
	}
}}}