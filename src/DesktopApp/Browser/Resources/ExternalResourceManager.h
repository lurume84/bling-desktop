#pragma once

#include "IResourceManager.h"

namespace desktop { namespace ui{

	class ExternalResourceManager : public IResourceManager
	{
		CefRefPtr<CefResourceHandler> GetResourceHandler(
		  CefRefPtr<CefBrowser> browser,
		  CefRefPtr<CefFrame> frame,
		  CefRefPtr<CefRequest> request) override;

		  CefRequestHandler::ReturnValue OnBeforeResourceLoad(
		  CefRefPtr<CefBrowser> browser,
		  CefRefPtr<CefFrame> frame,
		  CefRefPtr<CefRequest> request,
		  CefRefPtr<CefRequestCallback> callback) override;
	};
}}