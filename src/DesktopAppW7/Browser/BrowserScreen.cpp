#include "StdAfx.h"

#pragma warning(push)
#pragma warning(disable : 4100)
#pragma warning(disable : 4481)

#include "Browser/BrowserScreen.h"

#include "DesktopApp.h"
#include "Browser/BrowserClientHandler.h"
#include "Browser/Resources/InternalResourceManager.h"
#include "Browser/Resources/ExternalResourceManager.h"
#include "DesktopCore\System\Services\FileInfoService.h"
#include "Constants.h"
#include "Events.h"

#include <cef/cef_app.h>
#include <cef/cef_browser.h>
#include <cef/cef_frame.h>

#pragma warning(pop)

#include <HtmlHelp.h>

namespace desktop { namespace ui{

	BrowserScreen::BrowserScreen(const std::string& path, CWnd* pParent /*=NULL*/)
		: CDialog(BrowserScreen::IDD, pParent), m_path(path)
		, m_cefBrowser(NULL)
	{
		//{{AFX_DATA_INIT(BrowserScreen)
			// NOTE: the ClassWizard will add member initialization here
		//}}AFX_DATA_INIT

	}

	BrowserScreen::~BrowserScreen() = default;

	void BrowserScreen::DoDataExchange(CDataExchange* pDX)
	{
		CDialog::DoDataExchange(pDX);
		//{{AFX_DATA_MAP(BrowserScreen)
		//}}AFX_DATA_MAP
	}

	BEGIN_MESSAGE_MAP(BrowserScreen, CDialog)
		ON_WM_SIZE()
		ON_MESSAGE(wm::browser::CEF_PRE_KEYBOARD_HOOK, OnKeyBoardHook)
	END_MESSAGE_MAP()

	
	/////////////////////////////////////////////////////////////////////////////
	// CUserManager2 message handlers

	BOOL BrowserScreen::OnInitDialog() 
	{
		// Make the child dialog part of the parent tab list.
		ModifyStyle(0, WS_TABSTOP);
		ModifyStyleEx(0, WS_EX_CONTROLPARENT);

		CRect rect;
		GetClientRect(&rect);

		if (false)
		{
			std::unique_ptr<InternalResourceManager> resourceManager(new InternalResourceManager);

			m_cefHandler = new BrowserClientHandler(this, std::move(resourceManager));
		}
		else
		{
			std::unique_ptr<ExternalResourceManager> resourceManager(new ExternalResourceManager);
			m_cefHandler = new BrowserClientHandler(this, std::move(resourceManager));
		}

		std::string url = m_path;

		theApp.CreateBrowser(m_cefHandler, m_hWnd, rect, url.c_str() );

		ShowWindow(SW_SHOW);

		return TRUE;
	}

	void BrowserScreen::refresh()
	{
		navigate(m_path);
	}

	void BrowserScreen::navigate(std::string path) 
	{
		if(m_cefBrowser != NULL)
		{
			m_cefBrowser->GetMainFrame()->LoadURL(path);
		}
	}

	void BrowserScreen::executeJS(std::string js) 
	{
		if(m_cefBrowser != NULL)
		{
			m_cefBrowser->GetMainFrame()->ExecuteJavaScript(js, m_cefBrowser->GetMainFrame()->GetURL(), 0);
		}
	}

	void BrowserScreen::onBrowserCreated(CefRefPtr<CefBrowser> browser)
	{
		m_cefBrowser = browser;

		m_downloadBasePath = theApp.onBrowserCreated(m_cefBrowser);

		m_subscriber.subscribe([this](const desktop::core::utils::patterns::Event& rawEvt)
		{
			TCHAR szFileName[MAX_PATH + 1];
			GetModuleFileName(NULL, szFileName, MAX_PATH + 1);

			core::service::FileInfoService service;
			auto version = service.getProductVersion(std::string(szFileName));

			executeJS("setAppVersion(\"" + version + "\");");
		}, events::BROWSER_LOAD_END_EVENT);
	}

	void BrowserScreen::onBrowserClosed(CefRefPtr<CefBrowser> browser)
	{
		
	}

	void BrowserScreen::onTitleChange(const std::string& title)
	{
		
	}

	void BrowserScreen::setDownloadFolder(const std::string& downloadFolder)
	{
		m_downloadFolder = downloadFolder;
	}

	std::string BrowserScreen::getDownloadPath(const std::string& file_name) const
	{
		return m_downloadBasePath + m_downloadFolder + file_name;
	}

	LRESULT BrowserScreen::OnKeyBoardHook(WPARAM /*wParam*/, LPARAM lParam)
	{
		MSG* pMsg = (MSG*)lParam;

		switch (pMsg->message)
		{
		case WM_KEYDOWN:
			if (pMsg->wParam == VK_F5)
			{
				m_cefBrowser->Reload();
			}
			break;
		}

		return S_OK;
	}

	void BrowserScreen::OnSize(UINT, int cx, int cy)
	{
		if (m_cefBrowser.get())
		{
			::SetWindowPos(m_cefBrowser->GetHost()->GetWindowHandle(), NULL, 0, 0, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
			m_cefBrowser->GetHost()->WasResized();
		}
	}
}}