#include "stdafx.h"

// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "simple_handler.h"

#include <sstream>
#include <string>

#include "include/cef/base/cef_bind.h"
#include "include/cef/cef_app.h"
#include "include/cef/views/cef_browser_view.h"
#include "include/cef/views/cef_window.h"
#include "include/cef/wrapper/cef_closure_task.h"
#include "include/cef/wrapper/cef_helpers.h"

#include "BlinkCore\Upgrade\Events.h"
#include "Toast\ToastEventHandler.h"
#include "Toast\Toast.h"

#include <boost\filesystem\operations.hpp>

namespace
{
	std::wstring utf8toUtf16(const std::string & str)
	{
		if (str.empty())
			return std::wstring();

		size_t charsNeeded = ::MultiByteToWideChar(CP_UTF8, 0,
			str.data(), (int)str.size(), NULL, 0);
		if (charsNeeded == 0)
			throw std::runtime_error("Failed converting UTF-8 string to UTF-16");

		std::vector<wchar_t> buffer(charsNeeded);
		int charsConverted = ::MultiByteToWideChar(CP_UTF8, 0,
			str.data(), (int)str.size(), &buffer[0], buffer.size());
		if (charsConverted == 0)
			throw std::runtime_error("Failed converting UTF-8 string to UTF-16");

		return std::wstring(&buffer[0], charsConverted);
	}

	SimpleHandler* g_instance = NULL;

}  // namespace

SimpleHandler::SimpleHandler(bool use_views)
    : use_views_(use_views), is_closing_(false)
{
  DCHECK(!g_instance);
  g_instance = this;
  
  m_subscriber.subscribe([this](const blink::core::utils::patterns::Event& rawEvt)
  {
	  auto evt = static_cast<const blink::core::events::UpgradeCompletedEvent&>(rawEvt);

	  auto version = utf8toUtf16(evt.m_version);

	  SimpleHandler* self = this;
	  
	  ToastEventHandler* handler = new ToastEventHandler([&self]()
	  {
		  for (auto &browser : self->browser_list_)
		  {
			  browser->GetMainFrame()->LoadURL(boost::filesystem::canonical("Html/viewer/index.html").string());
		  }

		  return true;
	  }, []() {return true; }, []() {return true; });

	  blink::app::Toast toast(handler);
	  toast.DisplayToast(L"Version " + version, L"Succesfully installed new version, click to refresh", L"icon");

  }, blink::core::events::UPGRADE_COMPLETED_EVENT);
}

SimpleHandler::~SimpleHandler() {
  g_instance = NULL;
}

// static
SimpleHandler* SimpleHandler::GetInstance() {
  return g_instance;
}

void SimpleHandler::OnTitleChange(CefRefPtr<CefBrowser> browser,
                                  const CefString& title) {
  CEF_REQUIRE_UI_THREAD();

  if (use_views_) {
    // Set the title of the window using the Views framework.
    CefRefPtr<CefBrowserView> browser_view =
        CefBrowserView::GetForBrowser(browser);
    if (browser_view) {
      CefRefPtr<CefWindow> window = browser_view->GetWindow();
      if (window)
        window->SetTitle(title);
    }
  } else {
    // Set the title of the window using platform APIs.
    PlatformTitleChange(browser, title);
  }
}

void SimpleHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
  CEF_REQUIRE_UI_THREAD();

  // Add to the list of existing browsers.
  browser_list_.push_back(browser);
}

bool SimpleHandler::DoClose(CefRefPtr<CefBrowser> browser) {
  CEF_REQUIRE_UI_THREAD();

  // Closing the main window requires special handling. See the DoClose()
  // documentation in the CEF header for a detailed destription of this
  // process.
  if (browser_list_.size() == 1) {
    // Set a flag to indicate that the window close should be allowed.
    is_closing_ = true;
  }

  // Allow the close. For windowed browsers this will result in the OS close
  // event being sent.
  return false;
}

void SimpleHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
  CEF_REQUIRE_UI_THREAD();

  // Remove from the list of existing browsers.
  BrowserList::iterator bit = browser_list_.begin();
  for (; bit != browser_list_.end(); ++bit) {
    if ((*bit)->IsSame(browser)) {
      browser_list_.erase(bit);
      break;
    }
  }

  if (browser_list_.empty()) {
    // All browser windows have closed. Quit the application message loop.
    CefQuitMessageLoop();
  }
}

void SimpleHandler::OnLoadError(CefRefPtr<CefBrowser> browser,
                                CefRefPtr<CefFrame> frame,
                                ErrorCode errorCode,
                                const CefString& errorText,
                                const CefString& failedUrl) {
  CEF_REQUIRE_UI_THREAD();

  // Don't display an error for downloaded files.
  if (errorCode == ERR_ABORTED)
    return;

  // Display a load error message.
  std::stringstream ss;
  ss << "<html><body bgcolor=\"white\">"
        "<h2>Failed to load URL "
     << std::string(failedUrl) << " with error " << std::string(errorText)
     << " (" << errorCode << ").</h2></body></html>";
  frame->LoadString(ss.str(), failedUrl);
}

void SimpleHandler::CloseAllBrowsers(bool force_close) {
  if (!CefCurrentlyOn(TID_UI)) {
    // Execute on the UI thread.
    CefPostTask(TID_UI, base::Bind(&SimpleHandler::CloseAllBrowsers, this,
                                   force_close));
    return;
  }

  if (browser_list_.empty())
    return;

  BrowserList::const_iterator it = browser_list_.begin();
  for (; it != browser_list_.end(); ++it)
    (*it)->GetHost()->CloseBrowser(force_close);
}

void SimpleHandler::PlatformTitleChange(CefRefPtr<CefBrowser> browser,
	const CefString& title) {
	CefWindowHandle hwnd = browser->GetHost()->GetWindowHandle();
	SetWindowText(hwnd, std::wstring(title).c_str());
}