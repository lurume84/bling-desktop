#include "LiveViewAgent.h"

#include "Utils\Patterns\PublisherSubscriber\Broker.h"
#include "../../Network/Events.h"
#include "..\..\Network\Model\Credentials.h"
#include "..\..\System\Services\IniFileService.h"
#include "System\Model\ExecutableFile.h"
#include "System\Model\ProcessInformation.h"
#include <cpprest\http_listener.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <chrono>
#include <locale>
#include <codecvt>
#include <string>
#include <thread>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/c_local_time_adjustor.hpp"

namespace desktop { namespace core { namespace agent {

	LiveViewAgent::LiveViewAgent(std::unique_ptr<service::system::ICreateProcessService> createProcessService, 
									std::unique_ptr<service::HTTPClientService> clientService,
									std::unique_ptr<service::ApplicationDataService> applicationService,
									std::unique_ptr<service::IniFileService> iniFileService,
									std::unique_ptr<service::TimestampFolderService> timestampFolderService,
									std::unique_ptr<service::TimeZoneService> timeZoneService)
	: m_ioService()
	, m_iniFileService(std::move(iniFileService))
	, m_clientService(std::move(clientService))
	, m_applicationService(std::move(applicationService))
	, m_timestampFolderService(std::move(timestampFolderService))
	, m_createProcessService(std::move(createProcessService))
	, m_timeZoneService(std::move(timeZoneService))
	{
		auto documents = m_applicationService->getMyDocuments();

		{
			m_seconds = m_iniFileService->get<unsigned int>(documents + "Blink.ini", "LiveView", "Interval", 10);

			m_outFolder = m_iniFileService->get<std::string>(documents + "Blink.ini", "LiveView", "Output", documents + "Download\\Videos\\");

			m_saveLocalTime = m_iniFileService->get<bool>(documents + "Blink.ini", "LiveView", "UseLocalTime", false);

			m_timer = std::make_unique<boost::asio::deadline_timer>(m_ioService, boost::posix_time::seconds(m_seconds));
		}

		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring endpoint = converter.from_bytes(m_iniFileService->get<std::string>(documents + "Blink.ini", "LiveView", "Endpoint", "http://127.0.0.1:9191/live"));

		auto uri = web::uri_builder(endpoint).to_uri();

		m_listener = std::make_unique<web::http::experimental::listener::http_listener>(uri);

		m_listener->support(web::http::methods::GET, std::bind(&LiveViewAgent::handleGET, this, std::placeholders::_1));
		m_listener->support(web::http::methods::POST, std::bind(&LiveViewAgent::handlePOST, this, std::placeholders::_1));

		m_listener->open();
	}

	LiveViewAgent::~LiveViewAgent()
	{
		m_enabled = false;
		m_timer->cancel();
		m_backgroundThread.join();
		m_ioService.reset();
		m_listener->close();
	}

	void LiveViewAgent::execute()
	{
		
	}

	void LiveViewAgent::armTimer(unsigned int seconds)
	{
		m_timer->expires_from_now(boost::posix_time::seconds(seconds));

		m_timer->async_wait([&](const boost::system::error_code& ec)
		{
			execute();
			armTimer(m_seconds);
		});
	}

	void LiveViewAgent::handleGET(web::http::http_request request) const
	{
		using namespace web::http;

		
	}

	void LiveViewAgent::handlePOST(web::http::http_request request)
	{
		using namespace web::http;

		auto bodyws = request.extract_string().get();

		std::string body(bodyws.begin(), bodyws.end());

		m_RTP = std::make_unique<model::RTP>(body);

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
		folderSS << m_timestampFolderService->get(currentTime) << "LiveView ";
		folderSS << currentTime;

		auto folder = folderSS.str();

		auto absPath = m_outFolder + folder;

		boost::filesystem::create_directories(absPath);

		model::system::ExecutableFile ffmpeg(model::system::ExecutableFile::Path(appFolder + "\\ffmpeg.exe"), 
											model::system::ExecutableFile::Arguments("-i " + m_RTP->m_url + " -f " + 
												"ssegment -hls_flags delete_segments -segment_list \"" + absPath + "\\out.m3u8\" -segment_list_type hls -segment_list_size 0 \"" + absPath + "\\out_%6d.ts\""));

		m_createProcessService->create(ffmpeg);

		std::wstring responsews(folder.begin(), folder.end());

		http_response response(status_codes::OK);
		response.headers().set_content_type(L"text/html");
		response.set_body(responsews);

		request.reply(response);
	}
}}}