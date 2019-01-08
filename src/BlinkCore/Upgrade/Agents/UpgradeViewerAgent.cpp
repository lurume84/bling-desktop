#include "UpgradeViewerAgent.h"

#include "../../Network/Services/HTTPClientService.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/regex.hpp>
#include <boost/filesystem.hpp>

namespace blink { namespace core { namespace agent {

	UpgradeViewerAgent::UpgradeViewerAgent(const std::string& host)
	: m_ioService()
	, m_timer(m_ioService, boost::posix_time::seconds(60))
	, m_host(host)
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
		unsigned int status;

		service::HTTPClientService service(m_host, "443");
		if (service.send("/repos/lurume84/blink-viewer/releases/latest", headers, content, status))
		{
			try
			{
				std::stringstream ss(content);
				boost::property_tree::ptree tree;
				boost::property_tree::json_parser::read_json(ss, tree);

				auto version = tree.get_child("tag_name").get_value<std::string>();

				if (!boost::filesystem::exists("versions/" + version + ".zip"))
				{
					download(tree.get_child("zipball_url").get_value<std::string>(), version);
				}
			}
			catch (std::exception& /*e*/)
			{
				
			}
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

	void UpgradeViewerAgent::download(const std::string& url, const std::string& version)
	{
		std::map<std::string, std::string> headers;
		std::string content;
		unsigned int status;

		auto pos = url.find(m_host) + m_host.size();

		std::string file;

		service::HTTPClientService service(m_host, "443");
		if (service.send(url.substr(pos), headers, file, status))
		{
			if (status == 302)
			{
				auto location = headers.find("Location");
				if (location != headers.end())
				{
					std::string protocol, domain, port, path, query, fragment;

					if (parseURI(location->second, protocol, domain, port, path, query, fragment))
					{
						std::map<std::string, std::string> headers;

						service::HTTPClientService service(domain, "443");
						if (service.send(path, headers, file, status))
						{
							save(version, file);
						}
					}
				}
			}
			else
			{
				save(version, file);
			}
		}
	}

	void UpgradeViewerAgent::save(const std::string& fileName, const std::string& content)
	{
		boost::filesystem::create_directories("versions");

		std::ofstream f("versions/" + fileName + ".zip", std::ios::binary);
		f << content;
		f.close();
	}

	bool UpgradeViewerAgent::parseURI(const std::string& uri, std::string& protocol, std::string& domain, std::string& port, std::string& path, std::string& query, std::string& fragment)
	{
		boost::regex ex("(http|https)://([^/ :]+):?([^/ ]*)(/?[^ #?]*)\\x3f?([^ #]*)#?([^ ]*)");
		boost::cmatch what;

		if (regex_match(uri.c_str(), what, ex))
		{
			protocol = std::string(what[1].first, what[1].second);
			domain = std::string(what[2].first, what[2].second);
			port = std::string(what[3].first, what[3].second);
			path = std::string(what[4].first, what[4].second);
			query = std::string(what[5].first, what[5].second);
			fragment = std::string(what[6].first, what[6].second);

			return true;
		}
		else
		{
			return false;
		}
	}
}}}