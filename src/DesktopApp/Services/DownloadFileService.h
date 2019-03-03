#pragma once

#include "DesktopCore\Utils\Patterns\PublisherSubscriber\Subscriber.h"

#include "DesktopCore\System\Services\EncodeStringService.h"
#include "DesktopCore\Network\Services\IDownloadFileService.h"

#pragma warning(push)
#pragma warning(disable : 4100)
#pragma warning(disable : 4481)
#include <cef/cef_app.h>
#pragma warning(pop)

#include <mutex>
#include <memory>
#include <condition_variable>

namespace ToastPP
{
	class CToast;
}

namespace desktop { 
	
	namespace core { namespace service {
		class HTTPClientService;
	}}
	
	namespace ui { 
		
		namespace toast
		{
			class ToastEventHandler;
		}
		
	namespace service {

	namespace cup = core::utils::patterns;

	class DownloadFileService : public core::service::IDownloadFileService
	{
	public:
		DownloadFileService(CefRefPtr<CefBrowser> browser, 
							std::unique_ptr<core::service::EncodeStringService> encodeService = std::make_unique<core::service::EncodeStringService>());
		~DownloadFileService();
		std::string download(const std::string& host, const std::string& url, std::map<std::string, std::string> requestHeaders, const std::string &folder) const override;
	private:
		CefRefPtr<CefBrowser>	m_browser;
		cup::Subscriber			m_subscriber;

		std::string				m_path;

		std::unique_ptr<core::service::EncodeStringService> m_encodeService;

		mutable std::condition_variable m_cv;
		mutable std::mutex				m_mutex;

		std::shared_ptr<toast::ToastEventHandler> m_handler;
		std::shared_ptr<ToastPP::CToast> m_toast;
	};
}}}