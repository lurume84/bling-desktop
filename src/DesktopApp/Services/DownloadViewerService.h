#pragma once

#include "DesktopCore\Utils\Patterns\PublisherSubscriber\Subscriber.h"

#include "DesktopCore\System\Services\EncodeStringService.h"
#include "DesktopCore\System\Services\ApplicationDataService.h"
#include "DesktopCore\Network\Services\IDownloadFileService.h"

#include <mutex>
#include <memory>
#include <condition_variable>

class CefBrowser;

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

	class DownloadViewerService : public core::service::IDownloadFileService
	{
	public:
		DownloadViewerService(CefBrowser& browser,
							std::unique_ptr<core::service::EncodeStringService> encodeService = std::make_unique<core::service::EncodeStringService>(),
							std::unique_ptr<core::service::ApplicationDataService> applicationService = std::make_unique<core::service::ApplicationDataService>());
		~DownloadViewerService();
		std::string download(const std::string& host, const std::string& url, std::map<std::string, std::string> requestHeaders, const std::string &folder) const override;
	private:
		CefBrowser&	m_browser;
		cup::Subscriber			m_subscriber;

		std::string				m_path;

		std::unique_ptr<core::service::EncodeStringService> m_encodeService;
		std::unique_ptr<core::service::ApplicationDataService> m_applicationService;

		mutable std::condition_variable m_cv;
		mutable std::mutex				m_mutex;

		std::shared_ptr<toast::ToastEventHandler> m_handler;
		std::shared_ptr<ToastPP::CToast> m_toast;
	};
}}}