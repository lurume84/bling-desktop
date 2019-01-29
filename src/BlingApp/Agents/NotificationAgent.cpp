#include "stdafx.h"

#include "NotificationAgent.h"

#include "BlingCore\Upgrade\Events.h"

#include "Toast\ToastEventHandler.h"

namespace bling { namespace ui {  namespace agent {

	NotificationAgent::ShowNotificationEvent::~ShowNotificationEvent() = default;

	NotificationAgent::NotificationAgent()
	{
		m_subscriber.subscribe([this](const bling::core::utils::patterns::Event& rawEvt)
		{
			const auto& evt = static_cast<const ShowNotificationEvent&>(rawEvt);

			m_ToastManager.Show(m_Toast, evt.m_handler.get());

		}, SHOW_NOTIFICATION_EVENT);
	}

	NotificationAgent::~NotificationAgent()
	{
		
	}

	bool NotificationAgent::initialize()
	{
		HRESULT hr = m_ToastManager.Create(L"Bling.Desktop");
		return SUCCEEDED(hr);
	}
	

}}}