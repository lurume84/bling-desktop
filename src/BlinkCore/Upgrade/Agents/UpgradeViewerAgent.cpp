#include "UpgradeViewerAgent.h"

#include "../../Network/Services/HTTPClientService.h"

namespace blink { namespace core { namespace agent {
	UpgradeViewerAgent::UpgradeViewerAgent()
	: m_ioService()
	, m_timer(m_ioService, boost::posix_time::seconds(60))
	{
		execute();
		armTimer();

		boost::thread t(boost::bind(&boost::asio::io_service::run, &m_ioService));
		m_backgroundThread.swap(t);
	}

	UpgradeViewerAgent::~UpgradeViewerAgent()
	{
		m_timer.cancel();
		m_backgroundThread.join();
		m_ioService.reset();
	}

	void UpgradeViewerAgent::execute()
	{
		std::map<std::string, std::string> headers;
		std::string content;

		service::HTTPClientService service("api.github.com", "443");
		if (service.send("/repos/lurume84/blink-viewer/releases/latest", headers, content))
		{
				
		}
	}

	void UpgradeViewerAgent::armTimer()
	{
		m_timer.expires_from_now(boost::posix_time::seconds(60));

		m_timer.async_wait([&](const boost::system::error_code& ec)
		{
			execute();
			armTimer();
		});
	}
}}}