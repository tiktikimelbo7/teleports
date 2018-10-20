#!/bin/bash

GENERATE_BUILD_DIR=build/tdlib/generate
BUILD_DIR=build/tdlib
TGPLUS_SOURCE_DIR=$PWD
TDLIB_SOURCE_DIR=libs/vendor/tdlib

DO_CLEAN=0
ARCH="armhf"

function usage
{
    echo "usage: $0 [arch] [clean]"
}

function clean
{
    docker run -i -v $TGPLUS_SOURCE_DIR:/home/root/tgplus -w /home/root/tgplus clickable/ubuntu-sdk:16.04-$ARCH bash <<-EOF
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
    docker run -i -v $TGPLUS_SOURCE_DIR:/home/root/tgplus -w /home/root/tgplus clickable/ubuntu-sdk:16.04-$ARCH bash <<-EOF
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

function parse
{
    if [ $# -gt 2 ]; then
        echo "Too many parameters..."
        return 1
    fi

    if [ $# -eq 2 -a "$1" != "clean" -a "$2" != "clean" ]; then
        return 1
    fi

    for param in $*; do
        case $param in
            "clean")
                DO_CLEAN=1
                ;;
            "amd64")
                ARCH="amd64"
                ;;
            "armhf")
                ;;
            *)
                return 1
        esac
    done

    return 0
}

parse $* 

if [ $? -eq 1 ]; then
    usage
    exit 1
fi

if [ $DO_CLEAN -eq 1 ]; then
    echo "Cleaning..."
    clean
    echo "Done..."
else
    echo "Building..."
    build
    echo "Done..."
fi

