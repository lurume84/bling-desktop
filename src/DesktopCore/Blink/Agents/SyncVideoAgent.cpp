#include "SyncVideoAgent.h"

#include "Utils\Patterns\PublisherSubscriber\Broker.h"
#include "../../Network/Events.h"
#include "..\..\Network\Model\Credentials.h"

#include <boost/filesystem.hpp>

namespace desktop { namespace core { namespace agent {

	SyncVideoAgent::SyncVideoAgent(const std::string& outFolder,
									std::unique_ptr<service::IDownloadFileService> downloadService,
									std::unique_ptr<service::HTTPClientService> clientService)
	: m_ioService()
	, m_timer(m_ioService, boost::posix_time::seconds(60))
	, m_downloadService(std::move(downloadService))
	, m_clientService(std::move(clientService))
	, m_outFolder(outFolder)
	{
		m_subscriber.subscribe([this](const desktop::core::utils::patterns::Event& rawEvt)
		{
			const auto& evt = static_cast<const core::events::CredentialsEvent&>(rawEvt);

			m_credentials = std::make_unique<model::Credentials>(evt.m_credentials);

			if (!m_enabled)
			{
				m_enabled = true;
				armTimer(1);

				boost::thread t(boost::bind(&boost::asio::io_service::run, &m_ioService));
				m_backgroundThread.swap(t);
			}
		}, events::CREDENTIALS_EVENT);
	}

	SyncVideoAgent::~SyncVideoAgent()
	{
		m_enabled = false;
		m_timer.cancel();
		m_backgroundThread.join();
		m_ioService.reset();
	}

	void SyncVideoAgent::execute()
	{
		if (m_enabled && m_credentials)
		{
			std::map<std::string, std::string> requestHeaders, responseHeaders;
			std::string content;
			unsigned int status;

			requestHeaders["token_auth"] = m_credentials->m_token;

			if (m_clientService->send(m_credentials->m_host, m_credentials->m_port, "/api/v2/videos/changed?since=-999999999-01-01T00:00:00+18:00", requestHeaders, responseHeaders, content, status))
			{
				
			}
		}
	}

	void SyncVideoAgent::armTimer(unsigned int seconds)
	{
		m_timer.expires_from_now(boost::posix_time::seconds(seconds));

		m_timer.async_wait([&](const boost::system::error_code& ec)
		{
			execute();
			armTimer();
		});
	}
}}}