#include "ProcessInformation.h"

#include <windows.h>

namespace desktop { namespace core { namespace model { namespace system {
	ProcessInformation::ProcessInformation(const PROCESS_INFORMATION& processInfo)
	: m_hProcess(processInfo.hProcess)
	, m_hThread(processInfo.hThread)
	, m_processID(processInfo.dwProcessId)
	, m_threadID(processInfo.dwThreadId)
	{

	}

	bool ProcessInformation::operator==(const PROCESS_INFORMATION& other) const
	{
		return m_hProcess == other.hProcess && m_hThread == other.hThread;
	}
}}}}