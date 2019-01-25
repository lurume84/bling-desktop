#pragma once

#ifndef __AFXWIN_H__
  #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"
#include <wrl.h>
#include "Toast/Toast.h"
#include "Browser/BrowserApp.h"


//Define INotificationActivationCallback for older versions of the Windows SDK
#include <ntverp.h>
#if VER_PRODUCTBUILD < 10011

typedef struct NOTIFICATION_USER_INPUT_DATA
{
  LPCWSTR Key;
  LPCWSTR Value;
}  NOTIFICATION_USER_INPUT_DATA;

MIDL_INTERFACE("53E31837-6600-4A81-9395-75CFFE746F94")
INotificationActivationCallback : public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE Activate(__RPC__in_string LPCWSTR appUserModelId, __RPC__in_opt_string LPCWSTR invokedArgs,
                                               __RPC__in_ecount_full_opt(count) const NOTIFICATION_USER_INPUT_DATA *data, ULONG count) = 0;
};

#else
#include <NotificationActivationCallback.h>
#endif //#if VER_PRODUCTBUILD < 10011



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
  HRESULT RegisterCOMServer(_In_z_ PCWSTR pszExePath);
  HRESULT UnRegisterCOMServer();
  HRESULT RegisterActivator();
  void UnregisterActivator();

  BOOL CreateBrowser(CefRefPtr<bling::ui::BrowserClientHandler> client_handler, HWND hWnd, CRect rect, LPCTSTR pszURL);

  DECLARE_MESSAGE_MAP()
};


extern BlingApp theApp;


//The COM server which implements the callback notifcation from Action Center
class DECLSPEC_UUID("383803B6-AFDA-4220-BFC3-0DBF810106BF")
  CToastNotificationActivationCallback : public Microsoft::WRL::RuntimeClass<Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::ClassicCom>, INotificationActivationCallback>
{
public:
//Constructors / Destructors
  CToastNotificationActivationCallback()
  {
  }

  virtual HRESULT STDMETHODCALLTYPE Activate(__RPC__in_string LPCWSTR appUserModelId, __RPC__in_opt_string LPCWSTR invokedArgs,
                                             __RPC__in_ecount_full_opt(count) const NOTIFICATION_USER_INPUT_DATA* data, ULONG count) override;
};
CoCreatableClass(CToastNotificationActivationCallback);
