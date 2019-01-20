#include <malloc.h>

#include <boost/thread.hpp>
#include <boost/asio.hpp>

namespace blink { namespace core { namespace agent {
	class UpgradeViewerAgent
	{
	public:
		UpgradeViewerAgent(const std::string& host);
		~UpgradeViewerAgent();

		void execute();
	private:
		void armTimer(unsigned int seconds = 60 * 60 * 12);
		void download(const std::string&, const std::string&);
		void save(const std::string&, const std::string&);
		bool parseURI(const std::string& uri, std::string& protocol, std::string& domain, std::string& port, std::string& path, std::string& query, std::string& fragment);
	private:
		boost::asio::io_service		m_ioService;
		boost::asio::deadline_timer	m_timer;
		boost::thread				m_backgroundThread;
		std::string					m_host;
	};
}}}