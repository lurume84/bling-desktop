// Copyright (c) 2015 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include <windows.h>

#include "include/base/cef_scoped_ptr.h"
#include "include/cef_command_line.h"
#include "include/cef_sandbox_win.h"
#include "browser/main_context_impl.h"
#include "browser/main_message_loop_multithreaded_win.h"
#include "browser/root_window_manager.h"
#include "browser/test_runner.h"
#include "browser/client_app_browser.h"
#include "browser/main_message_loop_external_pump.h"
#include "browser/main_message_loop_std.h"
#include "common/client_app_other.h"
#include "common/client_switches.h"
#include "renderer/client_app_renderer.h"

#include "DesktopCore\DesktopCore.h"
#include "Events.h"
#include "DesktopCore\Utils\Patterns\PublisherSubscriber\Subscriber.h"
#include "DesktopCore\System\Services\CrashReportService.h"
#include "DesktopCore\Upgrade\Agents\UpgradeViewerAgent.h"
#include "DesktopCore\Upgrade\Agents\UpgradeDesktopAgent.h"
#include "DesktopCore\Upgrade\Events.h"
#include "DesktopCore\Network\Agents\FileServerAgent.h"
#include "DesktopCore\Blink\Agents\SyncVideoAgent.h"
#include "DesktopCore\Blink\Agents\SyncThumbnailAgent.h"
#include "DesktopCore\Blink\Agents\LiveViewAgent.h"
#include "DesktopCore\Blink\Agents\ActivityAgent.h"
#include "DesktopCore\System\Agents\LogAgent.h"
#include "DesktopCore\System\Services\LogService.h"
#include "Services\DownloadViewerService.h"
#include "Services\DownloadDesktopService.h"

// When generating projects with CMake the CEF_USE_SANDBOX value will be defined
// automatically if using the required compiler version. Pass -DUSE_SANDBOX=OFF
// to the CMake command-line to disable use of the sandbox.
// Uncomment this line to manually enable sandbox support.
// #define CEF_USE_SANDBOX 1

#if defined(CEF_USE_SANDBOX)
// The cef_sandbox.lib static library may not link successfully with all VS
// versions.
#pragma comment(lib, "cef_sandbox.lib")
#endif

