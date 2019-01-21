#include <memory>

#include "BlinkCore\Utils\Patterns\PublisherSubscriber\Subscriber.h"

namespace blink { namespace app {  namespace agent {


	class NotificationAgent
	{
	public:
		NotificationAgent();
		~NotificationAgent();

		bool initialize();

		HRESULT TryCreateShortcut();
		HRESULT InstallShortcut(_In_z_ wchar_t *shortcutPath);
	private:
		blink::core::utils::patterns::Subscriber m_subscriber;
	};
}}}