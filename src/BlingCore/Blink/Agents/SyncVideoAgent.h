#pragma once

#include <malloc.h>

#include <boost/thread.hpp>
#include <boost/asio.hpp>

#include "../../Network/Services/HTTPClientService.h"
#include "../../Network/Services/DownloadFileService.h"

#include "../../Utils\Patterns\PublisherSubscriber\Subscriber.h"

namespace bling { namespace core { 
	
	namespace model
	{
		struct Credentials;
	}
	
	namespace agent {
	
	namespace cup = core::utils::patterns;
	
	class SyncVideoAgent
	{
	public:
		SyncVideoAgent(const std::string& outFolder,
						std::unique_ptr<service::IDownloadFileService> downloadService = std::make_unique<service::DownloadFileService>(),
						std::unique_ptr<service::HTTPClientService> clientService = std::make_unique<service::HTTPClientService>());
		~SyncVideoAgent();

		void execute();
	private:
		void armTimer(unsigned int seconds = 60);
	private:
		boost::asio::io_service		m_ioService;
		boost::asio::deadline_timer	m_timer;
		boost::thread				m_backgroundThread;
		std::string					m_outFolder;
		bool						m_enabled;

		std::unique_ptr<service::IDownloadFileService> m_downloadService;
		std::unique_ptr<service::HTTPClientService> m_clientService;
		std::unique_ptr<model::Credentials>			m_credentials;

		cup::Subscriber m_subscriber;
	};
}}}