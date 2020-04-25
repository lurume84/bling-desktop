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

## Folder structure
This application automatically creates folder %userprofile%/Documents/Bling.exe. This folder contains all user-related information that is described below

### CrashReports
In case application crashes, a dump will be generated inside this folder. If you want to contribute to its resolution send it to me.

### Download
Videos, Thumbnails and github releases will be downloaded here. To force a redownload of viewer just delete versions folder and open application again.

### Html
Contains the downloaded viewer. This is automatically stepped over by viewer updates, also contains your connection token. In case you want to remove credentials remove token.json file.

## Configuration
Follow [this guide](CONFIGURATION.md)

## Development Guide
You need vs2017. Checkout and uncompress third_party.7z in the same folder.

## Compatibility
Windows 10 and 7 is fully supported at this time.
