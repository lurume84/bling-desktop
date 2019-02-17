#pragma once

#include <memory>

namespace bling { namespace core {

	namespace agent
	{
		class UpgradeViewerAgent;
		class SyncVideoAgent;
	}

	class BlingContext
	{
	public:
		BlingContext(std::unique_ptr<agent::UpgradeViewerAgent>, std::unique_ptr<agent::SyncVideoAgent>);
	private:
		std::unique_ptr<agent::UpgradeViewerAgent> m_upgradeVieweragent;
		std::unique_ptr<agent::SyncVideoAgent>		m_syncVideoAgent;
	};
}}