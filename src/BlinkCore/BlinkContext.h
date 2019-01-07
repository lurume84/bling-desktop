#include <memory>

namespace blink { namespace core {

	namespace agent
	{
		class UpgradeViewerAgent;
	}

	class BlinkContext
	{
	public:
		BlinkContext(std::unique_ptr<agent::UpgradeViewerAgent>);
	private:
		std::unique_ptr<agent::UpgradeViewerAgent> m_upgradeVieweragent;
	};
}}