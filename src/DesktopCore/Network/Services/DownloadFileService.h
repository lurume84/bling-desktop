#pragma once

#include "IDownloadFileService.h"

#include "../../System/Services/FileIOService.h"
#include "ParseURIService.h"
#include "HTTPClientService.h"

namespace desktop { namespace core { namespace service {

	class DownloadFileService : public IDownloadFileService
	{
	public:
		DownloadFileService(std::unique_ptr<service::HTTPClientService> clientService = std::make_unique<service::HTTPClientService>(),
							std::unique_ptr<service::ParseURIService> uriService = std::make_unique<service::ParseURIService>(),
							std::unique_ptr<service::FileIOService> fileIOService = std::make_unique<service::FileIOService>());
		~DownloadFileService();
		std::string download(const std::string& host, const std::string& url, std::map<std::string, std::string> requestHeaders, const std::string &folder) const override;
	private:
		std::unique_ptr<service::HTTPClientService> m_clientService;
		std::unique_ptr<service::ParseURIService> m_uriService;
		std::unique_ptr<service::FileIOService> m_fileIOService;
	};
}}}