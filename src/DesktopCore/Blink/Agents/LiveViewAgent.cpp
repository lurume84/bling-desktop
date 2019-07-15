#include "LiveViewAgent.h"

#include "Utils\Patterns\PublisherSubscriber\Broker.h"
#include "../../Network/Events.h"
#include "..\..\Network\Model\Credentials.h"
#include "..\..\System\Services\IniFileService.h"
#include "System\Model\ExecutableFile.h"
#include "System\Model\ProcessInformation.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <chrono>
#include <thread>

namespace desktop { namespace core { namespace agent {

	LiveViewAgent::LiveViewAgent(std::unique_ptr<service::system::ICreateProcessService> createProcessService, 
									std::unique_ptr<service::HTTPClientService> clientService,
									std::unique_ptr<service::ApplicationDataService> applicationService,
									std::unique_ptr<service::IniFileService> iniFileService,
									std::unique_ptr<service::TimestampFolderService> timestampFolderService)
	: m_ioService()
	, m_iniFileService(std::move(iniFileService))
	, m_clientService(std::move(clientService))
	, m_applicationService(std::move(applicationService))
	, m_timestampFolderService(std::move(timestampFolderService))
	, m_createProcessService(std::move(createProcessService))
	{
		auto documents = m_applicationService->getMyDocuments();

		{
			m_seconds = m_iniFileService->get<unsigned int>(documents + "Blink.ini", "Activity", "Interval", 10);

			m_timer = std::make_unique<boost::asio::deadline_timer>(m_ioService, boost::posix_time::seconds(m_seconds));
		}

		m_subscriber.subscribe([this](const desktop::core::utils::patterns::Event& rawEvt)
		{
			const auto& evt = static_cast<const core::events::LiveViewEvent&>(rawEvt);

			m_RTP = std::make_unique<model::RTP>(evt.m_rtp);

			auto viewerFolder = m_applicationService->getViewerFolder();
			auto appFolder = m_applicationService->getApplicationFolder();

			model::system::ExecutableFile ffmpeg(model::system::ExecutableFile::Path(appFolder + "\\ffmpeg.exe"), model::system::ExecutableFile::Arguments("-i " + m_RTP->m_url + " -vcodec libx264 -f ssegment -hls_flags delete_segments -segment_list " + viewerFolder + "\\out.m3u8 -segment_list_type hls -segment_list_size 0 " + viewerFolder + "\\out_%6d.ts"));

			m_createProcessService->create(ffmpeg);

			/*if (!m_enabled)
			{
				m_enabled = true;

				armTimer(1);

				boost::thread t(boost::bind(&boost::asio::io_service::run, &m_ioService));
				m_backgroundThread.swap(t);
			}*/
		}, events::LIVEVIEW_EVENT);
	}

	LiveViewAgent::~LiveViewAgent()
	{
		m_enabled = false;
		m_timer->cancel();
		m_backgroundThread.join();
		m_ioService.reset();
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
}}}