#include "stdafx.h"
#include "BlingApp.h"
#include "BlingAppDlg.h"
#include "Toast/Toast.h"
#include "Toast/ToastPayload.h"

BlingAppDlg::BlingAppDlg(_In_opt_ CWnd* pParent)
: CDialog(BlingAppDlg::IDD, pParent)
, m_browser_dlg("http://www.google.es", this)
{

}

void BlingAppDlg::DoDataExchange(CDataExchange* pDX)
{
  //Let the base class do its thing
  __super::DoDataExchange(pDX);

 /* DDX_Control(pDX, IDC_PAYLOADS, m_wndPayloads);
  DDX_Control(pDX, IDC_PAYLOAD, m_wndPayload);
  DDX_Text(pDX, IDC_PAYLOAD, m_sPayload);
  DDX_Control(pDX, IDC_MESSAGES, m_wndStatusMessages);*/
}

BEGIN_MESSAGE_MAP(BlingAppDlg, CDialog)
  ON_WM_PAINT()
  ON_WM_SIZE()
  ON_WM_QUERYDRAGICON()
  /*ON_BN_CLICKED(IDC_POP_TOAST, &BlingAppDlg::OnPopToast)
  ON_CBN_SELCHANGE(IDC_PAYLOADS, &BlingAppDlg::OnSelchangePayloads)
  ON_EN_CHANGE(IDC_PAYLOAD, &BlingAppDlg::OnChangePayload)
  ON_BN_CLICKED(IDC_CLEAR, &BlingAppDlg::OnClickedClear)*/
END_MESSAGE_MAP()

BOOL BlingAppDlg::OnInitDialog()
{
  //Let the base class do its thing
  __super::OnInitDialog();

  SetIcon(m_hIcon, TRUE);
  SetIcon(m_hIcon, FALSE);

  //Populate the combo box and edit boxes
  /*for (int i=0; i<_countof(g_ToastPayloads); i++)
  {
    m_wndPayloads.AddString(g_ToastPayloads[i].pszName);
    if (i == 0)
    {
      CString sXML(g_ToastPayloads[i].pszXML);
      m_wndPayload.SetWindowText(sXML);
      m_wndPayloads.SetCurSel(i);
    }
  }*/

  //Create the toast manager
  HRESULT hr = m_ToastManager.Create(L"Bling.Desktop");
  if (FAILED(hr))
  {
    CString sMsg;
    sMsg.Format(_T("Failed to create Toast manager, Error:0x%08X"), hr);
    AfxMessageBox(sMsg, MB_OK | MB_ICONEXCLAMATION);
    EndDialog(IDCANCEL);
  }
  
  m_browser_dlg.Create(bling::ui::BrowserScreen::IDD, this);

  ShowWindow(SW_MAXIMIZE);

  return TRUE;
}

void BlingAppDlg::OnPaint() 
{
  if (IsIconic())
  {
    CPaintDC dc(this);

    SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

    //Center icon in client rectangle
    int cxIcon = GetSystemMetrics(SM_CXICON);
    int cyIcon = GetSystemMetrics(SM_CYICON);
    CRect rect;
    GetClientRect(&rect);
    int x = (rect.Width() - cxIcon + 1) / 2;
    int y = (rect.Height() - cyIcon + 1) / 2;

    //Draw the icon
    dc.DrawIcon(x, y, m_hIcon);
  }
  else
    __super::OnPaint();
}

HCURSOR BlingAppDlg::OnQueryDragIcon()
{
  return static_cast<HCURSOR>(m_hIcon);
}

void BlingAppDlg::OnPopToast() 
{
  //Get the values from the UI
  if (!UpdateData(TRUE))
    return;

  //Create the toast
  CStringW sPayload(FixUpImagesInXML(CStringW(m_sPayload)));
  HRESULT hr = m_Toast.Create(sPayload);
  if (FAILED(hr))
  {
    CString sMsg;
    sMsg.Format(_T("Failed to create the toast, Error:%08X\r\n"), hr);
    ReportToastNotification(sMsg, TRUE);
    return;
  }

  //Show the toast
  hr = m_ToastManager.Show(m_Toast, this);
  if (SUCCEEDED(hr))
  {
    ReportToastNotification(_T("Toast shown successfully\r\n"), TRUE);
  }
  else
  {
    CString sMsg;
    sMsg.Format(_T("Failed to show the toast, Error:%08X\r\n"), hr);
    ReportToastNotification(sMsg, TRUE);
  }
}

void BlingAppDlg::ReportToastNotification(_In_z_ LPCTSTR pszDetails, _In_ BOOL bAppend)
{
  if (bAppend)
  {
    int nLength = m_wndStatusMessages.GetWindowTextLength();
    m_wndStatusMessages.SetSel(nLength, nLength);
    m_wndStatusMessages.ReplaceSel(pszDetails);
  }
  else
    m_wndStatusMessages.SetWindowText(pszDetails);
}

void BlingAppDlg::OnSelchangePayloads()
{
  int nCurSel = m_wndPayloads.GetCurSel();
  if (nCurSel == CB_ERR)
    return;
  CString sXML(g_ToastPayloads[nCurSel].pszXML);
  m_wndPayload.SetWindowText(sXML);
}

