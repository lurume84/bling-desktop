#include "stdafx.h"

#include "ExternalResourceManager.h"

#include "DesktopCore\Network\Model\Credentials.h"
#include "DesktopCore\Network\Events.h"
#include "DesktopCore\Network\Services\ParseURIService.h"
#include "DesktopCore\Utils\Patterns\PublisherSubscriber\Broker.h"

#include <locale>
#include <codecvt> 
#include <cstdint>

namespace desktop { namespace ui{

	CefRefPtr<CefResourceHandler> ExternalResourceManager::GetResourceHandler(CefRefPtr<CefBrowser> browser,CefRefPtr<CefFrame> frame,CefRefPtr<CefRequest> request)
	{
		return NULL;
	}

	CefRequestHandler::ReturnValue ExternalResourceManager::OnBeforeResourceLoad(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,CefRefPtr<CefRequest> request, CefRefPtr<CefRequestCallback> callback)
	{
		std::string url(request->GetURL());

		CefRequest::HeaderMap headers;
		request->GetHeaderMap(headers);

		for (auto &header : headers)
		{
			if (header.first == "TOKEN_AUTH")
			{
				std::string protocol, domain, port, path, query, fragment;

				core::service::ParseURIService service;
				if (service.parse(request->GetURL().ToString(), protocol, domain, port, path, query, fragment))
				{
					core::model::Credentials credentials(domain, port, header.second);
					core::events::CredentialsEvent evt(credentials);
					core::utils::patterns::Broker::get().publish(evt);
				}
				break;
			}
		}

		return RV_CONTINUE;
	}
}}