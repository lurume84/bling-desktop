#include "stdafx.h"

#include "BrowserKeyboardHandler.h"

#include "Constants.h"

namespace desktop { namespace ui {

	#define REQUIRE_UI_THREAD()   ASSERT(CefCurrentlyOn(TID_UI));

	bool BrowserKeyboardHandler::OnPreKeyEvent(CefRefPtr<CefBrowser> browser, const CefKeyEvent& /*evt*/, CefEventHandle os_event, bool* /*is_keyboard_shortcut*/)
	{
		if(os_event != NULL)
		{
			if(os_event->wParam == VK_F10)
			{
				return true;
			}
			else if (os_event->wParam == VK_ESCAPE)
			{
				browser->GetMainFrame()->ExecuteJavaScript("document.exitFullscreen();", browser->GetMainFrame()->GetURL(), 0);
				return true;
			}
			else
			{
				HWND hWindow = GetParent( browser->GetHost()->GetWindowHandle() );

				return ::SendMessage( hWindow, wm::browser::CEF_PRE_KEYBOARD_HOOK, NULL, (LPARAM)os_event ) == TRUE;
			}
		}
		else
		{
			return false;
		}
	}

	bool BrowserKeyboardHandler::OnKeyEvent(CefRefPtr<CefBrowser> browser, const CefKeyEvent& /*evt*/, CefEventHandle os_event)
	{
		return false;
	}
}}

