#include "StdAfx.h"

#include "RequestResourceProvider.h"

#pragma warning(push)
#pragma warning(disable : 4100)
#pragma warning(disable : 4481)
#include <cef/wrapper/cef_stream_resource_handler.h>
#pragma warning(pop)

namespace desktop { namespace ui{

	RequestResourceProvider::RequestResourceProvider()
	{

	}

	bool RequestResourceProvider::OnRequest(scoped_refptr<CefResourceManager::Request> request)
	{
		CEF_REQUIRE_IO_THREAD();

		return true;
	}
}}
