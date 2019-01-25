#!/bin/bash

# This must be executed in the amd64 container regardless of the target arch

set -ex

function prebuild
{
    mkdir -p $BUILD_DIR_GENERATE
    cd $BUILD_DIR_GENERATE

    cmake -DCMAKE_BUILD_TYPE=Release $TDLIB_SOURCE
    cmake --build . --target tl_generate_common
    cmake --build . --target tl_generate_json
    cmake --build . --target tl_generate_c
    cmake --build . --target tdmime_auto
}

# Switch to tg-plus root dir
TGPLUS_SOURCE_DIR="$( cd "$(dirname "$0")" ; pwd -P )"
TDLIB_SOURCE=$TGPLUS_SOURCE_DIR/libs/vendor/tdlib

# Build for amd64
BUILD_DIR_GENERATE=$TGPLUS_SOURCE_DIR/build/tdlib/x86_64-linux-gnu/generate
prebuild

# Build for armhfs
BUILD_DIR_GENERATE=$TGPLUS_SOURCE_DIR/build/tdlib/arm-linux-gnueabihf/generate
prebuild
