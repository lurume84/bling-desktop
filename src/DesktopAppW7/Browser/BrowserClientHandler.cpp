#include "stdafx.h"

#include "Events.h"
#include "DesktopCore\Utils\Patterns\PublisherSubscriber\Broker.h"

#include <stdio.h>
#include <algorithm>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#pragma warning(push)
#pragma warning(disable : 4100)
#pragma warning(disable : 4481)
#include "Browser/BrowserClientHandler.h"
#include "Browser/BrowserKeyboardHandler.h"
#include "Browser/Resources/IResourceManager.h"

#include <cef/cef_browser.h>
#include <cef/cef_frame.h>
#include <cef/cef_path_util.h>
#include <cef/cef_process_util.h>
#include <cef/cef_trace.h>
#pragma warning(pop)

#include <boost/filesystem.hpp>

namespace desktop { namespace ui{

	#define REQUIRE_UI_THREAD()   ASSERT(CefCurrentlyOn(TID_UI));
	#define REQUIRE_IO_THREAD()   ASSERT(CefCurrentlyOn(TID_IO));
	#define REQUIRE_FILE_THREAD() ASSERT(CefCurrentlyOn(TID_FILE));

	#define CEF_BIT_IS_LOADING			0x001
	#define CEF_BIT_CAN_GO_BACK			0x002
	#define CEF_BIT_CAN_GO_FORWARD		0x004

	struct CEFAuthenticationValues
	{
		LPCTSTR lpszHost;
		LPCTSTR lpszRealm;
		TCHAR szUserName[1024];
		TCHAR szUserPass[1024];
	};

	const char kMultiThreadedMessageLoop[] = "multi-threaded-message-loop";
	const char kCachePath[] = "cache-path";
	const char kUrl[] = "url";
	const char kExternalDevTools[] = "external-devtools";
	const char kOffScreenRenderingEnabled[] = "off-screen-rendering-enabled";
	const char kTransparentPaintingEnabled[] = "transparent-painting-enabled";
	const char kMouseCursorChangeDisabled[] = "mouse-cursor-change-disabled";

	BrowserClientHandler::BrowserClientHandler(Delegate* delegate, std::unique_ptr<IResourceManager> resourceManager)
		: m_delegate(delegate)
		, m_keyboardHandler(new BrowserKeyboardHandler)
		, m_resourceManager(std::move(resourceManager))
	{
		
	}

	BrowserClientHandler::~BrowserClientHandler()
	{
	}

	bool BrowserClientHandler::DoClose(CefRefPtr<CefBrowser> browser)
	{
		// get browser ID
		INT nBrowserId = browser->GetIdentifier();
		// The frame window will be the parent of the browser window
		HWND hWindow = GetParent( browser->GetHost()->GetWindowHandle() );

		// call parent
		return CefLifeSpanHandler::DoClose(browser);
	}

	void BrowserClientHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser)
	{
		REQUIRE_UI_THREAD();

		// get browser ID
		INT nBrowserId = browser->GetIdentifier();
		// The frame window will be the parent of the browser window
		HWND hWindow = GetParent( browser->GetHost()->GetWindowHandle() );

		// assign new browser
		//CefBrowser* pBrowser = browser;
		//::SendMessage( hWindow, WM_APP_CEF_NEW_BROWSER, (WPARAM)nBrowserId, (LPARAM)pBrowser );

		m_delegate->onBrowserCreated(browser);

		// call parent
		CefLifeSpanHandler::OnAfterCreated(browser);
	}

