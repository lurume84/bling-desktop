#include "stdafx.h"
#include "DesktopApp.h"
#include "DesktopAppDlg.h"
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