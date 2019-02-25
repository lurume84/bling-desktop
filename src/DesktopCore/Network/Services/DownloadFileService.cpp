#include "DownloadFileService.h"

namespace desktop { namespace core { namespace service {

	DownloadFileService::DownloadFileService(std::unique_ptr<service::HTTPClientService> clientService, 
											 std::unique_ptr<service::ParseURIService> uriService,
											 std::unique_ptr<service::FileIOService> fileIOService)
	: m_clientService(std::move(clientService))
	, m_uriService(std::move(uriService))
	, m_fileIOService(std::move(fileIOService))
	{
	
	}

	DownloadFileService::~DownloadFileService() = default;

	std::string DownloadFileService::download(const std::string& host, const std::string& url, const std::string &folder) const
	{
		std::map<std::string, std::string> requestHeaders, responseHeaders;
		unsigned int status;

		auto pos = url.find(host) + host.size();

		std::string file;

		if (m_clientService->send(host, "443", url.substr(pos), requestHeaders, responseHeaders, file, status))
		{
			if (status == 302)
			{
				auto location = responseHeaders.find("Location");
				if (location != responseHeaders.end())
				{
					std::string protocol, domain, port, path, query, fragment;

					if (m_uriService->parse(location->second, protocol, domain, port, path, query, fragment))
					{
						std::map<std::string, std::string> requestHeaders, responseHeaders;

						if (m_clientService->send(domain, "443", path, requestHeaders, responseHeaders, file, status) && status == 200)
						{
							if (m_fileIOService->save(folder, file))
							{
								return folder;
							}
						}
					}
				}
			}
			else if (status == 200)
			{
				if (m_fileIOService->save(folder + "latest.zip", file))
				{
					return folder + "latest.zip";
				}
			}
		}

		return "";
	}
}}}