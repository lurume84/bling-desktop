#include "UpgradeViewerAgent.h"

#include "Utils\Patterns\PublisherSubscriber\Broker.h"
#include "../Events.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/filesystem.hpp>

namespace bling { namespace core { namespace agent {

	UpgradeViewerAgent::UpgradeViewerAgent(const std::string& host, std::unique_ptr<service::IDownloadFileService> downloadService,
																	std::unique_ptr<service::HTTPClientService> clientService,
																	std::unique_ptr<service::FileIOService> fileIOService,
																	std::unique_ptr<service::CompressionService> compressionService,
																	std::unique_ptr<service::ReplaceFolderService> replaceFolderService)
	: m_ioService()
	, m_timer(m_ioService, boost::posix_time::seconds(60 * 60 * 12))
	, m_host(host)
	, m_downloadService(std::move(downloadService))
	, m_clientService(std::move(clientService))
	, m_fileIOService(std::move(fileIOService))
	, m_compressionService(std::move(compressionService))
	, m_replaceFolderService(std::move(replaceFolderService))
	{
		armTimer(1);

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

		if (m_clientService->send(m_host, "443", "/repos/lurume84/bling-viewer/releases/latest", headers, content, status))
		{
			try
			{
				std::stringstream ss(content);
				boost::property_tree::ptree tree;
				boost::property_tree::json_parser::read_json(ss, tree);

				auto version = tree.get_child("tag_name").get_value<std::string>();

				if (!boost::filesystem::exists("versions/" + version + ".zip"))
				{
					events::DownloadUpgradeEvent evt(version);
					utils::patterns::Broker::get().publish(evt);

					auto fileContent = m_downloadService->download(m_host, tree.get_child("zipball_url").get_value<std::string>());

					if (fileContent != "")
					{
						if (m_fileIOService->save("versions/" + version + ".zip", fileContent))
						{
							events::ExtractUpgradeEvent evt(version + ".zip");
							utils::patterns::Broker::get().publish(evt);

							if (m_compressionService->extract("zip", "versions/" + version + ".zip", "versions/"))
							{
								auto target = boost::filesystem::path("versions/");
								boost::filesystem::directory_iterator it(target);

								m_replaceFolderService->replace(it->path().string(), "Html/viewer");

								events::UpgradeCompletedEvent evt(version);
								utils::patterns::Broker::get().publish(evt);
							}
						}
					}
				}
			}
			catch (std::exception& /*e*/)
			{
				
			}
		}
	}

	void UpgradeViewerAgent::armTimer(unsigned int seconds)
	{
		m_timer.expires_from_now(boost::posix_time::seconds(seconds));

		m_timer.async_wait([&](const boost::system::error_code& ec)
		{
			execute();
			armTimer();
		});
	}
}}}