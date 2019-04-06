#pragma once

#include "DesktopCore\Utils\Patterns\PublisherSubscriber\Subscriber.h"

#include "DesktopCore\Blink\Services\IActivityNotificationService.h"

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

	namespace ui { 
		
		namespace toast
		{
			class ToastEventHandler;
		}
		
	namespace service {

	namespace cup = core::utils::patterns;

	class ActivityNotificationService : public core::service::IActivityNotificationService
	{
	public:
		ActivityNotificationService(CefRefPtr<CefBrowser> browser);
		~ActivityNotificationService();
		void notify() override;
	private:
		CefRefPtr<CefBrowser>	m_browser;
		cup::Subscriber			m_subscriber;

		std::shared_ptr<toast::ToastEventHandler> m_handler;
		std::shared_ptr<ToastPP::CToast> m_toast;
	};
}}}