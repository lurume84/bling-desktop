#include "UpgradeViewerAgent.h"

#include "Utils\Patterns\PublisherSubscriber\Broker.h"
#include "../Events.h"
#include "System/Services/LogService.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/filesystem.hpp>

namespace desktop { namespace core { namespace agent {

	UpgradeViewerAgent::UpgradeViewerAgent(std::unique_ptr<service::IDownloadFileService> downloadService,
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

		if (m_iniFileService->get<bool>(documents + "Bling.ini", "UpgradeViewer", "Enabled", true))
		{
			m_enabled = true;

			armTimer(5);

			boost::thread t(boost::bind(&boost::asio::io_service::run, &m_ioService));
			m_backgroundThread.swap(t);

			m_host = m_iniFileService->get<std::string>(documents + "Bling.ini", "UpgradeViewer", "Host", "api.github.com");
			m_repository = m_iniFileService->get<std::string>(documents + "Bling.ini", "UpgradeViewer", "Repository", "/repos/lurume84/bling-viewer/releases/latest");
			m_inFolder = m_iniFileService->get<std::string>(documents + "Bling.ini", "UpgradeViewer", "Input", documents + "Download\\Versions\\Viewer\\");
			m_outFolder = m_iniFileService->get<std::string>(documents + "Bling.ini", "UpgradeViewer", "Output", m_applicationService->getViewerFolder());

			boost::filesystem::create_directories(m_inFolder);

			service::LogService::info("Viewer Upgrades from {}{} to {}", m_host, m_repository, m_inFolder);
		}
	}

	UpgradeViewerAgent::~UpgradeViewerAgent()
	{
		m_enabled = false;
		m_timer.cancel();
		m_backgroundThread.join();
		m_ioService.reset();
	}

	void UpgradeViewerAgent::execute()
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

					if (!boost::filesystem::exists(m_inFolder + version + ".zip"))
					{
						auto url = tree.get_child("zipball_url").get_value<std::string>();

						std::map<std::string, std::string> requestHeaders;
						auto path = m_downloadService->download(m_host, url, requestHeaders, m_inFolder + version + ".zip");

						if (path != "")
						{
							service::LogService::info("Viewer Upgrade downloaded at {}", path);

							events::ExtractUpgradeEvent evt(path);
							utils::patterns::Broker::get().publish(evt);

							if (m_compressionService->extract("zip", path, m_inFolder))
							{
								service::LogService::info("Viewer Upgrade {} extracted at {}", path, m_inFolder);

								auto target = boost::filesystem::path(m_inFolder);

								for (auto &it : boost::filesystem::directory_iterator(target))
								{
									if (boost::filesystem::is_directory(it.path()))
									{
										bool fresh = !boost::filesystem::exists(m_outFolder + "/index.html");

										auto inFolder = it.path().string();

										auto dataInFolder = boost::filesystem::path(m_outFolder) / "data";
										auto dataOutFolder = boost::filesystem::path(m_outFolder).parent_path() / "/data";

										if (boost::filesystem::exists(dataInFolder))
										{
											m_folderOperationService->copy(dataInFolder.string(), dataOutFolder.string());
										}

										m_folderOperationService->replace(inFolder, m_outFolder);

										if (boost::filesystem::exists(dataOutFolder))
										{
											boost::filesystem::remove_all(m_outFolder + "/data");
											boost::filesystem::rename(dataOutFolder, m_outFolder + "/data");
										}

										boost::filesystem::rename(path, m_inFolder + version + ".zip");

										events::UpgradeViewerCompletedEvent evt(version, fresh);
										utils::patterns::Broker::get().publish(evt);
										break;
									}
								}
							}
							else
							{
								service::LogService::error("Viewer Upgrade {} could not be extracted", path);
							}
						}
						else
						{
							service::LogService::error("Viewer Upgrade {} could not be downloaded", path);
						}
					}
				}
				catch (std::exception& /*e*/)
				{
					service::LogService::error("Viewer Upgrade: An error occured while checking for updates");
				}
			}

			armTimer();
		}
	}

	void UpgradeViewerAgent::armTimer(unsigned int seconds)
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