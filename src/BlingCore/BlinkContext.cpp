#include "BlingContext.h"

#include "Upgrade/Agents/UpgradeViewerAgent.h"

namespace bling { namespace core {
	BlingContext::BlingContext(std::unique_ptr<agent::UpgradeViewerAgent> agent)
	: m_upgradeVieweragent(std::move(agent))
	{
	
	}
}}