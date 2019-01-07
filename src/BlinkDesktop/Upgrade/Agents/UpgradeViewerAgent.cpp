#include "UpgradeViewerAgent.h"

namespace blink { namespace agents {
	UpgradeViewerAgent::UpgradeViewerAgent()
	: m_ioService()
	, m_timer(m_ioService, boost::posix_time::seconds(1000))
	{
		m_timer.async_wait([](const boost::system::error_code& ec)
		{
		
		});

		boost::thread t(boost::bind(&boost::asio::io_service::run, &m_ioService));
		m_backgroundThread.swap(t);
	}

	UpgradeViewerAgent::~UpgradeViewerAgent()
	{
		m_timer.cancel();
		m_backgroundThread.join();
		m_ioService.reset();
	}
}}