#!/bin/bash

GENERATE_BUILD_DIR=build/tdlib/generate
BUILD_DIR=build/tdlib
TGPLUS_SOURCE_DIR=$PWD
TDLIB_SOURCE_DIR=libs/vendor/tdlib

function usage
{
    echo "usage: $0 [clean]"
}

function clean
{
    docker run -i -v $TGPLUS_SOURCE_DIR:/home/root/tgplus -w /home/root/tgplus clickable/ubuntu-sdk:16.04-armhf bash <<-EOF
    #!/bin/bash
    echo "TODO: clean gernated files in $TDLIB_SOURCE_DIR"
    rm -rf $BUILD_DIR
EOF
}
    
function build
{
    # apt-get -y install zlib1g-dev libssl-dev gperf
    git submodule update --init --recursive
    mkdir -p $GENERATE_BUILD_DIR
    cd $GENERATE_BUILD_DIR
    cmake -DCMAKE_BUILD_TYPE=Release ../../../$TDLIB_SOURCE_DIR
    cmake --build . --target tl_generate_common
    cmake --build . --target tl_generate_json
    cmake --build . --target tl_generate_c
    cmake --build . --target tdmime_auto
    cd ../../
    docker run -i -v $TGPLUS_SOURCE_DIR:/home/root/tgplus -w /home/root/tgplus clickable/ubuntu-sdk:16.04-armhf bash <<-EOF
    #!/bin/bash
    export MAKEFLAGS=-j1
    apt-get update
    apt-get -y install libssl-dev gperf
    mkdir -p $BUILD_DIR
    cd $BUILD_DIR
    cmake -DTHREADS_PTHREAD_ARG=0 -DCMAKE_CROSSCOMPILING=ON -DCMAKE_BUILD_TYPE=Release ../../$TDLIB_SOURCE_DIR
    cmake --build .
EOF
}


if [ $# -gt 1 ]; then
    usage
    exit 1
fi

if [ $# -eq 1 -a "$1" != "clean" ]; then
    usage
    exit 1
fi

if [ "$1" = "clean" ]; then
    echo "Cleaning..."
    clean
    echo "Done..."
else
    echo "Building..."
    build
    echo "Done..."
fi

