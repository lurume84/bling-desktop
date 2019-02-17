#pragma once

#include <memory>

namespace bling { namespace core {
	
	namespace agent
	{
		class UpgradeViewerAgent;
		class SyncVideoAgent;
	}
	
	class BlingContext;
	
	class BlingCore
	{
	public:
		BlingCore();
		~BlingCore();
		void initialize(std::unique_ptr<agent::UpgradeViewerAgent>, std::unique_ptr<agent::SyncVideoAgent>);
	private:
		std::unique_ptr<BlingContext> m_context;
	};
}}