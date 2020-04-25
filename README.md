[![Build Status AppVeyor](https://ci.appveyor.com/api/projects/status/github/lurume84/bling-desktop?svg=true)](https://ci.appveyor.com/project/lurume84/bling-desktop)

# Bling desktop
This application is the container of [Bling Viewer](https://github.com/lurume84/bling-viewer) project. It is a chrome-based application (CEF) that loads viewer seamlessly and provides local storage capabilities.

## What it does

* Live View (except XT2 cameras)
* Synchronize all videos in local storage
* Periodic snapshots synchronized in local storage
* Upgrades to last official Bling Viewer and Desktop automatically
* Disables CORS to allow connection with Blink Servers

## Installation instructions
Follow [this guide](INSTALLATION.md)

## Configuration instructions
Follow [this guide](CONFIGURATION.md)

## Development
You need vs2017 and Conan. Checkout and execute build.bat in the same folder.

## Compatibility
Windows 10 and 7 is fully supported at this time.
