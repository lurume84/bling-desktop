#include "BlingCore.h"

#include "BlingContext.h"

#include "Upgrade\Agents\UpgradeViewerAgent.h"

namespace bling { namespace core {
	BlingCore::BlingCore() = default;
	BlingCore::~BlingCore() = default;
	
	void BlingCore::initialize(std::unique_ptr<agent::UpgradeViewerAgent> upgradeViewerAgent)
	{
		m_context = std::make_unique<BlingContext>(std::move(upgradeViewerAgent));
	}
}}