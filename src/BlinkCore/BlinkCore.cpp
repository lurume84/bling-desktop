#include "BlinkCore.h"

#include "BlinkContext.h"

#include "Upgrade\Agents\UpgradeViewerAgent.h"

namespace blink { namespace core {
	BlinkCore::BlinkCore()
	{
	
	}

	BlinkCore::~BlinkCore() = default;
	
	void BlinkCore::initialize()
	{
		auto upgradeViewerAgent = std::make_unique<agent::UpgradeViewerAgent>();

		m_context = std::make_unique<BlinkContext>(std::move(upgradeViewerAgent));
	}
}}