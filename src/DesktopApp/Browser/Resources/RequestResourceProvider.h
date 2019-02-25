#pragma once

#pragma warning(push)
#pragma warning(disable : 4100)
#pragma warning(disable : 4481)
#include <cef/wrapper/cef_resource_manager.h>
#pragma warning(pop)

namespace desktop { namespace ui{

	class RequestResourceProvider : public CefResourceManager::Provider
	{
	public:
		explicit RequestResourceProvider();
		bool OnRequest(scoped_refptr<CefResourceManager::Request> request);
	};
}}