#pragma once

#include <memory>

namespace bling { namespace core {
	
	namespace agent
	{
		class UpgradeViewerAgent;
	}
	
	class BlingContext;
	
	class BlingCore
	{
	public:
		BlingCore();
		~BlingCore();
		void initialize(std::unique_ptr<agent::UpgradeViewerAgent>);
	private:
		std::unique_ptr<BlingContext> m_context;
	};
}}