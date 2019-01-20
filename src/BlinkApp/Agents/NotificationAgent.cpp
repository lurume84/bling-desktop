#include "NotificationAgent.h"

#include "BlinkCore\Upgrade\Events.h"

HRESULT CToastNotificationActivationCallback::Activate(__RPC__in_string LPCWSTR appUserModelId, __RPC__in_opt_string LPCWSTR invokedArgs,
	__RPC__in_ecount_full_opt(count) const NOTIFICATION_USER_INPUT_DATA* data, ULONG count)
{
	CStringW sMsg;
	sMsg.Format(L"INotificationActivationCallback, The toast was activated. appUserModelId:\"%s\", Arguments:\"%s\"\r\n", appUserModelId, invokedArgs);
	if (count)
	{
		ATLASSUME(data != nullptr);
		sMsg += L" Data:\r\n";
		for (ULONG i = 0; i<count; i++)
		{
#pragma warning(suppress: 6011)
			sMsg.AppendFormat(L"  Key:\"%s\", Value:\"%s\"\r\n", data[i].Key, data[i].Value);
		}
	}

	return S_OK;
}

namespace blink { namespace app {  namespace agent {

	NotificationAgent::NotificationAgent()
	: m_winRTInitializer(RO_INIT_MULTITHREADED)
	{
		//Get the executable path
		std::wstring sModuleName;
		HRESULT hr = ToastPP::CManager::GetExecutablePath(sModuleName);
		if (FAILED(hr))
		{
			CString sMsg;
			sMsg.Format(_T("Failed to get executable path, Error:0x%08X"), hr);
		}

		RegisterCOMServer(sModuleName.c_str());

		//Register for toast notifications
		hr = ToastPP::CManager::RegisterForNotificationSupport(L"Blink Desktop", sModuleName.c_str(), L"Blink.NotificationManager", __uuidof(CToastNotificationActivationCallback));
		if (FAILED(hr))
		{
			CString sMsg;
			sMsg.Format(_T("Failed to register for Toast Notifications, Error:0x%08X"), hr);
		}

		RegisterActivator();

		hr = m_ToastManager.Create(L"Blink.NotificationManager");
		
		if (SUCCEEDED(hr))
		{
			LPCTSTR toast = _T("<toast>\r\n\
										 <visual>\r\n\
										  <binding template=\"ToastGeneric\">\r\n\
										   <text>Hello World</text>\r\n\
										   <text>This is a simple toast message</text>\r\n\
										  </binding>\r\n\
										 </visual>\r\n\
										</toast>");

			HRESULT hr2 = m_Toast.Create(toast);
			if (SUCCEEDED(hr2))
			{
				//Show the toast
				hr2 = m_ToastManager.Show(m_Toast, this);
				if (SUCCEEDED(hr2))
				{
					hr2 = hr2;
				}
			}

			m_subscriber.subscribe([this](const blink::core::utils::patterns::Event&)
			{
				
			}, blink::core::events::UPGRADE_COMPLETED_EVENT);
		}
	}

	NotificationAgent::~NotificationAgent()
	{
		UnregisterActivator();
	}

	void NotificationAgent::OnToastActivated(_In_opt_ ABI::Windows::UI::Notifications::IToastNotification* pSender, _In_opt_ IInspectable* /*pArgs*/)
	{
		pSender = pSender;
	}

	void NotificationAgent::OnToastDismissed(_In_opt_ ABI::Windows::UI::Notifications::IToastNotification* pSender, _In_ ABI::Windows::UI::Notifications::ToastDismissalReason reason)
	{
		pSender = pSender;
	}

	void NotificationAgent::OnToastFailed(_In_opt_ ABI::Windows::UI::Notifications::IToastNotification* pSender, _In_ HRESULT errorCode)
	{
		pSender = pSender;
	}

	HRESULT NotificationAgent::RegisterCOMServer(_In_z_ PCWSTR pszExePath)
	{
		//In this case, just register this application to start
		return HRESULT_FROM_WIN32(::RegSetKeyValueW(HKEY_CURRENT_USER, L"SOFTWARE\\Classes\\CLSID\\{383803B6-AFDA-4220-BFC3-0DBF810106BF}\\LocalServer32", nullptr, REG_SZ, pszExePath, static_cast<DWORD>(wcslen(pszExePath) * sizeof(wchar_t))));
	}

	HRESULT NotificationAgent::UnRegisterCOMServer()
	{
		HRESULT hr = HRESULT_FROM_WIN32(::RegDeleteKey(HKEY_CURRENT_USER, _T("SOFTWARE\\Classes\\CLSID\\{383803B6-AFDA-4220-BFC3-0DBF810106BF}\\LocalServer32")));
		if (FAILED(hr))
			return hr;
		return HRESULT_FROM_WIN32(::RegDeleteKey(HKEY_CURRENT_USER, _T("SOFTWARE\\Classes\\CLSID\\{383803B6-AFDA-4220-BFC3-0DBF810106BF}")));
	}

	HRESULT NotificationAgent::RegisterActivator()
	{
		Microsoft::WRL::Module<Microsoft::WRL::OutOfProc>::Create([] {});
		Microsoft::WRL::Module<Microsoft::WRL::OutOfProc>::GetModule().IncrementObjectCount();
		return Microsoft::WRL::Module<Microsoft::WRL::OutOfProc>::GetModule().RegisterObjects();
	}

	void NotificationAgent::UnregisterActivator()
	{
		Microsoft::WRL::Module<Microsoft::WRL::OutOfProc>::GetModule().UnregisterObjects();
		Microsoft::WRL::Module<Microsoft::WRL::OutOfProc>::GetModule().DecrementObjectCount();
	}

}}}