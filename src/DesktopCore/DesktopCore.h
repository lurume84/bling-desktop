#pragma once

#include <memory>

namespace desktop { namespace core {
	
	namespace agent
	{
		class UpgradeViewerAgent;
		class SyncVideoAgent;
	}
	
	class DesktopContext;
	
	class DesktopCore
	{
	public:
		DesktopCore();
		~DesktopCore();
		void initialize(std::unique_ptr<agent::UpgradeViewerAgent>, std::unique_ptr<agent::SyncVideoAgent>);
	private:
		std::unique_ptr<DesktopContext> m_context;
	};
}}