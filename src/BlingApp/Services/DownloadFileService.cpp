#include "stdafx.h"

#include "DownloadFileService.h"

namespace bling { namespace ui { namespace service {

	DownloadFileService::DownloadFileService(CefRefPtr<CefBrowser> browser)
	: m_browser(browser)
	{
	
	}

	DownloadFileService::~DownloadFileService() = default;

	std::string DownloadFileService::download(const std::string& host, const std::string& url) const
	{
		std::map<std::string, std::string> headers;
		std::string content;
		unsigned int status;

		auto pos = url.find(host) + host.size();

		std::string file;

		return "";
	}
}}}