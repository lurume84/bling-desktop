#include <malloc.h>

#include <boost/thread.hpp>
#include <boost/asio.hpp>

namespace blink { namespace core { namespace agent {
	class UpgradeViewerAgent
	{
	public:
		UpgradeViewerAgent();
		~UpgradeViewerAgent();

		void execute();
	private:
		boost::asio::io_service		m_ioService;
		boost::asio::deadline_timer	m_timer;
		boost::thread				m_backgroundThread;
	};
}}}