namespace client {
namespace {

int RunMain(HINSTANCE hInstance, int nCmdShow) 
{
	desktop::core::service::CrashReportService service;
	service.initialize({});

  // Enable High-DPI support on Windows 7 or newer.
  CefEnableHighDPISupport();

  CefMainArgs main_args(hInstance);

  void* sandbox_info = NULL;

#if defined(CEF_USE_SANDBOX)
  // Manage the life span of the sandbox information object. This is necessary
  // for sandbox support on Windows. See cef_sandbox_win.h for complete details.
  CefScopedSandboxInfo scoped_sandbox;
  sandbox_info = scoped_sandbox.sandbox_info();
#endif

  // Parse command-line arguments.
  CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();
  command_line->InitFromString(::GetCommandLineW());

  command_line->AppendSwitch(switches::kUseViews);
  command_line->AppendSwitch(switches::kHideFrame);
  command_line->AppendSwitch(switches::kHideControls);
  command_line->AppendSwitch(switches::kExternalMessagePump);
  command_line->AppendSwitchWithValue("disable-web-security", "true");

  // Create a ClientApp of the correct type.
  CefRefPtr<CefApp> app;
  ClientApp::ProcessType process_type = ClientApp::GetProcessType(command_line);
  if (process_type == ClientApp::BrowserProcess)
    app = new ClientAppBrowser();
  else if (process_type == ClientApp::RendererProcess)
    app = new ClientAppRenderer();
  else if (process_type == ClientApp::OtherProcess)
    app = new ClientAppOther();

  // Execute the secondary process, if any.
  int exit_code = CefExecuteProcess(main_args, app, sandbox_info);
  if (exit_code >= 0)
    return exit_code;
  
  // Create the main context object.
  scoped_ptr<MainContextImpl> context(new MainContextImpl(command_line, true));

  CefSettings settings;
  settings.remote_debugging_port = 8088;
  //CefString(&settings.user_agent).FromString("BlingBrowser");

  settings.external_message_pump = true;

#if !defined(CEF_USE_SANDBOX)
  settings.no_sandbox = true;
#endif

  // Populate the settings based on command line arguments.
  context->PopulateSettings(&settings);

  // Create the main message loop object.
  scoped_ptr<MainMessageLoop> message_loop;
  if (settings.multi_threaded_message_loop)
    message_loop.reset(new MainMessageLoopMultithreadedWin);
  else if (settings.external_message_pump)
    message_loop = MainMessageLoopExternalPump::Create();
  else
    message_loop.reset(new MainMessageLoopStd);

  // Initialize CEF.
  context->Initialize(main_args, settings, app, sandbox_info);

  // Register scheme handlers.
  test_runner::RegisterSchemeHandlers();

  RootWindowConfig window_config;
  window_config.always_on_top = command_line->HasSwitch(switches::kAlwaysOnTop);
  window_config.with_controls =
      !command_line->HasSwitch(switches::kHideControls);
  window_config.with_osr = settings.windowless_rendering_enabled ? true : false;
  
  {
	  desktop::core::service::ApplicationDataService applicationService;

	  if (boost::filesystem::exists(applicationService.getViewerFolder() + "/index.html"))
	  {
		  desktop::core::service::IniFileService iniFileService;

		  window_config.url = iniFileService.get<std::string>(applicationService.getMyDocuments() + "Bling.ini", "FileServer", "Endpoint", "http://127.0.0.1:9191/");
	  }
	  else
	  {
		  window_config.url = boost::filesystem::canonical("Html/loading/index.html").string();
	  }
  }

  desktop::core::DesktopCore core;

  core.initialize();

  core.addAgent(std::make_unique<desktop::core::agent::LogAgent>());
  core.addAgent(std::make_unique<desktop::core::agent::SyncVideoAgent>());
  core.addAgent(std::make_unique<desktop::core::agent::SyncThumbnailAgent>());
  core.addAgent(std::make_unique<desktop::core::agent::LiveViewAgent>());
  core.addAgent(std::make_unique<desktop::core::agent::FileServerAgent>());

  desktop::core::utils::patterns::Subscriber subscriber;
  subscriber.subscribe([&core, &subscriber](const desktop::core::utils::patterns::Event& rawEvt)
  {
	  const auto& evt = static_cast<const desktop::ui::events::BrowserCreatedEvent&>(rawEvt);

	  auto &browser = evt.m_browser;
	  
	  desktop::core::service::LogService::info("Browser instance created");

      core.addAgent(std::make_unique<desktop::core::agent::UpgradeViewerAgent>(std::make_unique<desktop::ui::service::DownloadViewerService>(browser)));
	  core.addAgent(std::make_unique<desktop::core::agent::UpgradeDesktopAgent>(std::make_unique<desktop::ui::service::DownloadDesktopService>(browser)));
      
	  subscriber.unsubscribe(desktop::ui::events::BROWSER_CREATED_EVENT);

  }, desktop::ui::events::BROWSER_CREATED_EVENT);

  // Create the first window.
  context->GetRootWindowManager()->CreateRootWindow(window_config);

  // Run the message loop. This will block until Quit() is called by the
  // RootWindowManager after all windows have been destroyed.
  int result = message_loop->Run();

  // Shut down CEF.
  context->Shutdown();

  // Release objects in reverse order of creation.
  message_loop.reset();
  context.reset();

  return result;
}

}  // namespace
}  // namespace client

// Program entry point function.
int APIENTRY wWinMain(HINSTANCE hInstance,
                      HINSTANCE hPrevInstance,
                      LPTSTR lpCmdLine,
                      int nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);
  return client::RunMain(hInstance, nCmdShow);
}