	void BrowserClientHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser)
	{
		REQUIRE_UI_THREAD();

		//// get browser ID
		//INT nBrowserId = browser->GetIdentifier();
		//// The frame window will be the parent of the browser window
		//HWND hWindow = GetParent( browser->GetHost()->GetWindowHandle() );

		//// close browser
		//::SendMessage( hWindow, WM_APP_CEF_CLOSE_BROWSER, (WPARAM)nBrowserId, (LPARAM)NULL );

		// call parent
		CefLifeSpanHandler::OnBeforeClose(browser);
	}

	void BrowserClientHandler::OnBeforeContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model)
	{
		REQUIRE_UI_THREAD();

		/*if((params->GetTypeFlags() & (CM_TYPEFLAG_PAGE | CM_TYPEFLAG_FRAME)) != 0)
		{
		}*/

		model->Clear();

		// call parent
		CefContextMenuHandler::OnBeforeContextMenu(browser, frame, params, model);
	}

	bool BrowserClientHandler::OnContextMenuCommand(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, int command_id, EventFlags event_flags)
	{
		// The frame window will be the parent of the browser window
		HWND hWindow = GetParent( browser->GetHost()->GetWindowHandle() );

		// call parent
		return CefContextMenuHandler::OnContextMenuCommand(browser, frame, params, command_id, event_flags);
	}

	void BrowserClientHandler::OnLoadingStateChange(CefRefPtr<CefBrowser> browser, bool isLoading, bool canGoBack, bool canGoForward)
	{
		REQUIRE_UI_THREAD();

		INT nState = 0;
		// set state
		if( isLoading )
			nState |= CEF_BIT_IS_LOADING;
		if( canGoBack )
			nState |= CEF_BIT_CAN_GO_BACK;
		if( canGoForward )
			nState |= CEF_BIT_CAN_GO_FORWARD;

		// The frame window will be the parent of the browser window
		HWND hWindow = GetParent( browser->GetHost()->GetWindowHandle() );

		// send message
		//::SendMessage( hWindow, WM_APP_CEF_STATE_CHANGE, (WPARAM)nState, NULL );

		// call parent
		CefLoadHandler::OnLoadingStateChange(browser, isLoading, canGoBack, canGoForward);
	}

	void BrowserClientHandler::OnAddressChange(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& url)
	{
		REQUIRE_UI_THREAD();

		// The frame window will be the parent of the browser window
		HWND hWindow = GetParent( browser->GetHost()->GetWindowHandle() );

		/*LPCTSTR pszURL(url.c_str());
		::SendMessage( hWindow, WM_APP_CEF_ADDRESS_CHANGE, (WPARAM)pszURL, NULL );*/

		// call parent
		CefDisplayHandler::OnAddressChange(browser, frame, url);
	}

	void BrowserClientHandler::OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title)
	{
		REQUIRE_UI_THREAD();

		m_delegate->onTitleChange(title);

		// call parent
		CefDisplayHandler::OnTitleChange(browser, title);
	}

	void BrowserClientHandler::OnStatusMessage(CefRefPtr<CefBrowser> browser, const CefString& value)
	{
		REQUIRE_UI_THREAD();

		// The frame window will be the parent of the browser window
		HWND hWindow = GetParent( browser->GetHost()->GetWindowHandle() );

		/*LPCTSTR pszStatus(value.c_str());
		::SendMessage( hWindow, WM_APP_CEF_STATUS_MESSAGE, (WPARAM)pszStatus, NULL );*/

		// call parent
		CefDisplayHandler::OnStatusMessage(browser, value);
	}

	bool BrowserClientHandler::OnConsoleMessage(CefRefPtr<CefBrowser> /*browser*/, cef_log_severity_t /*level*/, const CefString& /*message*/, const CefString& /*source*/, int /*line*/)
	{
		REQUIRE_UI_THREAD();
		return TRUE;
	}

	void BrowserClientHandler::OnBeforeDownload(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDownloadItem> download_item, const CefString& suggested_name, CefRefPtr<CefBeforeDownloadCallback> callback)
	{
		REQUIRE_UI_THREAD();

		if (boost::filesystem::extension(suggested_name.c_str()) == ".mp4")
		{
			// Continue the download and show the "Save As" dialog.
			callback->Continue(GetDownloadPath(suggested_name), true);
		}
		else
		{
			callback->Continue(m_delegate->getDownloadPath(suggested_name), false);
		}
	}

	void BrowserClientHandler::OnDownloadUpdated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDownloadItem> download_item, CefRefPtr<CefDownloadItemCallback> callback)
	{
		REQUIRE_UI_THREAD();

		events::DownloadStatusEvent evt;

		evt.bIsValid = download_item->IsValid();
		evt.bIsInProgress = download_item->IsInProgress();
		evt.bIsComplete = download_item->IsComplete();
		evt.bIsCanceled = download_item->IsCanceled();
		evt.nProgress = download_item->GetPercentComplete();
		evt.nSpeed = download_item->GetCurrentSpeed();
		evt.nReceived = download_item->GetReceivedBytes();
		evt.nTotal = download_item->GetTotalBytes();
		evt.m_path = download_item->GetFullPath();

		core::utils::patterns::Broker::get().publish(evt);
	}

	void BrowserClientHandler::OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, TransitionType transition_type)
	{
		REQUIRE_UI_THREAD();

		// The frame window will be the parent of the browser window
		//HWND hWindow = GetParent( browser->GetHost()->GetWindowHandle() );

		// send message
		//::SendMessage( hWindow, WM_APP_CEF_LOAD_START, NULL, NULL);

		// call parent
		CefLoadHandler::OnLoadStart(browser, frame, transition_type);
	}

	void BrowserClientHandler::OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode)
	{
		REQUIRE_UI_THREAD();

		// The frame window will be the parent of the browser window
		HWND hWindow = GetParent( browser->GetHost()->GetWindowHandle() );

		// send message
		//::SendMessage( hWindow, WM_APP_CEF_LOAD_END, httpStatusCode, NULL);

		events::BrowserLoadEndEvent evt;
		core::utils::patterns::Broker::get().publish(evt);

		// call parent
		CefLoadHandler::OnLoadEnd(browser, frame, httpStatusCode);
	}

	void BrowserClientHandler::OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl)
	{
		REQUIRE_UI_THREAD();

		// net_error_list.h NAME_NOT_RESOLVED -105
		if( errorCode == -105)
		{
			// The frame window will be the parent of the browser window
			HWND hWindow = GetParent( browser->GetHost()->GetWindowHandle() );

			/*LPCTSTR pszSearch(failedUrl.c_str());
			::SendMessage( hWindow, WM_APP_CEF_SEARCH_URL, (WPARAM)pszSearch, NULL );*/
		}

		// call parent
		CefLoadHandler::OnLoadError(browser, frame, errorCode, errorText, failedUrl);
	}

	bool BrowserClientHandler::GetAuthCredentials(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, bool isProxy, const CefString& host, int port, const CefString& realm, const CefString& scheme, CefRefPtr<CefAuthCallback> callback)
	{
		// The frame window will be the parent of the browser window
		HWND hWindow = GetParent( browser->GetHost()->GetWindowHandle() );

		//CEFAuthenticationValues values;
		//values.lpszHost = host.c_str();
		//values.lpszRealm = realm.c_str();
		//_tcscpy_s(values.szUserName, _T(""));
		//_tcscpy_s(values.szUserPass, _T(""));

		//// send info
		//if(::SendMessage( hWindow, WM_APP_CEF_AUTHENTICATE, (WPARAM)&values, (LPARAM)NULL ) == S_OK)
		//{
		//	callback->Continue( values.szUserName, values.szUserPass );
		//	return TRUE;
		//}
		// canceled
		return FALSE;
	}

	CefRefPtr<CefResourceHandler> BrowserClientHandler::GetResourceHandler(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request)
	{
		return m_resourceManager->GetResourceHandler(browser, frame, request);
	}

	void BrowserClientHandler::OnProtocolExecution(CefRefPtr<CefBrowser> browser, const CefString& url, bool& allow_os_execution)
	{
		// do default
		CefRequestHandler::OnProtocolExecution(browser, url, allow_os_execution);
	}

	bool BrowserClientHandler::OnBeforeBrowse(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, bool user_gesture, bool is_redirect)
	{
		// The frame window will be the parent of the browser window
		HWND hWindow = GetParent( browser->GetHost()->GetWindowHandle() );

		// get URL requested
		CefString newURL = request->GetURL();

		//LPCTSTR pszURL(newURL.c_str());
		//if( ::SendMessage( hWindow, WM_APP_CEF_BEFORE_BROWSE, (WPARAM)pszURL, (LPARAM)is_redirect ) == S_FALSE )
		//{
		//	// cancel navigation
		//	return TRUE;
		//}

		// call parent
		return CefRequestHandler::OnBeforeBrowse(browser, frame, request, user_gesture, is_redirect);
	}

	// CefJSDialogHandler methods
	bool BrowserClientHandler::OnBeforeUnloadDialog(CefRefPtr< CefBrowser > browser, const CefString& message_text, bool is_reload, CefRefPtr< CefJSDialogCallback > callback)
	{
		// do defulat
		return FALSE;
	}

	void BrowserClientHandler::OnDialogClosed(CefRefPtr< CefBrowser > browser)
	{
	}

	bool BrowserClientHandler::OnJSDialog(CefRefPtr<CefBrowser> /*browser*/,
		const CefString& /*origin_url*/,
		JSDialogType /*dialog_type*/,
		const CefString& /*message_text*/,
		const CefString& /*default_prompt_text*/,
		CefRefPtr<CefJSDialogCallback> /*callback*/,
		bool& suppress_message)
	{
		// do default
		suppress_message = FALSE;
		return FALSE;
	}

	void BrowserClientHandler::OnResetDialogState(CefRefPtr< CefBrowser > browser)
	{

	}

	CefRequestHandler::ReturnValue BrowserClientHandler::OnBeforeResourceLoad(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, CefRefPtr<CefRequestCallback> callback)
	{
		return m_resourceManager->OnBeforeResourceLoad(browser, frame, request, callback);
	}

	std::string BrowserClientHandler::GetDownloadPath(const std::string& file_name)
	{
		TCHAR szFolderPath[MAX_PATH];
		std::string path;

		// Save the file in the user's "My Documents" folder.
		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL | CSIDL_FLAG_CREATE,
			NULL, 0, szFolderPath))) {
				path = CefString(szFolderPath);
				path += "\\" + file_name;
		}
		
		return path;
	}
	
	bool BrowserClientHandler::OnCertificateError(
      CefRefPtr<CefBrowser> browser,
      cef_errorcode_t cert_error,
      const CefString& request_url,
      CefRefPtr<CefSSLInfo> ssl_info,
      CefRefPtr<CefRequestCallback> callback)
	{
		callback->Continue(true);
		return true;
	}

	CefRefPtr<CefKeyboardHandler> BrowserClientHandler::GetKeyboardHandler()
	{
		return m_keyboardHandler;
	}
}}

