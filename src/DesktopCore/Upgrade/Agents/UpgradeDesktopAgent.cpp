#include "UpgradeDesktopAgent.h"

#include "Utils\Patterns\PublisherSubscriber\Broker.h"
#include "../Events.h"
#include "System/Services/LogService.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/filesystem.hpp>

namespace desktop { namespace core { namespace agent {

	UpgradeDesktopAgent::UpgradeDesktopAgent(std::unique_ptr<service::IDownloadFileService> downloadService,
		std::unique_ptr<service::IniFileService> iniFileService,
		std::unique_ptr<service::ApplicationDataService> applicationService,
		std::unique_ptr<service::HTTPClientService> clientService,
		std::unique_ptr<service::CompressionService> compressionService,
		std::unique_ptr<service::FolderOperationService> folderOperationService)
		: m_ioService()
		, m_timer(m_ioService, boost::posix_time::seconds(60 * 10))
		, m_downloadService(std::move(downloadService))
		, m_clientService(std::move(clientService))
		, m_compressionService(std::move(compressionService))
		, m_folderOperationService(std::move(folderOperationService))
		, m_applicationService(std::move(applicationService))
		, m_iniFileService(std::move(iniFileService))
		, m_enabled(false)
	{
		auto documents = m_applicationService->getMyDocuments();

		if (m_iniFileService->get<bool>(documents + "Bling.ini", "UpgradeDesktop", "Enabled", true))
		{
			m_enabled = true;

			armTimer(1);

			boost::thread t(boost::bind(&boost::asio::io_service::run, &m_ioService));
			m_backgroundThread.swap(t);

			m_host = m_iniFileService->get<std::string>(documents + "Bling.ini", "UpgradeDesktop", "Host", "api.github.com");
			m_repository = m_iniFileService->get<std::string>(documents + "Bling.ini", "UpgradeDesktop", "Repository", "/repos/lurume84/bling-desktop/releases/latest");
			m_inFolder = m_iniFileService->get<std::string>(documents + "Bling.ini", "UpgradeDesktop", "Input", documents + "Download\\Versions\\Desktop\\");

			boost::filesystem::create_directories(m_inFolder);

			service::LogService::info("Desktop Upgrades from {}{} to {}", m_host, m_repository, m_inFolder);
		}
	}

	UpgradeDesktopAgent::~UpgradeDesktopAgent()
	{
		m_enabled = false;
		m_timer.cancel();

		m_downloadService->cancel();

		m_backgroundThread.join();
		m_ioService.reset();
	}

	void UpgradeDesktopAgent::execute()
	{
		if (m_enabled)
		{
			std::map<std::string, std::string> requestHeaders, responseHeaders;
			std::string content;
			unsigned int status;

			if (m_clientService->get(m_host, "443", m_repository, requestHeaders, responseHeaders, content, status))
			{
				try
				{
					std::stringstream ss(content);
					boost::property_tree::ptree tree;
					boost::property_tree::json_parser::read_json(ss, tree);

					auto version = tree.get_child("tag_name").get_value<std::string>();

					if (!boost::filesystem::exists(m_inFolder + version + ".exe"))
					{
						auto assets = tree.get_child("assets");

						for (auto asset : assets)
						{
							std::string url = asset.second.get_child("browser_download_url").get_value<std::string>();

							size_t pos = url.find("Setup.exe");

							if (pos != std::string::npos)
							{
								std::map<std::string, std::string> requestHeaders;
								auto path = m_downloadService->download(url, requestHeaders, m_inFolder + version + ".exe");

								if (path != "")
								{
									service::LogService::info("Desktop Upgrade {} downloaded at {}", url, path);

									events::UpgradeDesktopCompletedEvent evt(version, path);
									utils::patterns::Broker::get().publish(evt);
								}
								else
								{
									service::LogService::error("Desktop Upgrade {} could not be downloaded at {}", url, m_inFolder + version + ".exe");
								}
							}
							else
							{
								service::LogService::error("Desktop Upgrade {} could not be downloaded, does not contain 'Setup.exe'", url);
							}
						}
					}
				}
				catch (std::exception& /*e*/)
				{
					service::LogService::error("Desktop Upgrade: An error occured while checking for updates");
				}
			}

			armTimer();
		}
	}

	void UpgradeDesktopAgent::armTimer(unsigned int seconds)
	{
		if (m_enabled)
		{
			m_timer.expires_from_now(boost::posix_time::seconds(seconds));

			m_timer.async_wait([&](const boost::system::error_code& ec)
			{
				execute();
			});
		}
	}
}}}