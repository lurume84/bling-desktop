#pragma once

#include "IResourceManager.h"

namespace desktop { namespace ui{

	class InternalResourceManager: public IResourceManager
	{
	public:
		InternalResourceManager();

		CefRefPtr<CefResourceHandler> GetResourceHandler(
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefRequest> request) override;

		CefRequestHandler::ReturnValue OnBeforeResourceLoad(
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefRequest> request,
			CefRefPtr<CefRequestCallback> callback) override;
	private:
		CefRefPtr<CefResourceManager> m_resourceManager;
	};
}}