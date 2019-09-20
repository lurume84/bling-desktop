// Copyright (c) 2015 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "browser/main_context_impl.h"

#include <direct.h>
#include <shlobj.h>

#include "DesktopCore\System\Services\ApplicationDataService.h"

#include <boost\filesystem\path.hpp>

namespace client {

std::string MainContextImpl::GetDownloadPath(const std::string& file_name, bool& showDialog) 
{
  std::string path;

  boost::filesystem::path p(file_name);

  if (p.extension() == ".zip")
  {
	  desktop::core::service::ApplicationDataService service;
	  auto documents = service.getMyDocuments();
	  path = documents + "Download\\Versions\\" + p.filename().string();

	  showDialog = false;
  }
  else
  {
	  TCHAR szFolderPath[MAX_PATH];
	  
	  // Save the file in the user's "My Documents" folder.
	  if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL | CSIDL_FLAG_CREATE, NULL,
									0, szFolderPath))) {
		path = CefString(szFolderPath);
		path += "\\" + file_name;
	  }

	  showDialog = true;
  }
  
  return path;
}

std::string MainContextImpl::GetAppWorkingDirectory() {
  char szWorkingDir[MAX_PATH + 1];
  if (_getcwd(szWorkingDir, MAX_PATH) == NULL) {
    szWorkingDir[0] = 0;
  } else {
    // Add trailing path separator.
    size_t len = strlen(szWorkingDir);
    szWorkingDir[len] = '\\';
    szWorkingDir[len + 1] = 0;
  }
  return szWorkingDir;
}

}  // namespace client
