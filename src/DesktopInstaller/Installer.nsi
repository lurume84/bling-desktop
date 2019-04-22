;NSIS Modern User Interface
;Basic Example Script
;Written by Joost Verburg

;--------------------------------
;Include Bling Desktop

  !include "MUI2.nsh"

!define MUI_ICON "..\..\src\DesktopUI\img\logo2.ico"
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "..\..\src\DesktopUI\img\logo.bmp"
!define MUI_HEADERIMAGE_RIGHT
!define PRODUCT_NAME "Bling Desktop"
!define PRODUCT_VERSION "0.1.10.0"
!define VERSION "0.1.10.0"
!define PRODUCT_PUBLISHER "Luis Ruiz"
!define PRODUCT_WEB_SITE "https://github.com/lurume84/bling-desktop"

VIProductVersion "${VERSION}"
VIFileVersion "${VERSION}"
VIAddVersionKey "ProductName" "Bling Desktop"
VIAddVersionKey "FileVersion" "${VERSION}"
VIAddVersionKey "LegalCopyright" "Copyright (c) 2019 Luis Ruiz"
VIAddVersionKey "FileDescription" "Bling Desktop"
;--------------------------------
;General

  ;Name and file
  Name "Bling Desktop"
  OutFile "../../BlingSetup.exe"

  ;Default installation folder
  InstallDir "$LOCALAPPDATA\Bling Desktop"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\Bling Desktop" ""

  ;Request application privileges for Windows Vista
  RequestExecutionLevel user

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_LICENSE "..\..\LICENSE"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES

  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

Section "Desktop" SecDummy

  SetOutPath "$INSTDIR"
  File /x "*.pdb" /x "*.ipdb" /x "*.iobj" /x "*.lib" "..\..\bin\Release\DesktopApp\*.*"
  
  SetOutPath "$INSTDIR\Html"
  File /r "..\..\bin\Release\DesktopApp\Html\loading"

  SetOutPath "$INSTDIR\locales"
  File /r "..\..\bin\Release\DesktopApp\locales\*.*"

  SetOutPath "$INSTDIR\swiftshader"
  File /r "..\..\bin\Release\DesktopApp\swiftshader\*.dll"

  ; Create application shortcut (first in installation dir to have the correct "start in" target)
  SetOutPath "$INSTDIR"
  CreateShortCut "$INSTDIR\${PRODUCT_NAME}.lnk" "$INSTDIR\Bling.exe"

  ; Start menu entries
  SetOutPath "$SMPROGRAMS\${PRODUCT_NAME}\"
  CopyFiles "$INSTDIR\${PRODUCT_NAME}.lnk" "$SMPROGRAMS\${PRODUCT_NAME}\"
  CopyFiles "$INSTDIR\${PRODUCT_NAME}.lnk" "$desktop"
  Delete "$INSTDIR\${PRODUCT_NAME}.lnk"

  ;Store installation folder
  WriteRegStr HKCU "Software\Bling Desktop" "" $INSTDIR
  
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"

SectionEnd

;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_SecDummy ${LANG_ENGLISH} "Desktop section."

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecDummy} $(DESC_SecDummy)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
;Uninstaller Section

Section "Uninstall"

  ;ADD YOUR OWN FILES HERE...

  Delete "$INSTDIR\Uninstall.exe"

  RMDir /r "$INSTDIR"

  DeleteRegKey /ifempty HKCU "Software\Bling Desktop"

SectionEnd