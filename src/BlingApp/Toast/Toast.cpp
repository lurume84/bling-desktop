#include "stdafx.h"

#include "Toast.h"

#include "StringReferenceWrapper.h"

#include "Toast\ToastEventHandler.h"

using namespace Microsoft::WRL;
using namespace ABI::Windows::UI::Notifications;
using namespace ABI::Windows::Data::Xml::Dom;
using namespace Windows::Foundation;

namespace bling { namespace app {

	Toast::Toast(ToastEventHandler* handler)
	: m_handler(handler)
	{
	
	}

	// Display the toast using classic COM. Note that is also possible to create and display the toast using the new C++ /ZW options (using handles,
	// COM wrappers, etc.) 
	HRESULT Toast::DisplayToast(const std::wstring& title, const std::wstring& message, const std::wstring& icon)
	{
		ComPtr<IToastNotificationManagerStatics> toastStatics;
		HRESULT hr = GetActivationFactory(StringReferenceWrapper(RuntimeClass_Windows_UI_Notifications_ToastNotificationManager).Get(), &toastStatics);

		if (SUCCEEDED(hr))
		{
			ComPtr<IXmlDocument> toastXml;
			hr = CreateToastXml(title, message, icon, toastStatics.Get(), &toastXml);
			if (SUCCEEDED(hr))
			{
				hr = CreateToast(toastStatics.Get(), toastXml.Get());
			}
		}
		return hr;
	}

	// Create the toast XML from a template
	HRESULT Toast::CreateToastXml(const std::wstring& title, const std::wstring& message, const std::wstring& icon, _In_ IToastNotificationManagerStatics *toastManager, _Outptr_ IXmlDocument** inputXml)
	{
		// Retrieve the template XML
		HRESULT hr = toastManager->GetTemplateContent(ToastTemplateType_ToastImageAndText04, inputXml);
		if (SUCCEEDED(hr))
		{
			wchar_t *imagePath = _wfullpath(nullptr, icon.c_str(), MAX_PATH);

			hr = imagePath != nullptr ? S_OK : HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
			if (SUCCEEDED(hr))
			{
				hr = SetImageSrc(imagePath, *inputXml);
				if (SUCCEEDED(hr))
				{
					wchar_t* textValues[] = {
						(wchar_t*)title.c_str(),
						(wchar_t*)message.c_str()
					};

					UINT32 textLengths[] = { title.size(), message.size() };

					hr = SetTextValues(textValues, 2, textLengths, *inputXml);
				}
			}
		}
		return hr;
	}

	// Set the value of the "src" attribute of the "image" node
	HRESULT Toast::SetImageSrc(_In_z_ wchar_t *imagePath, _In_ IXmlDocument *toastXml)
	{
		wchar_t imageSrc[MAX_PATH] = L"file:///";
		HRESULT hr = StringCchCat(imageSrc, ARRAYSIZE(imageSrc), imagePath);
		if (SUCCEEDED(hr))
		{
			ComPtr<IXmlNodeList> nodeList;
			hr = toastXml->GetElementsByTagName(StringReferenceWrapper(L"image").Get(), &nodeList);
			if (SUCCEEDED(hr))
			{
				ComPtr<IXmlNode> imageNode;
				hr = nodeList->Item(0, &imageNode);
				if (SUCCEEDED(hr))
				{
					ComPtr<IXmlNamedNodeMap> attributes;

					hr = imageNode->get_Attributes(&attributes);
					if (SUCCEEDED(hr))
					{
						ComPtr<IXmlNode> srcAttribute;

						hr = attributes->GetNamedItem(StringReferenceWrapper(L"src").Get(), &srcAttribute);
						if (SUCCEEDED(hr))
						{
							hr = SetNodeValueString(StringReferenceWrapper(imageSrc).Get(), srcAttribute.Get(), toastXml);
						}
					}
				}
			}
		}
		return hr;
	}

	// Set the values of each of the text nodes
	HRESULT Toast::SetTextValues(_In_reads_(textValuesCount) wchar_t **textValues, _In_ UINT32 textValuesCount, _In_reads_(textValuesCount) UINT32 *textValuesLengths, _In_ IXmlDocument *toastXml)
	{
		HRESULT hr = textValues != nullptr && textValuesCount > 0 ? S_OK : E_INVALIDARG;
		if (SUCCEEDED(hr))
		{
			ComPtr<IXmlNodeList> nodeList;
			hr = toastXml->GetElementsByTagName(StringReferenceWrapper(L"text").Get(), &nodeList);
			if (SUCCEEDED(hr))
			{
				UINT32 nodeListLength;
				hr = nodeList->get_Length(&nodeListLength);
				if (SUCCEEDED(hr))
				{
					hr = textValuesCount <= nodeListLength ? S_OK : E_INVALIDARG;
					if (SUCCEEDED(hr))
					{
						for (UINT32 i = 0; i < textValuesCount; i++)
						{
							ComPtr<IXmlNode> textNode;
							hr = nodeList->Item(i, &textNode);
							if (SUCCEEDED(hr))
							{
								hr = SetNodeValueString(StringReferenceWrapper(textValues[i], textValuesLengths[i]).Get(), textNode.Get(), toastXml);
							}
						}
					}
				}
			}
		}
		return hr;
	}

	HRESULT Toast::SetNodeValueString(_In_ HSTRING inputString, _In_ IXmlNode *node, _In_ IXmlDocument *xml)
	{
		ComPtr<IXmlText> inputText;

		HRESULT hr = xml->CreateTextNode(inputString, &inputText);
		if (SUCCEEDED(hr))
		{
			ComPtr<IXmlNode> inputTextNode;

			hr = inputText.As(&inputTextNode);
			if (SUCCEEDED(hr))
			{
				ComPtr<IXmlNode> pAppendedChild;
				hr = node->AppendChild(inputTextNode.Get(), &pAppendedChild);
			}
		}

		return hr;
	}

	// Create and display the toast
	HRESULT Toast::CreateToast(_In_ IToastNotificationManagerStatics *toastManager, _In_ IXmlDocument *xml)
	{
		ComPtr<IToastNotifier> notifier;
		HRESULT hr = toastManager->CreateToastNotifierWithId(StringReferenceWrapper(AppId).Get(), &notifier);
		if (SUCCEEDED(hr))
		{
			ComPtr<IToastNotificationFactory> factory;
			hr = GetActivationFactory(StringReferenceWrapper(RuntimeClass_Windows_UI_Notifications_ToastNotification).Get(), &factory);
			if (SUCCEEDED(hr))
			{
				ComPtr<IToastNotification> toast;
				hr = factory->CreateToastNotification(xml, &toast);
				if (SUCCEEDED(hr))
				{
					// Register the event handlers
					EventRegistrationToken activatedToken, dismissedToken, failedToken;

					ComPtr<ToastEventHandler> handler = m_handler;

					hr = toast->add_Activated(handler.Get(), &activatedToken);
					if (SUCCEEDED(hr))
					{
						hr = toast->add_Dismissed(handler.Get(), &dismissedToken);
						if (SUCCEEDED(hr))
						{
							hr = toast->add_Failed(handler.Get(), &failedToken);
							if (SUCCEEDED(hr))
							{
								hr = notifier->Show(toast.Get());
							}
						}
					}
				}
			}
		}
		return hr;
	}
}}