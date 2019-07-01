#pragma once

#pragma warning(push)
#pragma warning(disable : 4100)
#pragma warning(disable : 4481)
#include <cef/cef_app.h>
#pragma warning(pop)

namespace desktop{ namespace ui{

	class BrowserClientHandler;

	class BrowserApp: public CefApp, public CefBrowserProcessHandler
	{
	public:
		~BrowserApp();

		void OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line) OVERRIDE;

		// CefApp methods:
		virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() OVERRIDE { return this; }

		// CefBrowserProcessHandler methods:
		virtual void OnContextInitialized() OVERRIDE;

		BOOL CreateBrowser(CefRefPtr<BrowserClientHandler> client_handler, HWND hWnd, CRect rect, LPCTSTR pszURL);

		void initialize();

	private:
		// Include the default reference counting implementation.
		IMPLEMENT_REFCOUNTING(BrowserApp);
	};
}}