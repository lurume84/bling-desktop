# Bling desktop
This application is the container of [Bling Viewer](https://github.com/lurume84/bling-viewer) project. It is a chrome-based application (CEF) that loads viewer seamlessly and provides local storage capabilities.

## What it does

* Disables CORS to allow connection with Blink Servers
* Upgrades to last official Bling Viewer automatically
* Synchronize all videos in local storage
* Periodic snapshots synchronized in local storage

## What it will do

* Notify user of movement using Windows Toast Notifications (WIP)
* Live View

## Installation Guide
It is a standalone application, just uncompress release and execute BlingApp.exe. All viewers are downloaded in Download/Versions and persisted forever to keep trace of the last available release.

## Development Guide
You need vs2017. Checkout and uncompress third_party.7z in the same folder.

## Compatibility
Only Windows 10 is supported at this time due to Toast Notifications. Windows 7 may be supported in the future.
