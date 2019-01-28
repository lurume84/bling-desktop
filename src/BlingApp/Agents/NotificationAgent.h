#include "Toast\Toast.h"

#include "BlingCore\Utils\Patterns\PublisherSubscriber\Subscriber.h"

#include <memory>

namespace bling { namespace ui {  
	
	namespace toast
	{
		class ToastEventHandler;
	}
	
	namespace agent {

	namespace cup = core::utils::patterns;

	const cup::EventType SHOW_NOTIFICATION_EVENT = "SHOW_NOTIFICATION_EVENT";

	class NotificationAgent
	{
	public:
		struct ShowNotificationEvent : public cup::Event
		{
			ShowNotificationEvent(std::unique_ptr<Notification> handler)
				: m_handler(std::move(handler))
			{
				m_name = SHOW_NOTIFICATION_EVENT;
			}

			~ShowNotificationEvent();

			std::unique_ptr<toast::ToastEventHandler> m_handler;
		};

		NotificationAgent();
		~NotificationAgent();

		bool initialize();
		
	private:
		cup::Subscriber m_subscriber;
		ToastPP::CManager m_ToastManager;
		ToastPP::CToast m_Toast;
	};
}}}