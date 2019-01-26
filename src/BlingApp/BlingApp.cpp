#include "stdafx.h"

#include "BlingApp.h"
#include "BlingAppDlg.h"

#include "Agents\NotificationAgent.h"
#include "BlingCore/BlingCore.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(BlingApp, CWinApp)
END_MESSAGE_MAP()

class CToastPPCommandLineInfo : public CCommandLineInfo
{
public:
  //Constructors / Destructors
  CToastPPCommandLineInfo() : m_bRegister(FALSE),
                              m_bUnRegister(FALSE)
  {

  }

  //Methods
  void ParseParam(const TCHAR* lpszParam, BOOL bFlag, BOOL bLast) override
  {
    if (bFlag)
    {
      CString sParamUpper(lpszParam);
      sParamUpper.MakeUpper();
      if (sParamUpper == _T("REGSERVER"))
      {
        m_bRegister = TRUE;
        m_bUnRegister = FALSE;
      }
      else if (sParamUpper == _T("UNREGSERVER"))
      {
        m_bUnRegister = TRUE;
        m_bRegister = FALSE;
      }
      else
        __super::ParseParam(lpszParam, bFlag, bLast);
    }
    else
      __super::ParseParam(lpszParam, bFlag, bLast);
  };

  //Member variables
  BOOL m_bRegister;
  BOOL m_bUnRegister;
};

BlingApp::BlingApp() : m_nExitCode(ERROR_SUCCESS)
{

}

BlingApp theApp;

BOOL BlingApp::InitInstance()
{
	// Enable High-DPI support on Windows 7 or newer.
	CefEnableHighDPISupport();

	void* sandbox_info = NULL;

#if defined(CEF_USE_SANDBOX)
	// Manage the life span of the sandbox information object. This is necessary
	// for sandbox support on Windows. See cef_sandbox_win.h for complete details.
	CefScopedSandboxInfo scoped_sandbox;
	sandbox_info = scoped_sandbox.sandbox_info();
#endif

	// Provide CEF with command-line arguments.
	CefMainArgs main_args(GetModuleHandle(NULL));

	// CEF applications have multiple sub-processes (render, plugin, GPU, etc)
	// that share the same executable. This function checks the command-line and,
	// if this is a sub-process, executes the appropriate logic.
	int exit_code = CefExecuteProcess(main_args, NULL, sandbox_info);
	if (exit_code >= 0) {
		// The sub-process has completed so return here.
		return exit_code;
	}

  //Initialize the Windows Runtime
  Microsoft::WRL::Wrappers::RoInitializeWrapper winRTInitializer(RO_INIT_MULTITHREADED);
  {
    HRESULT hr = winRTInitializer;
    if (FAILED(hr))
    {
      CString sMsg;
      sMsg.Format(_T("Failed to initialize Windows Runtime, Error:0x%08X"), hr);
      AfxMessageBox(sMsg, MB_OK | MB_ICONEXCLAMATION);
      return FALSE;
    }

    //Get the executable path
    std::wstring sModuleName;
    hr = ToastPP::CManager::GetExecutablePath(sModuleName);
    if (FAILED(hr))
    {
      CString sMsg;
      sMsg.Format(_T("Failed to get executable path, Error:0x%08X"), hr);
      AfxMessageBox(sMsg, MB_OK | MB_ICONEXCLAMATION);
      return FALSE;
    }

    //Parse the command line
    CToastPPCommandLineInfo cmdInfo;
    ParseCommandLine(cmdInfo);
    if (cmdInfo.m_bRegister)
    {
      m_nExitCode = RegisterCOMServer(sModuleName.c_str());
      return FALSE;
    }
    else if (cmdInfo.m_bUnRegister)
    {
      m_nExitCode = UnRegisterCOMServer();
      return FALSE;
    }

    //Register for toast notifications
    hr = ToastPP::CManager::RegisterForNotificationSupport(L"Bling Desktop", sModuleName.c_str(), L"Bling.Desktop", __uuidof(CToastNotificationActivationCallback));
    if (FAILED(hr))
    {
      CString sMsg;
      sMsg.Format(_T("Failed to register for Toast Notifications, Error:0x%08X"), hr);
      AfxMessageBox(sMsg, MB_OK | MB_ICONEXCLAMATION);
      return FALSE;
    }

    //Register the class factories
    m_nExitCode = RegisterActivator();
    if (FAILED(hr))
      return FALSE;

	auto core = std::make_unique<bling::core::BlingCore>();
	core->initialize();

	bling::ui::agent::NotificationAgent notification;

	m_cefApp = new bling::ui::BrowserApp();
	m_cefApp->initialize();

    //Bring up the main dialog
    BlingAppDlg mainDlg;
    m_pMainWnd = &mainDlg;
    mainDlg.DoModal();

    UnregisterActivator();

	CefShutdown();
  }

  return FALSE;
}

