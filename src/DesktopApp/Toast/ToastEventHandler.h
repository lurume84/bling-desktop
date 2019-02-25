#pragma once

#include "Toast\Toast.h"
#include "DesktopCore\Model\Notification.h"

#include <memory>

namespace desktop { namespace ui {  namespace toast {
	class ToastEventHandler : public ToastPP::INotifier 
	{
	public:
		ToastEventHandler::ToastEventHandler(std::unique_ptr<core::model::Notification> notification);
		~ToastEventHandler();
       
		void OnToastActivated(_In_opt_ ABI::Windows::UI::Notifications::IToastNotification* pSender, _In_opt_ IInspectable* pArgs) override;
		void OnToastDismissed(_In_opt_ ABI::Windows::UI::Notifications::IToastNotification* pSender, _In_ ABI::Windows::UI::Notifications::ToastDismissalReason reason) override;
		void OnToastFailed(_In_opt_ ABI::Windows::UI::Notifications::IToastNotification* pSender, _In_ HRESULT errorCode) override;

	private:
		std::unique_ptr<core::model::Notification> m_notification;
	};
}}}
