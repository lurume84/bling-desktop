#include "DesktopContext.h"

#include "Upgrade/Agents/UpgradeViewerAgent.h"
#include "Blink/Agents/SyncVideoAgent.h"

namespace desktop { namespace core {
	DesktopContext::DesktopContext(std::unique_ptr<agent::UpgradeViewerAgent> agent, 
							   std::unique_ptr<agent::SyncVideoAgent> syncVideoAgent)
	: m_upgradeVieweragent(std::move(agent))
	, m_syncVideoAgent(std::move(syncVideoAgent))
	{
	
	}
}}