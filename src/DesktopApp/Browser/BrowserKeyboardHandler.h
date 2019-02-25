#pragma once

#include "cef/cef_keyboard_handler.h"

namespace desktop { namespace ui{

	class BrowserKeyboardHandler : public CefKeyboardHandler {
	 public:
	  // Called before a keyboard event is sent to the renderer. |event| contains
	  // information about the keyboard event. |os_event| is the operating system
	  // event message, if any. Return true if the event was handled or false
	  // otherwise. If the event will be handled in OnKeyEvent() as a keyboard
	  // shortcut set |is_keyboard_shortcut| to true and return false.
	  /*--cef()--*/
	  virtual bool OnPreKeyEvent(CefRefPtr<CefBrowser> browser,
								 const CefKeyEvent& evt,
								 CefEventHandle os_event,
								 bool* is_keyboard_shortcut) OVERRIDE;

	   virtual bool OnKeyEvent(CefRefPtr<CefBrowser> browser,
                          const CefKeyEvent& event,
                          CefEventHandle os_event) OVERRIDE;

	protected:
		IMPLEMENT_REFCOUNTING(BrowserKeyboardHandler);
	};

}}