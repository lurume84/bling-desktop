#pragma once

#include <wrl.h>
//Define INotificationActivationCallback for older versions of the Windows SDK
#include <ntverp.h>
#if VER_PRODUCTBUILD < 10011

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

#else
#include <NotificationActivationCallback.h>
#endif //#if VER_PRODUCTBUILD < 10011

namespace bling { namespace ui {  namespace toast {
	//The COM server which implements the callback notifcation from Action Center
	class DECLSPEC_UUID("383803B6-AFDA-4220-BFC3-0DBF810106BF")
		ToastNotificationActivationCallback : public Microsoft::WRL::RuntimeClass<Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::ClassicCom>, INotificationActivationCallback>
	{
	public:
		//Constructors / Destructors
		ToastNotificationActivationCallback()
		{
		}

		virtual HRESULT STDMETHODCALLTYPE Activate(__RPC__in_string LPCWSTR appUserModelId, __RPC__in_opt_string LPCWSTR invokedArgs,
			__RPC__in_ecount_full_opt(count) const NOTIFICATION_USER_INPUT_DATA* data, ULONG count) override;
	};
	CoCreatableClass(ToastNotificationActivationCallback);
}}}