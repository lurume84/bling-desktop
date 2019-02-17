#include "BlingContext.h"

#include "Upgrade/Agents/UpgradeViewerAgent.h"
#include "Blink/Agents/SyncVideoAgent.h"

namespace bling { namespace core {
	BlingContext::BlingContext(std::unique_ptr<agent::UpgradeViewerAgent> agent, 
							   std::unique_ptr<agent::SyncVideoAgent> syncVideoAgent)
	: m_upgradeVieweragent(std::move(agent))
	, m_syncVideoAgent(std::move(syncVideoAgent))
	{
	
	}
}}