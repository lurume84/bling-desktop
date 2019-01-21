#include <memory>


#include "BlinkCore\Utils\Patterns\PublisherSubscriber\Subscriber.h"

namespace blink { namespace app {  namespace agent {


	class NotificationAgent
	{
	public:
		NotificationAgent();
		~NotificationAgent();

		HRESULT Initialize();

		HRESULT TryCreateShortcut();
		HRESULT InstallShortcut(_In_z_ wchar_t *shortcutPath);

		HRESULT DisplayToast(const std::string& title, const std::string& message);
		HRESULT CreateToastXml(const std::string& title, const std::string& message,
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

		static LRESULT CALLBACK WndProc(
			_In_ HWND hWnd,
			_In_ UINT message,
			_In_ WPARAM wParam,
			_In_ LPARAM lParam
		);
	private:
		blink::core::utils::patterns::Subscriber m_subscriber;
		HWND _hwnd;
		HWND _hEdit;
	};
}}}