CStringW BlingAppDlg::FixUpImagesInXML(_In_ const CStringW& sXML)
{
  CStringW sUpdatedXML(sXML);
  std::wstring sModuleName;
  if (FAILED(ToastPP::CManager::GetExecutablePath(sModuleName)))
    return sUpdatedXML;
  ATL::CPathW sPath(sModuleName.c_str());
  if (!sPath.RemoveFileSpec())
    return sUpdatedXML;
  sUpdatedXML.Replace(L"%EXEPATH%", sPath);
  return sUpdatedXML;
}

void BlingAppDlg::OnToastActivated(_In_opt_ ABI::Windows::UI::Notifications::IToastNotification* /*pSender*/, _In_opt_ IInspectable* /*pArgs*/)
{
  CString sMsg(_T("IToastNotification, The user clicked on the toast\r\n"));
  ReportToastNotification(sMsg, TRUE);
}

void BlingAppDlg::OnToastDismissed(_In_opt_ ABI::Windows::UI::Notifications::IToastNotification* /*pSender*/, _In_ ABI::Windows::UI::Notifications::ToastDismissalReason reason)
{
  CString sMsg;
  switch (reason)
  {
    case ABI::Windows::UI::Notifications::ToastDismissalReason_UserCanceled:
    {
      sMsg = _T("IToastNotification, The user dismissed the toast\r\n");
      break;
    }
    case ABI::Windows::UI::Notifications::ToastDismissalReason_ApplicationHidden:
    {
      sMsg = _T("IToastNotification, The application programatically hid the toast\r\n");
      break;
    }
    case ABI::Windows::UI::Notifications::ToastDismissalReason_TimedOut:
    {
      sMsg = _T("IToastNotification, The toast has timed out\r\n");
      break;
    }
    default:
    {
      sMsg.Format(_T("IToastNotification, The toast was dimissed for an unknown reason %d\r\n"), reason);
      break;
    }
  }
  ReportToastNotification(sMsg, TRUE);
}

void BlingAppDlg::OnToastFailed(_In_opt_ ABI::Windows::UI::Notifications::IToastNotification* /*pSender*/, _In_ HRESULT errorCode)
{
  CString sMsg;
  sMsg.Format(_T("IToastNotification, An error occurred with the toast. Error:%08X\r\n"), errorCode);
  ReportToastNotification(sMsg, TRUE);
}

HRESULT BlingAppDlg::VerifyXML(_Inout_ CString& sError)
{
  ATL::CComPtr<IXMLDOMDocument> xml;
  HRESULT hr = xml.CoCreateInstance(__uuidof(DOMDocument60));
  if (FAILED(hr))
    return hr;
  ATL::CComBSTR bstrXML(m_sPayload);
  VARIANT_BOOL bSuccessfull = VARIANT_FALSE;
  hr = xml->loadXML(bstrXML, &bSuccessfull);
  if (FAILED(hr))
    return hr;

  if (bSuccessfull == VARIANT_FALSE)
  {
    ATL::CComPtr<IXMLDOMParseError> parseError;
    hr = xml->get_parseError(&parseError);
    if (FAILED(hr))
      return hr;
    ATL::CComBSTR bstrReason;
    hr = parseError->get_reason(&bstrReason);
    if (FAILED(hr))
      return hr;
    long nErrorCode = 0;
    hr = parseError->get_errorCode(&nErrorCode);
    if (FAILED(hr))
      return hr;
    long nLine = 0;
    hr = parseError->get_line(&nLine);
    if (FAILED(hr))
      return hr;
    long nLinePos = 0;
    hr = parseError->get_linepos(&nLinePos);
    if (FAILED(hr))
      return hr;
    long nFilePos = 0;
    hr = parseError->get_filepos(&nFilePos);
    if (FAILED(hr))
      return hr;
    sError.Format(_T("Error Code: %X, Line: %d, Line position: %d, File Position: %d, Reason: %s"), nErrorCode, nLine, nLinePos, nFilePos, ATL::CW2T(bstrReason.operator wchar_t*()).operator LPTSTR());
    return E_FAIL;
  }

  return S_OK;
}

void BlingAppDlg::OnChangePayload()
{
  if (!UpdateData(TRUE))
    return;

  CString sError;
  HRESULT hr = VerifyXML(sError);
  if (FAILED(hr))
  {
    CString sMsg;
    sMsg.Format(_T("Failed to verify the XML. Error:%08X, %s\r\n"), hr, sError.GetString());
    ReportToastNotification(sMsg, FALSE);
  }
  else
    ReportToastNotification(_T("Toast content is valid XML\r\n"), FALSE);
}

void BlingAppDlg::OnClickedClear()
{
  m_wndStatusMessages.SetWindowText(_T(""));
}

void BlingAppDlg::OnSize(UINT /*nType*/, int cx, int cy)
{
	if (m_browser_dlg.GetSafeHwnd() != NULL)
	{
		m_browser_dlg.MoveWindow(0, 0, cx, cy);
	}
}