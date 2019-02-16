#pragma once

#ifndef __AFXWIN_H__
  #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"
#include "Browser/BrowserApp.h"

#include <memory>

namespace bling
{
	namespace core
	{
		class BlingCore;
	}
}

class BlingApp : public CWinApp
{
public:
//Constructors
  BlingApp();

//Member variables
  int  m_nExitCode;

  CefRefPtr<bling::ui::BrowserApp> m_cefApp;

  std::unique_ptr<bling::core::BlingCore> m_core;

//Methods
  BOOL InitInstance() override;
  int ExitInstance() override;

  BOOL CreateBrowser(CefRefPtr<bling::ui::BrowserClientHandler> client_handler, HWND hWnd, CRect rect, LPCTSTR pszURL);
  std::string onBrowserCreated(CefRefPtr<CefBrowser> browser);

  std::wstring m_toastAction;

  DECLARE_MESSAGE_MAP()
};


extern BlingApp theApp;