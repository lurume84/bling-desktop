#pragma once

#include <malloc.h>

#include <boost/thread.hpp>
#include <boost/asio.hpp>

#include "../../Network/Services/HTTPClientService.h"
#include "../../Network/Services/DownloadFileService.h"
#include "../../System/Services/CompressionService.h"
#include "../../System/Services/ReplaceFolderService.h"

namespace desktop { namespace core { namespace agent {
	class UpgradeViewerAgent
	{
	public:
		UpgradeViewerAgent(const std::string& host, const std::string& repository, const std::string& inFolder, const std::string& outFolder,
							std::unique_ptr<service::IDownloadFileService> downloadService = std::make_unique<service::DownloadFileService>(),
							std::unique_ptr<service::HTTPClientService> clientService = std::make_unique<service::HTTPClientService>(),
							std::unique_ptr<service::CompressionService> compressionService = std::make_unique<service::CompressionService>(),
							std::unique_ptr<service::ReplaceFolderService> replaceFolderService = std::make_unique<service::ReplaceFolderService>());
		~UpgradeViewerAgent();

		void execute();
	private:
		void armTimer(unsigned int seconds = 60 * 60 * 12);
	private:
		boost::asio::io_service		m_ioService;
		boost::asio::deadline_timer	m_timer;
		boost::thread				m_backgroundThread;
		std::string					m_host;
		std::string					m_repository;
		std::string					m_inFolder;
		std::string					m_outFolder;
		bool						m_enabled;

		std::unique_ptr<service::IDownloadFileService> m_downloadService;
		std::unique_ptr<service::HTTPClientService> m_clientService;
		std::unique_ptr<service::CompressionService> m_compressionService;
		std::unique_ptr<service::ReplaceFolderService> m_replaceFolderService;
	};
}}}