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
		auto pos = url.find(host) + host.size();

		std::string script = "window.location = 'https://" + host + url.substr(pos) + "';";

		m_browser->GetMainFrame()->ExecuteJavaScript(script, m_browser->GetMainFrame()->GetURL(), 0);

		return "";
	}
}}}