#include <memory>

#include "BlingCore\Utils\Patterns\PublisherSubscriber\Subscriber.h"

namespace bling { namespace ui {  namespace agent {


	class NotificationAgent
	{
	public:
		NotificationAgent();
		~NotificationAgent();

		bool initialize();

	private:
		bling::core::utils::patterns::Subscriber m_subscriber;
	};
}}}