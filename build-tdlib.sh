#!/bin/bash

set -ex

TGPLUS_SOURCE_DIR=$PWD
TDLIB_SOURCE_DIR=libs/vendor/tdlib

DO_CLEAN=0
ARCH="armhf"
ARCH_TRIPLET="arm-linux-gnueabihf"

DOCKER_COMMAND=""
JOBS="1"
SUDO=""

function usage
{
    echo "usage: $0 [arch] [clean]"
}

function clean
{
    $DOCKER_COMMAND bash <<-EOF
    #!/bin/bash
    echo "TODO: clean generated files in $TDLIB_SOURCE_DIR"
    rm -rf $BUILD_DIR
EOF
}

function build
{
    # apt-get -y install zlib1g-dev libssl-dev gperf
    git submodule update --init --recursive
    mkdir -p $GENERATE_BUILD_DIR
    cd $GENERATE_BUILD_DIR
    cmake -DCMAKE_BUILD_TYPE=Release $TGPLUS_SOURCE_DIR/$TDLIB_SOURCE_DIR
    cmake --build . --target tl_generate_common
    cmake --build . --target tl_generate_json
    cmake --build . --target tl_generate_c
    cmake --build . --target tdmime_auto
    cd $TGPLUS_SOURCE_DIR
    $DOCKER_COMMAND bash <<-EOF
    #!/bin/bash
    export MAKEFLAGS=-j$JOBS
    $SUDO apt-get update
    $SUDO apt-get -y install libssl-dev gperf
    mkdir -p $BUILD_DIR
    cd $BUILD_DIR
    cmake -DTHREADS_PTHREAD_ARG=0 -DCMAKE_CROSSCOMPILING=ON -DCMAKE_BUILD_TYPE=Release $TGPLUS_SOURCE_DIR/$TDLIB_SOURCE_DIR
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
                ARCH_TRIPLET="x86_64-linux-gnu"
                ;;
            "armhf")
                ARCH="armhf"
                ARCH_TRIPLET="arm-linux-gnueabihf"
                ;;
            "host")
                ARCH="host"
                ARCH_TRIPLET=$(dpkg-architecture -q DEB_HOST_MULTIARCH)
                ;;

            *)
                return 1
        esac
    done

    if [ "$ARCH" == "host" ]; then
        DOCKER_COMMAND=""
        JOBS="4"
        SUDO="sudo"
    fi

    DOCKER_COMMAND="docker run -i -v $TGPLUS_SOURCE_DIR:/home/root/tgplus -w /home/root/tgplus clickable/ubuntu-sdk:16.04-$ARCH"

    return 0
}

parse $*

GENERATE_BUILD_DIR=build/tdlib/$ARCH_TRIPLET/generate
BUILD_DIR=build/tdlib/$ARCH_TRIPLET

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
