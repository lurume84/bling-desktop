#include "stdafx.h"

#include "NotificationAgent.h"

#include "BlingCore\Upgrade\Events.h"

#include "Toast/StringReferenceWrapper.h"
#include "Toast/ToastEventHandler.h"
#include "Toast/Toast.h"

#include <locale>
#include <codecvt>

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

using namespace Microsoft::WRL;
using namespace ABI::Windows::UI::Notifications;
using namespace ABI::Windows::Data::Xml::Dom;
using namespace Windows::Foundation;

namespace bling { namespace app {  namespace agent {

	NotificationAgent::NotificationAgent()
	{
		Windows::Foundation::Initialize(RO_INIT_MULTITHREADED);
	}

	NotificationAgent::~NotificationAgent()
	{
		Windows::Foundation::Uninitialize();
	}

	// Prepare the main window
	bool NotificationAgent::initialize()
	{
		return SUCCEEDED(TryCreateShortcut());
	}
	// In order to display toasts, a desktop application must have a shortcut on the Start menu.
	// Also, an AppUserModelID must be set on that shortcut.
	// The shortcut should be created as part of the installer. The following code shows how to create
	// a shortcut and assign an AppUserModelID using Windows APIs. You must download and include the 
	// Windows API Code Pack for Microsoft .NET Framework for this code to function
	//
	// Included in this project is a wxs file that be used with the WiX toolkit
	// to make an installer that creates the necessary shortcut. One or the other should be used.

	HRESULT NotificationAgent::TryCreateShortcut()
	{
		wchar_t shortcutPath[MAX_PATH];
		DWORD charWritten = GetEnvironmentVariable(L"APPDATA", shortcutPath, MAX_PATH);
		HRESULT hr = charWritten > 0 ? S_OK : E_INVALIDARG;

		if (SUCCEEDED(hr))
		{
			errno_t concatError = wcscat_s(shortcutPath, ARRAYSIZE(shortcutPath), L"\\Microsoft\\Windows\\Start Menu\\Programs\\BlingDesktop.lnk");
			hr = concatError == 0 ? S_OK : E_INVALIDARG;
			if (SUCCEEDED(hr))
			{
				DWORD attributes = GetFileAttributes(shortcutPath);
				bool fileExists = attributes < 0xFFFFFFF;

				if (!fileExists)
				{
					hr = InstallShortcut(shortcutPath);
				}
				else
				{
					hr = S_FALSE;
				}
			}
		}
		return hr;
	}

	// Install the shortcut
	HRESULT NotificationAgent::InstallShortcut(_In_z_ wchar_t *shortcutPath)
	{
		wchar_t exePath[MAX_PATH];

		DWORD charWritten = GetModuleFileNameEx(GetCurrentProcess(), nullptr, exePath, ARRAYSIZE(exePath));

		HRESULT hr = charWritten > 0 ? S_OK : E_FAIL;

		if (SUCCEEDED(hr))
		{
			ComPtr<IShellLink> shellLink;
			hr = CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&shellLink));

			if (SUCCEEDED(hr))
			{
				hr = shellLink->SetPath(exePath);
				if (SUCCEEDED(hr))
				{
					hr = shellLink->SetArguments(L"");
					if (SUCCEEDED(hr))
					{
						ComPtr<IPropertyStore> propertyStore;

						hr = shellLink.As(&propertyStore);
						if (SUCCEEDED(hr))
						{
							PROPVARIANT appIdPropVar;
							hr = InitPropVariantFromString(AppId, &appIdPropVar);
							if (SUCCEEDED(hr))
							{
								hr = propertyStore->SetValue(PKEY_AppUserModel_ID, appIdPropVar);
								if (SUCCEEDED(hr))
								{
									hr = propertyStore->Commit();
									if (SUCCEEDED(hr))
									{
										ComPtr<IPersistFile> persistFile;
										hr = shellLink.As(&persistFile);
										if (SUCCEEDED(hr))
										{
											hr = persistFile->Save(shortcutPath, TRUE);
										}
									}
								}
								PropVariantClear(&appIdPropVar);
							}
						}
					}
				}
			}
		}
		return hr;
	}

}}}