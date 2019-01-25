#pragma once

#include "resource.h"

#pragma warning( push )
#pragma warning( disable: 4100 )
#pragma warning(disable : 4481)
#include "Browser/BrowserClientHandler.h"
#pragma warning( pop )

namespace bling { namespace ui{

	class BrowserApp;

	class BrowserScreen : public CDialog, public BrowserClientHandler::Delegate
	{
	// Construction
	public:
		BrowserScreen(const std::string& path, CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
		//{{AFX_DATA(Maintenance)
		enum { IDD = IDD_BROWSER };
		//}}AFX_DATA

		virtual int GetRealDialogID() const
		{
			return IDD;
		}

		CBrush m_whiteBrush;
		CBrush m_whiteOddBrush;
		CBrush m_darkBrush;

		void refresh();
		void navigate(std::string path);
		void executeJS(std::string js);

		void onBrowserCreated(CefRefPtr<CefBrowser> browser);
		void onBrowserClosed(CefRefPtr<CefBrowser> browser);
		virtual void onTitleChange(const std::string& title);

		afx_msg LRESULT OnKeyBoardHook(WPARAM wParam, LPARAM lParam);
		afx_msg void OnSize(UINT, int, int);
		DECLARE_MESSAGE_MAP()
	private:
	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CUserManager2)
		protected:
		virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
		//}}AFX_VIRTUAL

	// Implementation
	protected:

		// Generated message map functions
		//{{AFX_MSG(CUserManager2)
		virtual BOOL OnInitDialog();
		//}}AFX_MSG
	protected:
		CefRefPtr<BrowserClientHandler> m_cefHandler;
		CefRefPtr<CefBrowser>					  m_cefBrowser;
		std::string								  m_path;
	};
}}