int BlingApp::ExitInstance()
{
  __super::ExitInstance();
  return m_nExitCode;
}

HRESULT BlingApp::RegisterCOMServer(_In_z_ PCWSTR pszExePath)
{
  //In this case, just register this application to start
  return HRESULT_FROM_WIN32(::RegSetKeyValueW(HKEY_CURRENT_USER, L"SOFTWARE\\Classes\\CLSID\\{383803B6-AFDA-4220-BFC3-0DBF810106BF}\\LocalServer32", nullptr, REG_SZ, pszExePath, static_cast<DWORD>(wcslen(pszExePath)*sizeof(wchar_t))));
}

HRESULT BlingApp::UnRegisterCOMServer()
{
  HRESULT hr = HRESULT_FROM_WIN32(::RegDeleteKey(HKEY_CURRENT_USER, _T("SOFTWARE\\Classes\\CLSID\\{383803B6-AFDA-4220-BFC3-0DBF810106BF}\\LocalServer32")));
  if (FAILED(hr))
    return hr;
  return HRESULT_FROM_WIN32(::RegDeleteKey(HKEY_CURRENT_USER, _T("SOFTWARE\\Classes\\CLSID\\{383803B6-AFDA-4220-BFC3-0DBF810106BF}")));
}

HRESULT BlingApp::RegisterActivator()
{
  Microsoft::WRL::Module<Microsoft::WRL::OutOfProc>::Create([] {});
  Microsoft::WRL::Module<Microsoft::WRL::OutOfProc>::GetModule().IncrementObjectCount();
  return Microsoft::WRL::Module<Microsoft::WRL::OutOfProc>::GetModule().RegisterObjects();
}

void BlingApp::UnregisterActivator()
{
  Microsoft::WRL::Module<Microsoft::WRL::OutOfProc>::GetModule().UnregisterObjects();
  Microsoft::WRL::Module<Microsoft::WRL::OutOfProc>::GetModule().DecrementObjectCount();
}

HRESULT CToastNotificationActivationCallback::Activate(__RPC__in_string LPCWSTR appUserModelId, __RPC__in_opt_string LPCWSTR invokedArgs,
                                                       __RPC__in_ecount_full_opt(count) const NOTIFICATION_USER_INPUT_DATA* data, ULONG count)
{
	CStringW sMsg;
	sMsg.Format(L"INotificationActivationCallback, The toast was activated. appUserModelId:\"%s\", Arguments:\"%s\"\r\n", appUserModelId, invokedArgs);
	
	if (count)
	{
		ATLASSUME(data != nullptr);
		sMsg += L" Data:\r\n";

		for (ULONG i=0; i<count; i++)
		{
			#pragma warning(suppress: 6011)
			sMsg.AppendFormat(L"  Key:\"%s\", Value:\"%s\"\r\n", data[i].Key, data[i].Value);
		}
	}

	BlingAppDlg* pDlg = static_cast<BlingAppDlg*>(theApp.m_pMainWnd);
	if (pDlg == nullptr)
	{
		return S_OK;
	}

	pDlg->ReportToastNotification(CString(sMsg), TRUE);

	return S_OK;
}

BOOL BlingApp::CreateBrowser(CefRefPtr<bling::ui::BrowserClientHandler> client_handler, HWND hWnd, CRect rect, LPCTSTR pszURL)
{
	return m_cefApp->CreateBrowser(client_handler, hWnd, rect, pszURL);
}