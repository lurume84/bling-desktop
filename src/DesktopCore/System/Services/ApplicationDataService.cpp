#include "ApplicationDataService.h"

#include <boost/filesystem.hpp>
#include <windows.h>
#include <shlobj_core.h>

namespace desktop { namespace core { namespace service {

	ApplicationDataService::ApplicationDataService() = default;
	ApplicationDataService::~ApplicationDataService() = default;

	std::string ApplicationDataService::getMyDocuments() const
	{
		CHAR my_documents[MAX_PATH];
		HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, my_documents);

		if (result == S_OK)
		{
			return std::string(my_documents) + "\\" + getApplicationName() + "\\";
		}
		else
		{
			return "";
		}
	}

	std::string ApplicationDataService::getApplicationFolder() const
	{
		char moduleName[MAX_PATH + 1] = { '\0' };
		(void)GetModuleFileName(NULL, moduleName, MAX_PATH);

		auto path = boost::filesystem::path(moduleName).parent_path().string();

		return std::string(path);
	}

	std::string ApplicationDataService::getViewerFolder() const
	{
		char moduleName[MAX_PATH + 1] = { '\0' };
		(void)GetModuleFileName(NULL, moduleName, MAX_PATH);

		auto path = boost::filesystem::path(moduleName).parent_path() / "Html" / "viewer";

		return std::string(path.string());
	}

	std::string ApplicationDataService::getApplicationName() const
	{
		char moduleName[MAX_PATH + 1] = { '\0' };
		(void)GetModuleFileName(NULL, moduleName, MAX_PATH);

		auto filename = boost::filesystem::path(moduleName).filename().string();

		return std::string(filename);
	}
	
}}}