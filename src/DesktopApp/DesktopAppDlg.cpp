#include "stdafx.h"
#include "DesktopApp.h"
#include "DesktopAppDlg.h"
#include "Toast/Toast.h"
#include "Toast/ToastPayload.h"
#include "DesktopCore\System\Services\ApplicationDataService.h"

#include <boost\filesystem.hpp>
#include <boost\filesystem\operations.hpp>

DesktopAppDlg::DesktopAppDlg(_In_opt_ CWnd* pParent)
: CDialog(DesktopAppDlg::IDD, pParent)
{

}

void DesktopAppDlg::DoDataExchange(CDataExchange* pDX)
{
  //Let the base class do its thing
  __super::DoDataExchange(pDX);

 /* DDX_Control(pDX, IDC_PAYLOADS, m_wndPayloads);
  DDX_Control(pDX, IDC_PAYLOAD, m_wndPayload);
  DDX_Text(pDX, IDC_PAYLOAD, m_sPayload);
  DDX_Control(pDX, IDC_MESSAGES, m_wndStatusMessages);*/
}

BEGIN_MESSAGE_MAP(DesktopAppDlg, CDialog)
  ON_WM_PAINT()
  ON_WM_SIZE()
  ON_WM_QUERYDRAGICON()
  /*
  ON_EN_CHANGE(IDC_PAYLOAD, &DesktopAppDlg::OnChangePayload)
  ON_BN_CLICKED(IDC_CLEAR, &DesktopAppDlg::OnClickedClear)*/
END_MESSAGE_MAP()

BOOL DesktopAppDlg::OnInitDialog()
{
  //Let the base class do its thing
  __super::OnInitDialog();

  SetIcon(m_hIcon, TRUE);
  SetIcon(m_hIcon, FALSE);
  
  std::string url;

  desktop::core::service::ApplicationDataService applicationService;

  if (boost::filesystem::exists(applicationService.getViewerFolder() + "/index.html"))
  {
	  url = boost::filesystem::canonical(applicationService.getViewerFolder() + "/index.html").string();
  }
  else
  {
	  url = boost::filesystem::canonical("Html/loading/index.html").string();
  }

  m_browser_dlg = std::make_unique<desktop::ui::BrowserScreen>(url, this);
  m_browser_dlg->Create(desktop::ui::BrowserScreen::IDD, this);

  ShowWindow(SW_MAXIMIZE);

  return TRUE;
}

void DesktopAppDlg::OnPaint() 
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

HCURSOR DesktopAppDlg::OnQueryDragIcon()
{
  return static_cast<HCURSOR>(m_hIcon);
}

void DesktopAppDlg::ReportToastNotification(_In_z_ LPCTSTR pszDetails, _In_ BOOL bAppend)
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

HRESULT DesktopAppDlg::VerifyXML(_Inout_ CString& sError)
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

void DesktopAppDlg::OnChangePayload()
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

void DesktopAppDlg::OnClickedClear()
{
  m_wndStatusMessages.SetWindowText(_T(""));
}

void DesktopAppDlg::OnSize(UINT /*nType*/, int cx, int cy)
{
	if (m_browser_dlg->GetSafeHwnd() != NULL)
	{
		m_browser_dlg->MoveWindow(0, 0, cx, cy);
	}
}