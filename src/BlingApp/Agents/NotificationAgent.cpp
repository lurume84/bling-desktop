#include "stdafx.h"

#include "NotificationAgent.h"

#include "BlingCore\Upgrade\Events.h"

#include "Toast\ToastEventHandler.h"
#include "Toast\ToastCommandLineInfo.h"
#include "Toast\ToastNotificationActivationCallback.h"

namespace bling { namespace ui {  namespace agent {

	NotificationAgent::ShowNotificationEvent::~ShowNotificationEvent() = default;

	NotificationAgent::NotificationAgent()
	{
		m_subscriber.subscribe([this](const bling::core::utils::patterns::Event& rawEvt)
		{
			const auto& evt = static_cast<const ShowNotificationEvent&>(rawEvt);

			m_ToastManager.Show(*evt.m_toast.get(), evt.m_handler.get());

		}, SHOW_NOTIFICATION_EVENT);
	}

	NotificationAgent::~NotificationAgent()
	{
		UnregisterActivator();
	}

	bool NotificationAgent::initialize(boost::optional<bool> doRegister, std::string& reason, int &ecode)
	{
		ecode = 0;

		HRESULT hr = m_ToastManager.Create(L"Bling.Desktop");

		//Initialize the Windows Runtime
		Microsoft::WRL::Wrappers::RoInitializeWrapper winRTInitializer(RO_INIT_MULTITHREADED);
		{
			hr = winRTInitializer;
			if (FAILED(hr))
			{
				CString sMsg;
				sMsg.Format(_T("Failed to initialize Windows Runtime, Error:0x%08X"), hr);
				reason = sMsg;
				return false;
			}

			//Get the executable path
			std::wstring sModuleName;
			hr = ToastPP::CManager::GetExecutablePath(sModuleName);
			if (FAILED(hr))
			{
				CString sMsg;
				sMsg.Format(_T("Failed to get executable path, Error:0x%08X"), hr);
				reason = sMsg;
				return false;
			}

			if(doRegister)
			{
				if (*doRegister)
				{
					ecode = RegisterCOMServer(sModuleName.c_str());
					if (FAILED(ecode))
					{
						CString sMsg;
						sMsg.Format(_T("Failed to register COM server, Error:0x%08X"), hr);
						reason = sMsg;
						return false;
					}
				}
				else
				{
					ecode = UnRegisterCOMServer();
					if (FAILED(ecode))
					{
						CString sMsg;
						sMsg.Format(_T("Failed to unregister COM server, Error:0x%08X"), ecode);
						reason = sMsg;
						return false;
					}
				}
			}

			hr = ToastPP::CManager::RegisterForNotificationSupport(L"Bling Desktop", sModuleName.c_str(), L"Bling.Desktop", __uuidof(ui::toast::ToastNotificationActivationCallback));
			if (FAILED(hr))
			{
				CString sMsg;
				sMsg.Format(_T("Failed to register for Toast Notifications, Error:0x%08X"), hr);
				reason = sMsg;
				return false;
			}

			ecode = RegisterActivator();
			if (FAILED(ecode))
			{
				CString sMsg;
				sMsg.Format(_T("Failed to register Activator, Error:0x%08X"), ecode);
				reason = sMsg;
				return false;
			}
		}

		return true;
	}

	HRESULT NotificationAgent::RegisterCOMServer(PCWSTR pszExePath)
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