#!/bin/bash

# variables
CURRENT_DIR=$(pwd)
BUILD_DIR=$CURRENT_DIR/build
MANGOHUD_CONF_DIR=$CURRENT_DIR/scripts

SHARED=$1

# build
mkdir -p $BUILD_DIR
cp $MANGOHUD_CONF_DIR/MangoHud.conf $BUILD_DIR/MangoHud.conf
cd build
cmake -DBUILD_SHARED_LIBS=$SHARED ..
cmake --build .
