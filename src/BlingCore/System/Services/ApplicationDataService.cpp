#include "ApplicationDataService.h"

#include <boost/filesystem.hpp>
#include <windows.h>
#include <shlobj_core.h>

namespace bling { namespace core { namespace service {

	ApplicationDataService::ApplicationDataService() = default;
	ApplicationDataService::~ApplicationDataService() = default;

	std::string ApplicationDataService::getMyDocuments() const
	{
		CHAR my_documents[MAX_PATH];
		HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, my_documents);

		if (result == S_OK)
		{
			return std::string(my_documents) + "\\Bling Desktop\\";
		}
		else
		{
			return "";
		}
	}
	
}}}