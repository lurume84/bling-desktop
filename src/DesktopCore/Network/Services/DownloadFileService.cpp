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

	std::string DownloadFileService::download(const std::string& url, std::map<std::string, std::string> requestHeaders, const std::string &folder) const
	{
		std::string protocol, domain, port, path, query, fragment;

		if (m_uriService->parse(url, protocol, domain, port, path, query, fragment))
		{
			return download(domain, path, requestHeaders, folder);
		}

		return "";
	}

	std::string DownloadFileService::download(const std::string& host, const std::string& url, std::map<std::string, std::string> requestHeaders, const std::string &folder) const
	{
		std::map<std::string, std::string> responseHeaders;
		unsigned int status;

		std::string file;

		if (m_clientService->get(host, "443", url, requestHeaders, responseHeaders, file, status))
		{
			if (status == 302)
			{
				auto location = responseHeaders.find("Location");
				if (location == responseHeaders.end())
				{
					location = responseHeaders.find("location");
				}

				if (location != responseHeaders.end())
				{
					std::string protocol, domain, port, path, query, fragment;

					if (m_uriService->parse(location->second, protocol, domain, port, path, query, fragment))
					{
						std::map<std::string, std::string> requestHeaders, responseHeaders;

						bool downloaded = m_clientService->get(domain, "443", path + "?" + query, requestHeaders, responseHeaders, file, status);

						if (downloaded && (status == 200 || status == 204))
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
				if (m_fileIOService->save(folder, file))
				{
					return folder;
				}
			}
		}

		return "";
	}

	void DownloadFileService::cancel()
	{
		
	}
}}}