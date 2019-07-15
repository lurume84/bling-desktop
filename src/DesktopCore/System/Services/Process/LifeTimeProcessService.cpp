#include "LifeTimeProcessService.h"

#include "System\Model\ProcessInformation.h"

#include <windows.h>
#include <fstream>

namespace desktop { namespace core { namespace service { namespace system {

	namespace
	{
		BOOL CALLBACK enumWindowsProc(__in  HWND hWnd,__in  LPARAM lParam)
		{
			return -1;
		}
	}

	bool LifeTimeProcessService::isAlive(model::system::ProcessInformation& processInfo) const
	{
		DWORD exitCode = 0;

		GetExitCodeProcess(processInfo.m_hProcess, &exitCode);

		return (exitCode == STILL_ACTIVE);
	}
}}}}
