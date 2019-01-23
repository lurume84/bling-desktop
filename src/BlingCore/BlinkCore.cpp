#include "BlingCore.h"

#include "BlingContext.h"

#include "Upgrade\Agents\UpgradeViewerAgent.h"

namespace bling { namespace core {
	BlingCore::BlingCore()
	{
	
	}

	BlingCore::~BlingCore() = default;
	
	void BlingCore::initialize()
	{
		auto upgradeViewerAgent = std::make_unique<agent::UpgradeViewerAgent>("api.github.com");

		m_context = std::make_unique<BlingContext>(std::move(upgradeViewerAgent));
	}
}}