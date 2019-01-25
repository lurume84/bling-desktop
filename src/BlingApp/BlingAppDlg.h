#pragma once

#include "Browser\BrowserScreen.h"

class BlingAppDlg : public CDialog, public ToastPP::INotifier
{
public:
//Constructors / Destructors
  BlingAppDlg(_In_opt_ CWnd* pParent = nullptr);

//Member variables
  enum { IDD = IDD_TOASTPP_DIALOG };

//Methods
  void ReportToastNotification(_In_z_ LPCTSTR pszDetails, _In_ BOOL bAppend);

protected:
//Member variables
  HICON m_hIcon;
  CComboBox m_wndPayloads;
  CEdit m_wndPayload;
  CString m_sPayload;
  CEdit m_wndStatusMessages;
  ToastPP::CManager m_ToastManager;
  ToastPP::CToast m_Toast;

  bling::ui::BrowserScreen m_browser_dlg;

//Methods
  static CStringW FixUpImagesInXML(_In_ const CStringW& sXML);
  void DoDataExchange(CDataExchange* pDX) override;
  BOOL OnInitDialog() override;
  void OnToastActivated(_In_opt_ ABI::Windows::UI::Notifications::IToastNotification* pSender, _In_opt_ IInspectable* pArgs) override;
  void OnToastDismissed(_In_opt_ ABI::Windows::UI::Notifications::IToastNotification* pSender, _In_ ABI::Windows::UI::Notifications::ToastDismissalReason reason) override;
  void OnToastFailed(_In_opt_ ABI::Windows::UI::Notifications::IToastNotification* pSender, _In_ HRESULT errorCode) override;
  HRESULT VerifyXML(_Inout_ CString& sError);

//Message handlers
  afx_msg void OnPaint();
  afx_msg HCURSOR OnQueryDragIcon();
  afx_msg void OnPopToast();
  afx_msg void OnSelchangePayloads();
  afx_msg void OnChangePayload();
  afx_msg void OnClickedClear();
  afx_msg void OnSize(UINT, int, int);

  DECLARE_MESSAGE_MAP()
};
