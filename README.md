# Telegram Plus

## Building
Install [clickable](clickable.bhdouglass.com/en/latest/) and the following packages:

    sudo apt-get install gperf libssl-dev zlib1g-dev

Then run once:

    ./build-tdlib-in-ubuntu-sdk-docker.sh

From now on you can build the app with:

    clickable

## Running on desktop

To run the app on desktop, clean the tdlib if you've built it for armhf before and build it for amd64 once:

    ./build-tdlib-in-ubuntu-sdk-docker.sh amd64

Then you can build and run the app using clickable as usual:

    clickable desktop

If you get a `telegram-plus: error while loading shared libraries: libQt5QuickControls2.so.5: cannot open shared object file: No such file or directory`, 
remove the `.clickable` directory and try again. This usually happens when
switching between phone and desktop build.

## Cleaning the tdlib

You can clean the tdlib build by:

    ./build-tdlib-in-ubuntu-sdk-docker.sh clean

## Known issues

* No media attachments or photos are supported right now
* During login you need to resize the window after telephone number input or code input if you see a white box only. On the phone: *Rotate* the phone briefly to get the window resized
* No notifications
* Messages are not always refreshing, you have to back and forth between chat list and message window
* Messages are not being marked as read

## Logging out
**Note:** At the current stage it is adviseable to log out again from the App, especially if you are working on the Telegram test server. You can do this in the Account page.


