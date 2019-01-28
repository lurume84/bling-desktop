#pragma once

#include "Toast\Toast.h"
#include <functional>

namespace bling { namespace ui {  namespace toast {
	class ToastEventHandler : public ToastPP::INotifier 
	{
	public:
		ToastEventHandler::ToastEventHandler(std::function<bool()> activated, std::function<bool()> dismissed, std::function<bool()> failed);
		~ToastEventHandler();
       
		void OnToastActivated(_In_opt_ ABI::Windows::UI::Notifications::IToastNotification* pSender, _In_opt_ IInspectable* pArgs) override;
		void OnToastDismissed(_In_opt_ ABI::Windows::UI::Notifications::IToastNotification* pSender, _In_ ABI::Windows::UI::Notifications::ToastDismissalReason reason) override;
		void OnToastFailed(_In_opt_ ABI::Windows::UI::Notifications::IToastNotification* pSender, _In_ HRESULT errorCode) override;

	private:
		std::function<bool()> m_activated;
		std::function<bool()> m_dismissed;
		std::function<bool()> m_failed;
	};
}}}
