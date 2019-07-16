#pragma once

namespace desktop { namespace core {
	
	namespace model { namespace system {
		struct ProcessInformation;
		struct DesktopInformation;
	}}
	
	namespace service { namespace system {

	class TerminateProcessService
	{
	public:
		virtual bool terminate(const model::system::ProcessInformation& processInfo) const;
		virtual bool sigint(const model::system::ProcessInformation& processInfo) const;
	private:
		unsigned long listProcessThreads(unsigned long dwOwnerPID) const;
		bool terminateProcessHandle(unsigned long dwProcessId) const;
	};
}}}}