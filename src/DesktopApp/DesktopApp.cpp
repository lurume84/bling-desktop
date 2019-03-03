#include "stdafx.h"

#include "DesktopApp.h"
#include "DesktopAppDlg.h"

#include "Toast\ToastCommandLineInfo.h"
#include "Agents\NotificationAgent.h"
#include "DesktopCore\DesktopCore.h"
#include "DesktopCore\Upgrade\Agents\UpgradeViewerAgent.h"
#include "DesktopCore\Blink\Agents\SyncVideoAgent.h"
#include "DesktopCore\System\Services\ApplicationDataService.h"

#include "Services\DownloadFileService.h"

#include <boost/optional.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(DesktopApp, CWinApp)
END_MESSAGE_MAP()

DesktopApp::DesktopApp() : m_nExitCode(ERROR_SUCCESS)
{

}

DesktopApp theApp;

BOOL DesktopApp::InitInstance()
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
	if (exit_code >= 0)
	{
		// The sub-process has completed so return here.
		return exit_code;
	}

	m_core = std::make_unique<desktop::core::DesktopCore>();

	boost::optional<bool> doRegister;

	desktop::ui::toast::CToastPPCommandLineInfo cmdInfo;
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

	desktop::ui::agent::NotificationAgent agent(L"Bling");
	if (!agent.initialize(doRegister, reason, m_nExitCode))
	{
		std::string reason2 = reason + ". Check if Windows Notification Platform is available. Application will continue but notifications will not work.";
		AfxMessageBox(reason2.c_str(), MB_OK | MB_ICONEXCLAMATION);
	}

	m_cefApp = new desktop::ui::BrowserApp();
	m_cefApp->initialize();

    //Bring up the main dialog
    DesktopAppDlg mainDlg;
    m_pMainWnd = &mainDlg;
    mainDlg.DoModal();

	CefShutdown();

  return FALSE;
}

int DesktopApp::ExitInstance()
{
  __super::ExitInstance();
  return m_nExitCode;
}

BOOL DesktopApp::CreateBrowser(CefRefPtr<desktop::ui::BrowserClientHandler> client_handler, HWND hWnd, CRect rect, LPCTSTR pszURL)
{
	return m_cefApp->CreateBrowser(client_handler, hWnd, rect, pszURL);
}

std::string DesktopApp::onBrowserCreated(CefRefPtr<CefBrowser> browser)
{
	desktop::core::service::ApplicationDataService service;

	auto documents = service.getMyDocuments();
	
	boost::filesystem::create_directories(documents + "Download\\Versions");
	boost::filesystem::create_directories(documents + "Download\\Videos");

	auto updateAgent = std::make_unique<desktop::core::agent::UpgradeViewerAgent>("api.github.com", "/repos/lurume84/bling-viewer/releases/latest",
																				documents + "Download\\Versions\\", "Html/viewer",
																				std::make_unique<desktop::ui::service::DownloadFileService>(browser));

	auto syncVideoAgent = std::make_unique<desktop::core::agent::SyncVideoAgent>(documents + "Download\\Videos\\");

	m_core->initialize(std::move(updateAgent), std::move(syncVideoAgent));
	
	return documents + "Download\\Versions\\";
}