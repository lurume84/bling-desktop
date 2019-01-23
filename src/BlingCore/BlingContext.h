#include <memory>

namespace bling { namespace core {

	namespace agent
	{
		class UpgradeViewerAgent;
	}

	class BlingContext
	{
	public:
		BlingContext(std::unique_ptr<agent::UpgradeViewerAgent>);
	private:
		std::unique_ptr<agent::UpgradeViewerAgent> m_upgradeVieweragent;
	};
}}