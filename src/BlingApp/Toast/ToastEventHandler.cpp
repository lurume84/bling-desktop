#include "stdafx.h"

#include "ToastEventHandler.h"

namespace bling { namespace ui {  namespace toast {

	ToastEventHandler::ToastEventHandler(std::function<bool()> activated, std::function<bool()> dismissed, std::function<bool()> failed)
	: m_activated(activated)
	, m_dismissed(dismissed)
	, m_failed(failed)
	{

	}

	ToastEventHandler::~ToastEventHandler()
	{
    
	}

	void ToastEventHandler::OnToastActivated(_In_opt_ ABI::Windows::UI::Notifications::IToastNotification* /*pSender*/, _In_opt_ IInspectable* /*pArgs*/)
	{
		m_activated();
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
	
		m_dismissed();
	}

	void ToastEventHandler::OnToastFailed(_In_opt_ ABI::Windows::UI::Notifications::IToastNotification* /*pSender*/, _In_ HRESULT errorCode)
	{
		m_failed();
	}
}}}