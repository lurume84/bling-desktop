#include "stdafx.h"

#include "BlingApp.h"
#include "BlingAppDlg.h"

#include "Toast\ToastCommandLineInfo.h"
#include "Agents\NotificationAgent.h"
#include "BlingCore\BlingCore.h"
#include "BlingCore\Upgrade\Agents\UpgradeViewerAgent.h"

#include "Services\DownloadFileService.h"

#include <boost/optional.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(BlingApp, CWinApp)
END_MESSAGE_MAP()

BlingApp::BlingApp() : m_nExitCode(ERROR_SUCCESS)
{

}

BlingApp theApp;

BOOL BlingApp::InitInstance()
{
	m_core = std::make_unique<bling::core::BlingCore>();

	boost::optional<bool> doRegister;

	bling::ui::toast::CToastPPCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	if (cmdInfo.m_bRegister)
	{
		doRegister = true;
	}
	else if (cmdInfo.m_bUnRegister)
	{
		doRegister = false;
	}

	std::string reason;

	bling::ui::agent::NotificationAgent agent;
	if (!agent.initialize(doRegister, reason, m_nExitCode))
	{
		AfxMessageBox(reason.c_str(), MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

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
	if (exit_code >= 0)
	{
		// The sub-process has completed so return here.
		return exit_code;
	}

	m_cefApp = new bling::ui::BrowserApp();
	m_cefApp->initialize();

    //Bring up the main dialog
    BlingAppDlg mainDlg;
    m_pMainWnd = &mainDlg;
    mainDlg.DoModal();

	CefShutdown();

  return FALSE;
}

int BlingApp::ExitInstance()
{
  __super::ExitInstance();
  return m_nExitCode;
}

BOOL BlingApp::CreateBrowser(CefRefPtr<bling::ui::BrowserClientHandler> client_handler, HWND hWnd, CRect rect, LPCTSTR pszURL)
{
	return m_cefApp->CreateBrowser(client_handler, hWnd, rect, pszURL);
}

std::string BlingApp::onBrowserCreated(CefRefPtr<CefBrowser> browser)
{
	auto downloadService = std::make_unique<bling::ui::service::DownloadFileService>(browser);

	m_core->initialize(std::make_unique<bling::core::agent::UpgradeViewerAgent>("api.github.com", "Download/Versions/", "Html/viewer", 
																				std::move(downloadService)));

	return "Download/Versions/";
}