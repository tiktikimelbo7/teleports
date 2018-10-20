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

## Cleaning the tdlib

You can clean the tdlib build by:

    ./build-tdlib-in-ubuntu-sdk-docker.sh clean
