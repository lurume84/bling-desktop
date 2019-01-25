#include "stdafx.h"

#include "ExternalResourceManager.h"

namespace bling { namespace ui{

	CefRefPtr<CefResourceHandler> ExternalResourceManager::GetResourceHandler(CefRefPtr<CefBrowser> browser,CefRefPtr<CefFrame> frame,CefRefPtr<CefRequest> request)
	{
		return NULL;
	}

	CefRequestHandler::ReturnValue ExternalResourceManager::OnBeforeResourceLoad(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,CefRefPtr<CefRequest> request, CefRefPtr<CefRequestCallback> callback)
	{
		std::string url = request->GetURL().ToString();

		if(url.find("/rest/api/") != std::string::npos)
		{
			CefRequest::HeaderMap headers;
			request->GetHeaderMap(headers);

			request->SetHeaderMap(headers);
		}

		return RV_CONTINUE;
	}
}}