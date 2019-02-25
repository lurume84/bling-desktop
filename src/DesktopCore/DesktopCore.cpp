#include "DesktopCore.h"

#include "DesktopContext.h"

#include "Upgrade\Agents\UpgradeViewerAgent.h"
#include "Blink\Agents\SyncVideoAgent.h"

namespace desktop { namespace core {
	DesktopCore::DesktopCore() = default;
	DesktopCore::~DesktopCore() = default;
	
	void DesktopCore::initialize(std::unique_ptr<agent::UpgradeViewerAgent> upgradeViewerAgent,
								std::unique_ptr<agent::SyncVideoAgent> syncVideoAgent)
	{
		m_context = std::make_unique<DesktopContext>(std::move(upgradeViewerAgent), std::move(syncVideoAgent));
	}
}}