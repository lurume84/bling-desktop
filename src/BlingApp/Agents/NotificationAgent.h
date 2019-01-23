#include <memory>

#include "BlingCore\Utils\Patterns\PublisherSubscriber\Subscriber.h"

namespace bling { namespace app {  namespace agent {


	class NotificationAgent
	{
	public:
		NotificationAgent();
		~NotificationAgent();

		bool initialize();

		HRESULT TryCreateShortcut();
		HRESULT InstallShortcut(_In_z_ wchar_t *shortcutPath);
	private:
		bling::core::utils::patterns::Subscriber m_subscriber;
	};
}}}