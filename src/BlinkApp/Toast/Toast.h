#pragma once

#include "BlinkCore\Utils\Patterns\PublisherSubscriber\Event.h"
#include <string>

class ToastEventHandler;

namespace blink { namespace app {

	namespace sup = core::utils::patterns;

	const sup::EventType NOTIFICATION_EVENT = "NOTIFICATION_EVENT";
	struct NotificationEvent : public sup::Event
	{
		NotificationEvent(const std::string& version)
			: m_version(version)
		{
			m_name = NOTIFICATION_EVENT;
		}

		std::string m_version;
	};

	class Toast
	{
	public:
		Toast(ToastEventHandler* handler);

		HRESULT DisplayToast(const std::wstring& title, const std::wstring& message, const std::wstring& icon);
		HRESULT CreateToastXml(const std::wstring& title, const std::wstring& message, const std::wstring& icon,
			_In_ ABI::Windows::UI::Notifications::IToastNotificationManagerStatics *toastManager,
			_Outptr_ ABI::Windows::Data::Xml::Dom::IXmlDocument **xml
		);

		HRESULT CreateToast(
			_In_ ABI::Windows::UI::Notifications::IToastNotificationManagerStatics *toastManager,
			_In_ ABI::Windows::Data::Xml::Dom::IXmlDocument *xml
		);
		HRESULT SetImageSrc(
			_In_z_ wchar_t *imagePath,
			_In_ ABI::Windows::Data::Xml::Dom::IXmlDocument *toastXml
		);
		HRESULT SetTextValues(
			_In_reads_(textValuesCount) wchar_t **textValues,
			_In_ UINT32 textValuesCount,
			_In_reads_(textValuesCount) UINT32 *textValuesLengths,
			_In_ ABI::Windows::Data::Xml::Dom::IXmlDocument *toastXml
		);

		HRESULT SetNodeValueString(
			_In_ HSTRING onputString,
			_In_ ABI::Windows::Data::Xml::Dom::IXmlNode *node,
			_In_ ABI::Windows::Data::Xml::Dom::IXmlDocument *xml
		);

	private:
		std::wstring m_appID;
		ToastEventHandler* m_handler;
	};
}}