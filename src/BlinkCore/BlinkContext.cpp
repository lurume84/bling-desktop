#include "BlinkContext.h"

#include "Upgrade/Agents/UpgradeViewerAgent.h"

namespace blink { namespace core {
	BlinkContext::BlinkContext(std::unique_ptr<agent::UpgradeViewerAgent> agent)
	: m_upgradeVieweragent(std::move(agent))
	{
	
	}
}}