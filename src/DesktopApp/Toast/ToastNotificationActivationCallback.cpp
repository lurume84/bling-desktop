#include "stdafx.h"

#include "ToastNotificationActivationCallback.h"
#include "DesktopApp.h"

namespace desktop { namespace ui {  namespace toast {
	HRESULT ToastNotificationActivationCallback::Activate(__RPC__in_string LPCWSTR appUserModelId, __RPC__in_opt_string LPCWSTR invokedArgs,
		__RPC__in_ecount_full_opt(count) const NOTIFICATION_USER_INPUT_DATA* data, ULONG count)
	{
		CStringW sMsg;
		sMsg.Format(L"INotificationActivationCallback, The toast was activated. appUserModelId:\"%s\", Arguments:\"%s\"\r\n", appUserModelId, invokedArgs);

		if (count)
		{
			ATLASSUME(data != nullptr);
			sMsg += L" Data:\r\n";

			for (ULONG i = 0; i<count; i++)
			{
#pragma warning(suppress: 6011)
				sMsg.AppendFormat(L"  Key:\"%s\", Value:\"%s\"\r\n", data[i].Key, data[i].Value);
			}
		}

		theApp.m_toastAction = invokedArgs;

		return S_OK;
	}
}}}