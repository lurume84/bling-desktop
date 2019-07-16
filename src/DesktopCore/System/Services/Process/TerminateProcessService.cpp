#include "TerminateProcessService.h"

#include "System\Model\ProcessInformation.h"

#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>

#include <boost/filesystem.hpp>
#include <sstream>

namespace desktop { namespace core { namespace service { namespace system {

	bool TerminateProcessService::terminate(const model::system::ProcessInformation& processInfo) const
	{
		bool result = TerminateProcess(processInfo.m_hProcess, 0) == TRUE;

		CloseHandle(processInfo.m_hProcess);
		CloseHandle(processInfo.m_hThread);

		return result;
	}

	bool TerminateProcessService::sigint(const model::system::ProcessInformation& processInfo) const
	{
		std::stringstream ss;
		ss << "taskkill /PID " << processInfo.m_processID;

		int ret = WinExec(ss.str().c_str(), SW_HIDE);

		return true;
	}

	unsigned long TerminateProcessService::listProcessThreads(unsigned long dwOwnerPID) const
	{
		HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
		THREADENTRY32 te32;

		/// Snapshot of all running threads  
		hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
		if (hThreadSnap == INVALID_HANDLE_VALUE)
		{
			return(FALSE);
		}

		/// Fill in the size of the structure before using it. 
		te32.dwSize = sizeof(THREADENTRY32);

		/// Retrieve information about the first thread, and exit if unsuccessful
		if (!Thread32First(hThreadSnap, &te32))
		{
			CloseHandle(hThreadSnap);     // Must clean up the snapshot object!

			return(FALSE);
		}

		/// Iterate the thread list and return the belonging to the specified process
		do
		{
			if (te32.th32OwnerProcessID == dwOwnerPID)
			{
				return te32.th32ThreadID;
			}
		} while (Thread32Next(hThreadSnap, &te32));

		CloseHandle(hThreadSnap);

		return 0;
	}

	bool TerminateProcessService::terminateProcessHandle(unsigned long dwProcessId) const
	{
		HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, dwProcessId);

		if (hProcess == NULL)
		{
			return FALSE;
		}

		UINT uExitCode = 0;
		bool result = TerminateProcess(hProcess, uExitCode) == TRUE;
		CloseHandle(hProcess);

		return result;
	}
}}}}
