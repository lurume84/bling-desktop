#pragma once

#include <memory>

struct _PROCESS_INFORMATION;
typedef struct _PROCESS_INFORMATION PROCESS_INFORMATION;

namespace desktop { namespace core { namespace model { namespace system {
	struct ProcessInformation
	{
		ProcessInformation(const PROCESS_INFORMATION& processInfo);

		bool operator==(const PROCESS_INFORMATION& other) const;
				
		void* m_hProcess;
		void* m_hThread;
		unsigned long m_processID;
		unsigned long m_threadID;
	};
}}}}