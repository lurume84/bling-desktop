#include "StdAfx.h"

#include "InternalResourceManager.h"

#include "RequestResourceProvider.h"

namespace desktop { namespace ui{

	InternalResourceManager::InternalResourceManager()
		: m_resourceManager(new CefResourceManager)
	{
	
	}

	CefRefPtr<CefResourceHandler> InternalResourceManager::GetResourceHandler(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefRequest> request)
	{
		return m_resourceManager->GetResourceHandler(browser, frame, request);
	}

	CefRequestHandler::ReturnValue InternalResourceManager::OnBeforeResourceLoad(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefRequest> request,
		CefRefPtr<CefRequestCallback> callback)
	{
		return m_resourceManager->OnBeforeResourceLoad(browser, frame, request, callback);
	}

}}
