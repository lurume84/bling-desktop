#include "CreateProcessService.h"

#include "System\Model\ProcessInformation.h"
#include "System\Model\ExecutableFile.h"

#include <windows.h>

namespace desktop { namespace core { namespace service { namespace system {

	std::unique_ptr<model::system::ProcessInformation> CreateProcessService::create(const model::system::ExecutableFile& file, const std::string& currentDirectory) const
	{
		PROCESS_INFORMATION processInfo;
		ZeroMemory(&processInfo, sizeof(processInfo));

		STARTUPINFO startUpInfo;
		ZeroMemory(&startUpInfo, sizeof(startUpInfo));

		startUpInfo.dwFlags = STARTF_USESHOWWINDOW;
		startUpInfo.wShowWindow = SW_HIDE;

		std::string path = file.m_path.get().string() + " " + file.m_arguments.get();

		if (!CreateProcess(NULL, (char*)path.c_str(), NULL, NULL, FALSE, CREATE_NEW_CONSOLE | CREATE_NEW_PROCESS_GROUP, NULL, currentDirectory.c_str(), &startUpInfo, &processInfo))
		{
			throw CreateProcessServiceException() << CreateProcessServiceException::pathInfo(path) << CreateProcessServiceException::errorCodeInfo(GetLastError());
		}

		return std::make_unique<model::system::ProcessInformation>(processInfo);
	}
}}}}
