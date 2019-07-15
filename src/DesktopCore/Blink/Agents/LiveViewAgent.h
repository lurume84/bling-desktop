#pragma once

#include "../../Network/Services/HTTPClientService.h"
#include "../../System/Services/ApplicationDataService.h"
#include "../../System/Services/IniFileService.h"
#include "../../System/Services/TimestampFolderService.h"
#include "../../System/Services/Process/CreateProcessService.h"
#include "../../Utils\Patterns\PublisherSubscriber\Subscriber.h"
#include "../../Model/IAgent.h"

#include <string>
#include <map>
#include <boost/thread.hpp>
#include <boost/asio.hpp>

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
						std::unique_ptr<service::HTTPClientService> clientService = std::make_unique<service::HTTPClientService>(),
						std::unique_ptr<service::ApplicationDataService> applicationService = std::make_unique<service::ApplicationDataService>(),
						std::unique_ptr<service::IniFileService> iniFileService = std::make_unique<service::IniFileService>(),
						std::unique_ptr<service::TimestampFolderService> timestampFolderService = std::make_unique<service::TimestampFolderService>());
		~LiveViewAgent();

		void execute();
	private:
		void armTimer(unsigned int seconds = 10);
	private:
		std::unique_ptr<service::IniFileService> m_iniFileService;

		boost::asio::io_service		m_ioService;
		std::unique_ptr<boost::asio::deadline_timer>	m_timer;
		boost::thread				m_backgroundThread;
		bool						m_enabled;
		unsigned int				m_seconds;

		std::unique_ptr<service::HTTPClientService> m_clientService;
		std::unique_ptr<model::RTP>			m_RTP;
		std::unique_ptr<service::ApplicationDataService> m_applicationService;
		std::unique_ptr<service::TimestampFolderService> m_timestampFolderService;
		std::unique_ptr<service::system::ICreateProcessService> m_createProcessService;

		cup::Subscriber m_subscriber;
	};
}}}