#pragma once

#ifndef __AFXWIN_H__
  #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"
#include "Browser/BrowserApp.h"

#include <memory>

namespace desktop
{
	namespace core
	{
		class DesktopCore;
	}
}

class DesktopApp : public CWinApp
{
public:
//Constructors
  DesktopApp();

//Member variables
  int  m_nExitCode;

  CefRefPtr<desktop::ui::BrowserApp> m_cefApp;

  std::unique_ptr<desktop::core::DesktopCore> m_core;

//Methods
  BOOL InitInstance() override;
  int ExitInstance() override;

  BOOL CreateBrowser(CefRefPtr<desktop::ui::BrowserClientHandler> client_handler, HWND hWnd, CRect rect, LPCTSTR pszURL);
  std::string onBrowserCreated(CefRefPtr<CefBrowser> browser);

  DECLARE_MESSAGE_MAP()
};


extern DesktopApp theApp;