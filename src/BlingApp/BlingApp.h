#pragma once

#ifndef __AFXWIN_H__
  #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"
#include "Toast/Toast.h"
#include "Browser/BrowserApp.h"

class BlingApp : public CWinApp
{
public:
//Constructors
  BlingApp();

//Member variables
  int  m_nExitCode;

  CefRefPtr<bling::ui::BrowserApp> m_cefApp;

//Methods
  BOOL InitInstance() override;
  int ExitInstance() override;

  BOOL CreateBrowser(CefRefPtr<bling::ui::BrowserClientHandler> client_handler, HWND hWnd, CRect rect, LPCTSTR pszURL);

  DECLARE_MESSAGE_MAP()
};


extern BlingApp theApp;