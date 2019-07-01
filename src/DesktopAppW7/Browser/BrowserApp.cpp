#include "StdAfx.h"

#pragma warning(push)
#pragma warning(disable : 4100)
#pragma warning(disable : 4481)

#include "Browser/BrowserApp.h"

#include "Browser/BrowserClientHandler.h"

#include <cef/cef_client.h>
#pragma warning(pop)

namespace desktop { namespace ui{

	#define REQUIRE_UI_THREAD()   ASSERT(CefCurrentlyOn(TID_UI));
	#define REQUIRE_IO_THREAD()   ASSERT(CefCurrentlyOn(TID_IO));
	#define REQUIRE_FILE_THREAD() ASSERT(CefCurrentlyOn(TID_FILE));

	BrowserApp::~BrowserApp()
	{
		
	}

	void BrowserApp::OnContextInitialized()
	{
		REQUIRE_UI_THREAD()

		// SimpleHandler implements browser-level callbacks.
	}

	BOOL BrowserApp::CreateBrowser(CefRefPtr<BrowserClientHandler> client_handler, HWND hWnd, CRect rect, LPCTSTR pszURL)
	{
		// settings
		CefBrowserSettings settings;
		CefWindowInfo info;
	
		settings.web_security = STATE_DISABLED;

		// set browser as child
		info.SetAsChild( hWnd, rect );

		// create browser window
		return CefBrowserHost::CreateBrowser( info, client_handler, pszURL, settings, NULL );
	}

	void BrowserApp::initialize()
	{
		// setup settings
		CString szCEFCache;
		CString szPath;
		INT nLen = GetTempPath( 0, NULL ) + 1;
		GetTempPath( nLen, szPath.GetBuffer( nLen ));

		// save path
		szCEFCache.Format( _T("%scache\0\0"), szPath.GetString());

		CefSettings settings;

		settings.remote_debugging_port = 8088;
		
		settings.no_sandbox = TRUE;
		settings.multi_threaded_message_loop = TRUE;
		
		CefString(&settings.cache_path) = szCEFCache;

		CefEnableHighDPISupport();

		CefMainArgs main_args(NULL);
		CefInitialize(main_args, settings, this, NULL);
	}

	void BrowserApp::OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line)
	{
		std::map<CefString, CefString> test;
		command_line->GetSwitches(test);

		// Pass additional command-line flags to the browser process.
		if (process_type.empty())
		{
			//command_line->AppendSwitch("disable-gpu");
		}
	}
}}