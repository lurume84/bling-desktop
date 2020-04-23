#include "LiveViewAgent.h"

#include "Utils\Patterns\PublisherSubscriber\Broker.h"
#include "../../Network/Events.h"
#include "..\..\Network\Model\Credentials.h"
#include "..\..\System\Services\IniFileService.h"
#include "System\Model\ExecutableFile.h"
#include "System\Model\ProcessInformation.h"
#include "System/Services/LogService.h"

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
#include <boost/thread.hpp>

#include <httplib.h>

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
	, m_server(std::make_unique<httplib::Server>())
	{
		auto documents = m_applicationService->getMyDocuments();

		{
			m_seconds = m_iniFileService->get<unsigned int>(documents + "Blink.ini", "LiveView", "Interval", 10);

			m_outFolder = m_iniFileService->get<std::string>(documents + "Blink.ini", "LiveView", "Output", documents + "Download\\Videos\\");

			m_saveLocalTime = m_iniFileService->get<bool>(documents + "Blink.ini", "LiveView", "UseLocalTime", false);
		}

		auto host = m_iniFileService->get<std::string>(documents + "Blink.ini", "LiveView", "Host", "127.0.0.1");
		auto port = m_iniFileService->get<int>(documents + "Blink.ini", "LiveView", "Port", 9192);

		m_endpoint = "http://" + host + ":" + std::to_string(port);

		m_server->Get(".*", [this](const httplib::Request& req, httplib::Response& res) {handleGET(req, res); });
		m_server->Post(".*", [this](const httplib::Request& req, httplib::Response& res, const httplib::ContentReader &content_reader) {handlePOST(req, res, content_reader); });
		m_server->Delete(".*", [this](const httplib::Request& req, httplib::Response& res) {handleDELETE(req, res); });

		service::LogService::info("Listening LiveViewServer at {}:{}", host, port);

		std::thread([=]() 
		{
			m_server->listen(host.c_str(), port);
		}).detach();
	}

	LiveViewAgent::~LiveViewAgent()
	{
		m_enabled = false;

		m_server->stop();

		for(auto& liveView : m_liveViews)
		{
			m_terminateProcessService->sigint(*liveView.second);
			m_liveViews.erase(liveView.first);
		}
	}

	void LiveViewAgent::handleGET(const httplib::Request& req, httplib::Response& res) const
	{
		auto body = req.path;

		body = boost::replace_all_copy(body, "/", "\\");

		boost::replace_all(body, "%20", " ");

		boost::filesystem::path path(m_outFolder + body);
		
		std::string pepe = path.extension().string();

		if(path.extension() == ".m3u8")
		{
			while(!boost::filesystem::exists(path) && m_enabled)
			{
				boost::this_thread::sleep_for(boost::chrono::milliseconds(200));
			}
		}

		if (boost::filesystem::exists(path))
		{
			std::ifstream ifs(path.string(), std::ifstream::binary);
			std::string data = std::string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());

			res.set_content(data, "application/x-mpegURL");
		}
		else
		{
			res.status = 404;
		}

		service::LogService::info("HTTP GET {} LiveViewServer {}", res.status, body);
	}

	void LiveViewAgent::handlePOST(const httplib::Request& req, httplib::Response& res, const httplib::ContentReader &content_reader)
	{
		std::stringstream ss;
		content_reader([&](const char *data, size_t data_length) 
		{
			ss << std::string(data, data_length);
			return true;
		});

		boost::property_tree::ptree payload;
		boost::property_tree::read_json(ss, payload);

		std::string url = payload.get<std::string>("url");

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

		int camera_id = payload.get<int>("camera_id");

		std::stringstream camera_idss;
		camera_idss << camera_id;

		auto process = m_createProcessService->create(ffmpeg, absPath);

		m_liveViews.insert(std::make_pair(camera_id, std::move(process)));

		boost::replace_all(folder, "\\", "/");

		std::string endpoint = m_endpoint + "/" + boost::replace_all_copy(folder, "\\", "/") + "/out.m3u8";

		res.set_content("{\"camera_id\": " + camera_idss.str() + ", \"url\": \"" + endpoint + "\"}", "application/json");

		service::LogService::info("HTTP POST {} LiveViewServer {}", res.status, url);

		service::LogService::info("Execute FFMPEG {}", arguments);
	}

	void LiveViewAgent::handleDELETE(const httplib::Request& req, httplib::Response& res)
	{
		int camera_id;

		std::istringstream is(req.get_param_value("camera_id"));
		is >> camera_id;

		auto liveView = m_liveViews.find(camera_id);

		if (liveView != m_liveViews.end())
		{
			m_terminateProcessService->sigint(*liveView->second);

			m_liveViews.erase(camera_id);

			res.set_content("{}", "application/json");
		}
		else
		{
			res.status = 404;
		}

		service::LogService::info("HTTP DELETE {} LiveViewServer Camera {}", res.status, camera_id);
	}
}}}