#include "ApplicationDataService.h"

#include <boost/filesystem.hpp>
#include <windows.h>
#include <sstream>
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

		auto path = boost::filesystem::path(getMyDocuments()) / "Html" / "viewer";

		return std::string(path.string());
	}

	std::string ApplicationDataService::getApplicationName() const
	{
		char moduleName[MAX_PATH + 1] = { '\0' };
		(void)GetModuleFileName(NULL, moduleName, MAX_PATH);

		auto filename = boost::filesystem::path(moduleName).filename().string();

		return std::string(filename);
	}
	
	std::string ApplicationDataService::getApplicationVersion() const
	{
		std::string defaultVersion = "xxx";
		TCHAR buffer[MAX_PATH] = { 0 };
		DWORD bufSize = sizeof(buffer) / sizeof(*buffer);
		// Get the fully-qualified path of the executable
		if (GetModuleFileName(NULL, buffer, bufSize) == bufSize)
		{
			throw std::runtime_error("Path too long");
		}

		DWORD dwHandle, sz;
		sz = GetFileVersionInfoSizeA(buffer, &dwHandle);
		if (0 == sz)
		{
			DWORD error = GetLastError();
			UNREFERENCED_PARAMETER(error);
			return defaultVersion;
		}
		char *buf = new char[sz];
		if (!GetFileVersionInfoA(buffer, dwHandle, sz, &buf[0]))
		{
			delete buf;
			return defaultVersion;
		}
		VS_FIXEDFILEINFO * pvi;
		sz = sizeof(VS_FIXEDFILEINFO);
		if (!VerQueryValueA(&buf[0], "\\", (LPVOID*)&pvi, (unsigned int*)&sz))
		{
			delete buf;
			return defaultVersion;
		}
		std::ostringstream oss;
		oss << (pvi->dwProductVersionMS >> 16)
			<< "." << (pvi->dwFileVersionMS & 0xFFFF)
			<< "." << (pvi->dwFileVersionLS >> 16)
			<< "." << (pvi->dwFileVersionLS & 0xFFFF);
		delete buf;
		return oss.str();
	}
}}}