#pragma once

#pragma warning( push )
#pragma warning( disable: 4100 )
#pragma warning(disable : 4481)
#include <cef/cef_client.h>
#include <cef/wrapper/cef_resource_manager.h>
#pragma warning( pop )

#include <list>
#include <map>
#include <set>
#include <string>
#include <memory>

namespace desktop{ namespace ui{

	extern const char kMultiThreadedMessageLoop[];
	extern const char kCachePath[];
	extern const char kUrl[];
	extern const char kExternalDevTools[];
	extern const char kOffScreenRenderingEnabled[];
	extern const char kTransparentPaintingEnabled[];
	extern const char kMouseCursorChangeDisabled[];

	class BrowserKeyboardHandler;
	class IResourceManager;

	class BrowserClientHandler : public CefClient,
	public CefContextMenuHandler,
	public CefDisplayHandler,
	public CefDownloadHandler,
	public CefLifeSpanHandler,
	public CefLoadHandler,
	public CefRequestHandler,
	public CefJSDialogHandler
{
public:

	// Interface implemented to handle off-screen rendering.
	class RenderHandler : public CefRenderHandler
	{
		public:
			virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) =0;
	};

	class Delegate
	{
	public:
		// Called when the browser is created.
		virtual void onBrowserCreated(CefRefPtr<CefBrowser> browser) = 0;

		// Called when the browser has been closed.
		virtual void onBrowserClosed(CefRefPtr<CefBrowser> browser) = 0;

		// Set the window title.
		virtual void onTitleChange(const std::string& title) = 0;

		virtual std::string getDownloadPath(const std::string& file_name) const = 0;

	protected:
		virtual ~Delegate() {}
  };

	BrowserClientHandler(Delegate* delegate, std::unique_ptr<IResourceManager> resourceManager);
	virtual ~BrowserClientHandler();

	// CefClient methods
	virtual CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() OVERRIDE{ return this; }
	virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE{ return this; }
	virtual CefRefPtr<CefDownloadHandler> GetDownloadHandler() OVERRIDE{ return this; }
	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE{ return this; }
	virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE{ return this; }
	virtual CefRefPtr<CefRequestHandler> GetRequestHandler() OVERRIDE{ return this; }
	virtual CefRefPtr<CefJSDialogHandler> GetJSDialogHandler() OVERRIDE{ return this; }

	// CefLifeSpanHandler methods
	virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
	virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
	virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

	// CefContextMenuHandler methods
	virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model) OVERRIDE;
	virtual bool OnContextMenuCommand(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, int command_id, EventFlags event_flags) OVERRIDE;
	
	// CefLoadHandler methods
	virtual void OnLoadingStateChange(CefRefPtr<CefBrowser> browser, bool isLoading, bool canGoBack, bool canGoForward) OVERRIDE;

	// CefDisplayHandler methods
	virtual void OnAddressChange(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& url) OVERRIDE;
	virtual void OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title) OVERRIDE;
	virtual void OnStatusMessage(CefRefPtr<CefBrowser> browser, const CefString& value) OVERRIDE;
	virtual bool OnConsoleMessage(CefRefPtr<CefBrowser> browser, cef_log_severity_t level, const CefString& message, const CefString& source, int line) OVERRIDE;

	// CefDownloadHandler methods
	virtual void OnBeforeDownload(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDownloadItem> download_item, const CefString& suggested_name, CefRefPtr<CefBeforeDownloadCallback> callback) OVERRIDE;
	virtual void OnDownloadUpdated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDownloadItem> download_item, CefRefPtr<CefDownloadItemCallback> callback) OVERRIDE;

	// CefLoadHandler methods
	virtual void OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, TransitionType transition_type) OVERRIDE;
	virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode) OVERRIDE;
	virtual void OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl) OVERRIDE;

	// CefRequestHandler methods
	virtual bool GetAuthCredentials( CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, bool isProxy, const CefString& host, int port, const CefString& realm, const CefString& scheme, CefRefPtr<CefAuthCallback> callback) OVERRIDE;
	virtual CefRefPtr<CefResourceHandler> GetResourceHandler(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request) OVERRIDE;
	virtual void OnProtocolExecution(CefRefPtr<CefBrowser> browser, const CefString& url, bool& allow_os_execution) OVERRIDE;
	virtual bool OnBeforeBrowse(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, bool user_gesture, bool is_redirect) OVERRIDE;
	// CefJSDialogHandler methods
	virtual bool OnBeforeUnloadDialog(CefRefPtr< CefBrowser > browser, const CefString& message_text, bool is_reload, CefRefPtr< CefJSDialogCallback > callback) OVERRIDE;
	virtual void OnDialogClosed(CefRefPtr< CefBrowser > browser) OVERRIDE;
	virtual bool OnJSDialog(CefRefPtr<CefBrowser> browser,
		const CefString& origin_url,
		JSDialogType dialog_type,
		const CefString& message_text,
		const CefString& default_prompt_text,
		CefRefPtr<CefJSDialogCallback> callback,
		bool& suppress_message) OVERRIDE;
	virtual void OnResetDialogState(CefRefPtr< CefBrowser > browser) OVERRIDE;
	virtual ReturnValue OnBeforeResourceLoad(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, CefRefPtr<CefRequestCallback> callback) OVERRIDE;

	CefRefPtr<CefKeyboardHandler> GetKeyboardHandler() OVERRIDE;

	bool OnCertificateError(
      CefRefPtr<CefBrowser> browser,
      cef_errorcode_t cert_error,
      const CefString& request_url,
      CefRefPtr<CefSSLInfo> ssl_info,
      CefRefPtr<CefRequestCallback> callback);

	// Returns the startup URL.
	std::string GetStartupURL() { return m_StartupURL; }

	void DetachDelegate();
protected:
	// Returns the full download path for the specified file, or an empty path to use the default temp directory.
	std::string GetDownloadPath(const std::string& file_name);

	// Support for downloading files.
	std::string m_LastDownloadFile;

	// The startup URL.
	std::string m_StartupURL;

	Delegate* m_delegate;

	CefRefPtr<BrowserKeyboardHandler> m_keyboardHandler;

	std::unique_ptr<IResourceManager> m_resourceManager;

	// Include the default reference counting implementation.
	IMPLEMENT_REFCOUNTING(BrowserClientHandler);

	// Include the default locking implementation.
	//IMPLEMENT_LOCKING(ClientHandler);
};
}}