[![Build Status AppVeyor](https://ci.appveyor.com/api/projects/status/github/lurume84/bling-desktop?svg=true)](https://ci.appveyor.com/project/lurume84/bling-desktop)

# Bling desktop
This application is the container of [Bling Viewer](https://github.com/lurume84/bling-viewer) project. It is a chrome-based application (CEF) that loads viewer seamlessly and provides local storage capabilities.

## What it does

* Live View
* Synchronize all videos in local storage
* Periodic snapshots synchronized in local storage
* Upgrades to last official Bling Viewer automatically
* Disables CORS to allow connection with Blink Servers

## What it will do

* Notify user of movement using Windows Toast Notifications (WIP)

## Installation
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

### Blink.ini
This is the file you need to modify to configure everything related with Blink Servers. It is located in %userprofile%/Documents/Bling.exe/ but does not exist by default, you have to manually create it.

* SyncVideo
  * Enabled: By default this is enabled. It tells the application to poll Blink servers for new videos
  * UseLocalTime: By default this is disabled. Saves each video in your computer's timezone instead of UTC.
  * Interval: By default this is 60 seconds. The time to sleep until checking again Blink servers. Do not put a small value to avoid flooding Blink servers. Since desktop is thought to be always opened a check each minute is more than enough.
  * Sleep: By default this is 20 seconds. The time to sleep between each video download. This means 3 videos per minute. Again, do not put a small value here.
  * Output: By default this is %userprofile%/Documents/Download/Videos. The folder where videos will be downloaded. Put any path you want, even network locations should work. In case they give you problems, map them in Windows so they can be accessed by a drive letter.
  * LastUpdate: This is automatically generated. It is the timestamp of the last successful video download. It is used to speed up video downloads so we know when last video was downloaded. In case you delete videos folder you will have to remove this value too.
* SyncThumbnail
  * Enabled: By default this is disabled. It tells the application to poll Blink servers for a new snapshot for each camera.
  * Interval: By default this is 3600 seconds (1 hour). The time to sleep until polling again Blink servers. Do not put a small value to avoid flooding Blink servers
  * Sleep: By default this is 5 seconds. The time to sleep until checking again if snapshot has been successful.
  * Retries: By default this is 10. Number of times to check if snapshot was successful. Combined with sleep, this means we will wait 50 seconds to consider snapshot was unsuccessful.
  * Output: By default this is %userprofile%/Documents/Download/Thumbnails.
* LiveView
  * Output: By default this is %userprofile%/Documents/Download/Videos.
  * UseLocalTime: By default this is disabled. Saves each video in your computer's timezone instead of UTC.
  * Endpoint: By default this is http://127.0.0.1:9192. Change port in case you have another application using it.

* Example

[SyncVideo]

Enabled=true

UseLocalTime=false

Interval=60

Sleep=20

Output=D:\Videos


[SyncThumbnail]

Enabled=false

Interval=3600

Sleep=5

Retries=10

Output=D:\Thumbnails

### Bling.ini
This is the file you need to modify to configure Bling application. It is located in %userprofile%/Documents/Bling.exe/ but does not exist by default, you have to manually create it.

* FileServer
  * Endpoint: By default this is http://127.0.0.1:9191/. Used for serving viewer files
  * Host: By default this is 127.0.0.1. Same as above but splitted.
  * Port: By default this is 9191. Same as above but splitted.

* Log
  * Enabled: By default this is true. Created a Bling.log file inside Logs folder and logs application activity
  
* UpgradeDesktop
  * Enabled: By default this is true. Application polls this github to download latest release
  * Host: By default this is api.github.com.
  * Repository: By default this is /repos/lurume84/bling-desktop/releases/latest
  * Input: By default this is Download\\Versions\\Desktop\\
  
* UpgradeViewer
  * Enabled: By default this is true. Application polls viewer's github to download latest release
  * Host: By default this is api.github.com.
  * Repository: By default this is /repos/lurume84/bling-viewer/releases/latest
  * Input: By default this is Download\\Versions\\Viewer\\  

* Example

[UpgradeDesktop]
Enabled=false
[UpgradeViewer]
Enabled=false

## Development Guide
You need vs2017. Checkout and uncompress third_party.7z in the same folder.

## Compatibility
Only Windows 10 is fully supported at this time due to Toast Notifications. Windows 7 also works but without notifications system. In practice, this only means you need to manually update viewer by deleting %userprofile%/Documents/Html/viewer folder and starting application again.
