#include "stdafx.h"

#include "ExternalResourceManager.h"

#include "BlingCore\Network\Model\Credentials.h"
#include "BlingCore\Network\Events.h"
#include "BlingCore\Network\Services\ParseURIService.h"
#include "BlingCore\Utils\Patterns\PublisherSubscriber\Broker.h"

namespace bling { namespace ui{

	CefRefPtr<CefResourceHandler> ExternalResourceManager::GetResourceHandler(CefRefPtr<CefBrowser> browser,CefRefPtr<CefFrame> frame,CefRefPtr<CefRequest> request)
	{
		return NULL;
	}

	CefRequestHandler::ReturnValue ExternalResourceManager::OnBeforeResourceLoad(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,CefRefPtr<CefRequest> request, CefRefPtr<CefRequestCallback> callback)
	{
		CefRequest::HeaderMap headers;
		request->GetHeaderMap(headers);

		for (auto &header : headers)
		{
			if (header.first == "TOKEN_AUTH")
			{
				std::string protocol, domain, port, path, query, fragment;
				std::string url = request->GetURL().ToString();

				core::service::ParseURIService service;
				if (service.parse(url, protocol, domain, port, path, query, fragment))
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