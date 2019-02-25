#include "FileInfoService.h"

#include <Windows.h>
#include <sstream>

namespace desktop { namespace core { namespace service {

	FileInfoService::FileInfoService() = default;
	FileInfoService::~FileInfoService() = default;

	std::string FileInfoService::getProductVersion(const std::string& file) const
	{
		std::string version("");

		DWORD  verHandle = 0;
		UINT   size = 0;
		LPBYTE lpBuffer = NULL;
		DWORD  verSize = GetFileVersionInfoSize(file.c_str(), &verHandle);

		if (verSize != NULL)
		{
			LPSTR verData = new char[verSize];

			if (GetFileVersionInfo(file.c_str(), verHandle, verSize, verData))
			{
				if (VerQueryValue(verData, "\\", (VOID FAR* FAR*)&lpBuffer, &size))
				{
					if (size)
					{
						VS_FIXEDFILEINFO *verInfo = (VS_FIXEDFILEINFO *)lpBuffer;
						if (verInfo->dwSignature == 0xfeef04bd)
						{
							std::stringstream ss;
							ss << ((verInfo->dwFileVersionMS >> 16) & 0xffff) << "."
								<< ((verInfo->dwFileVersionMS >> 0) & 0xffff) << "."
								<< ((verInfo->dwFileVersionLS >> 16) & 0xffff) << "."
								<< ((verInfo->dwFileVersionLS >> 0) & 0xffff);

							version = ss.str();
						}
					}
				}
			}
			delete[] verData;
		}

		return version;
	}
	
}}}