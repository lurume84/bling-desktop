#pragma once

#include "Toast\Toast.h"

#include "DesktopCore\Utils\Patterns\PublisherSubscriber\Subscriber.h"

#include <memory>

namespace desktop { namespace ui {  
	
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
			ShowNotificationEvent(std::shared_ptr<ToastPP::CToast> toast, std::shared_ptr<toast::ToastEventHandler> handler)
				: m_handler(handler)
				, m_toast(toast)
			{
				m_name = SHOW_NOTIFICATION_EVENT;
			}

			~ShowNotificationEvent();

			std::shared_ptr<ToastPP::CToast> m_toast;
			std::shared_ptr<toast::ToastEventHandler> m_handler;
		};

		NotificationAgent(const std::wstring& appName);
		~NotificationAgent();

		bool initialize(boost::optional<bool> doRegister, std::string& reason, int &ecode);

		HRESULT RegisterCOMServer(_In_z_ PCWSTR pszExePath);
		HRESULT UnRegisterCOMServer();
		HRESULT RegisterActivator();
		void UnregisterActivator();
		
	private:
		cup::Subscriber m_subscriber;
		ToastPP::CManager m_ToastManager;
		Microsoft::WRL::Wrappers::RoInitializeWrapper m_winRTInitializer;

		std::wstring m_appName;
	};
}}}