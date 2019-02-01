#pragma once

#include "BlingCore\Network\Services\IDownloadFileService.h"

#pragma warning(push)
#pragma warning(disable : 4100)
#pragma warning(disable : 4481)
#include <cef/cef_app.h>
#pragma warning(pop)

namespace bling { 
	
	namespace core { namespace service {
		class HTTPClientService;
	}}
	
	namespace ui { namespace service {

	class DownloadFileService : public core::service::IDownloadFileService
	{
	public:
		DownloadFileService(CefRefPtr<CefBrowser> browser);
		~DownloadFileService();
		std::string download(const std::string& host, const std::string& url) const override;
	private:
		CefRefPtr<CefBrowser> m_browser;
	};
}}}