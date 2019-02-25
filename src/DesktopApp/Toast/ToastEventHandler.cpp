#include "stdafx.h"

#include "ToastEventHandler.h"

#include "DesktopCore\Model\Notification.h"

namespace desktop { namespace ui {  namespace toast {

	ToastEventHandler::ToastEventHandler(std::unique_ptr<core::model::Notification> notification)
	: m_notification(std::move(notification))
	{

	}

	ToastEventHandler::~ToastEventHandler()
	{

	}

	void ToastEventHandler::OnToastActivated(_In_opt_ ABI::Windows::UI::Notifications::IToastNotification* /*pSender*/, _In_opt_ IInspectable* /*pArgs*/)
	{
		m_notification->m_activated();
	}

	void ToastEventHandler::OnToastDismissed(_In_opt_ ABI::Windows::UI::Notifications::IToastNotification* /*pSender*/, _In_ ABI::Windows::UI::Notifications::ToastDismissalReason reason)
	{
		CString sMsg;
		switch (reason)
		{
			case ABI::Windows::UI::Notifications::ToastDismissalReason_UserCanceled:
			{
				sMsg = _T("IToastNotification, The user dismissed the toast\r\n");
				break;
			}
			case ABI::Windows::UI::Notifications::ToastDismissalReason_ApplicationHidden:
			{
				sMsg = _T("IToastNotification, The application programatically hid the toast\r\n");
				break;
			}
			case ABI::Windows::UI::Notifications::ToastDismissalReason_TimedOut:
			{
				sMsg = _T("IToastNotification, The toast has timed out\r\n");
				break;
			}
			default:
			{
				sMsg.Format(_T("IToastNotification, The toast was dimissed for an unknown reason %d\r\n"), reason);
				break;
			}
		}
	
		m_notification->m_dismissed();
	}

	void ToastEventHandler::OnToastFailed(_In_opt_ ABI::Windows::UI::Notifications::IToastNotification* /*pSender*/, _In_ HRESULT errorCode)
	{
		m_notification->m_failed();
	}
}}}