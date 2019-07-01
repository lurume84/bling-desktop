#pragma once

#include "Browser\BrowserScreen.h"

class DesktopAppDlg : public CDialog
{
public:
//Constructors / Destructors
  DesktopAppDlg(_In_opt_ CWnd* pParent = nullptr);

//Member variables
  enum { IDD = IDD_TOASTPP_DIALOG };

protected:
//Member variables
  HICON m_hIcon;
  CComboBox m_wndPayloads;
  CEdit m_wndPayload;
  CString m_sPayload;
  CEdit m_wndStatusMessages;

  std::unique_ptr<desktop::ui::BrowserScreen> m_browser_dlg;

//Methods
  void DoDataExchange(CDataExchange* pDX) override;
  BOOL OnInitDialog() override;
  
  HRESULT VerifyXML(_Inout_ CString& sError);

//Message handlers
  afx_msg void OnPaint();
  afx_msg HCURSOR OnQueryDragIcon();
  afx_msg void OnClickedClear();
  afx_msg void OnSize(UINT, int, int);

  DECLARE_MESSAGE_MAP()
};
