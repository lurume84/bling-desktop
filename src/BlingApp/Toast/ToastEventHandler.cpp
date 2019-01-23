#include "stdafx.h"

#include "ToastEventHandler.h"

using namespace ABI::Windows::UI::Notifications;

ToastEventHandler::ToastEventHandler(std::function<bool()> activated, std::function<bool()> dismissed, std::function<bool()> failed)
: _ref(1)
, m_activated(activated)
, m_dismissed(dismissed)
, m_failed(failed)
{

}

ToastEventHandler::~ToastEventHandler()
{
    
}

// DesktopToastActivatedEventHandler
IFACEMETHODIMP ToastEventHandler::Invoke(_In_ IToastNotification* /* sender */, _In_ IInspectable* /* args */)
{
    return m_activated() ? S_OK : E_FAIL;
}

// DesktopToastDismissedEventHandler
IFACEMETHODIMP ToastEventHandler::Invoke(_In_ IToastNotification* /* sender */, _In_ IToastDismissedEventArgs* e)
{
    ToastDismissalReason tdr;
    HRESULT hr = e->get_Reason(&tdr);

    if (SUCCEEDED(hr))
    {
        wchar_t *outputText;
        switch (tdr)
        {
        case ToastDismissalReason_ApplicationHidden:
            outputText = L"The application hid the toast using ToastNotifier.hide()";
            break;
        case ToastDismissalReason_UserCanceled:
            outputText = L"The user dismissed this toast";
            break;
        case ToastDismissalReason_TimedOut:
            outputText = L"The toast has timed out";
            break;
        default:
            outputText = L"Toast not activated";
            break;
        }

        hr = m_dismissed() ? S_OK : E_FAIL;
    }
    return hr;
}

// DesktopToastFailedEventHandler
IFACEMETHODIMP ToastEventHandler::Invoke(_In_ IToastNotification* /* sender */, _In_ IToastFailedEventArgs* /* e */)
{
    return m_failed() ? S_OK : E_FAIL;
}
