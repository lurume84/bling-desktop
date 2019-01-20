#include <memory>


#ifndef _SECURE_ATL
#define _SECURE_ATL 1 //Use the Secure C Runtime in ATL
#endif

#include "BlinkCore\Utils\Patterns\PublisherSubscriber\Subscriber.h"

#include "../Toast.h"

#include <wrl.h>


typedef struct NOTIFICATION_USER_INPUT_DATA
{
	LPCWSTR Key;
	LPCWSTR Value;
}  NOTIFICATION_USER_INPUT_DATA;

MIDL_INTERFACE("53E31837-6600-4A81-9395-75CFFE746F94")
INotificationActivationCallback : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE Activate(__RPC__in_string LPCWSTR appUserModelId, __RPC__in_opt_string LPCWSTR invokedArgs,
		__RPC__in_ecount_full_opt(count) const NOTIFICATION_USER_INPUT_DATA *data, ULONG count) = 0;
};

namespace blink { namespace app {  namespace agent {


	class NotificationAgent : public ToastPP::INotifier
	{
	public:
		NotificationAgent();
		~NotificationAgent();

		void OnToastActivated(_In_opt_ ABI::Windows::UI::Notifications::IToastNotification* pSender, _In_opt_ IInspectable* pArgs) override;
		void OnToastDismissed(_In_opt_ ABI::Windows::UI::Notifications::IToastNotification* pSender, _In_ ABI::Windows::UI::Notifications::ToastDismissalReason reason) override;
		void OnToastFailed(_In_opt_ ABI::Windows::UI::Notifications::IToastNotification* pSender, _In_ HRESULT errorCode) override;

		HRESULT RegisterCOMServer(_In_z_ PCWSTR pszExePath);
		HRESULT UnRegisterCOMServer();
		HRESULT RegisterActivator();
		void UnregisterActivator();
	private:
		blink::core::utils::patterns::Subscriber m_subscriber;
		ToastPP::CManager	m_ToastManager;
		ToastPP::CToast		m_Toast;
		Microsoft::WRL::Wrappers::RoInitializeWrapper m_winRTInitializer;
	};
}}}

//The COM server which implements the callback notifcation from Action Center
class DECLSPEC_UUID("383803B6-AFDA-4220-BFC3-0DBF810106BF")
	CToastNotificationActivationCallback : public Microsoft::WRL::RuntimeClass<Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::ClassicCom>, INotificationActivationCallback>
{
public:
	//Constructors / Destructors
	CToastNotificationActivationCallback()
	{
	}

	virtual HRESULT STDMETHODCALLTYPE Activate(__RPC__in_string LPCWSTR appUserModelId, __RPC__in_opt_string LPCWSTR invokedArgs,
		__RPC__in_ecount_full_opt(count) const NOTIFICATION_USER_INPUT_DATA* data, ULONG count) override;
};
CoCreatableClass(CToastNotificationActivationCallback);
