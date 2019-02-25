#pragma once

#include <memory>

namespace desktop { namespace core {

	namespace agent
	{
		class UpgradeViewerAgent;
		class SyncVideoAgent;
	}

	class DesktopContext
	{
	public:
		DesktopContext(std::unique_ptr<agent::UpgradeViewerAgent>, std::unique_ptr<agent::SyncVideoAgent>);
	private:
		std::unique_ptr<agent::UpgradeViewerAgent> m_upgradeVieweragent;
		std::unique_ptr<agent::SyncVideoAgent>		m_syncVideoAgent;
	};
}}