$version=$args[0]
$fileversion=$version.replace(".",",")

echo ("Changing file version to " + $version)
((Get-Content -path .\src\DesktopApp\resources\win\resources.rc -Raw) -replace '0,0,0,0', ($fileversion + ',0') -replace '0.0.0.0', ($version + '.0')) | Set-Content -Path .\src\DesktopApp\resources\win\resources.rc
((Get-Content -path .\src\DesktopInstaller\Installer.nsi -Raw) -replace '0.0.0.0', ($version + '.0')) | Set-Content -Path .\src\DesktopInstaller\Installer.nsi