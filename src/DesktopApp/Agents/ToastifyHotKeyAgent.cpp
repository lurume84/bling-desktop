#include "ToastifyHotKeyAgent.h"

#include "Events.h"

#pragma warning(push)
#pragma warning(disable : 4100)
#pragma warning(disable : 4481)
#include <cef/cef_app.h>
#pragma warning(pop)


namespace desktop { namespace ui { namespace agent {

	ToastifyHotKeyAgent::ToastifyHotKeyAgent(CefBrowser& browser)
	: m_browser(browser)
	{
		m_subscriber.subscribe([this](const desktop::core::utils::patterns::Event& rawEvt)
		{
			auto evt = static_cast<const events::ToastifyEvent&>(rawEvt);

			std::stringstream ss;
			ss << "$(document).trigger('toastify', '" << evt.m_code << "');";

			m_browser.GetMainFrame()->ExecuteJavaScript(ss.str(), "", 0);
		}, events::TOASTIFY_EVENT);

	}

	ToastifyHotKeyAgent::~ToastifyHotKeyAgent() = default;
}}}