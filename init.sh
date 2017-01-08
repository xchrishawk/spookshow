#!/bin/sh
set -e

# Build directory to use
BUILD_DIR="./build"

# Recreate build directory
rm -rf $BUILD_DIR
mkdir $BUILD_DIR
cd $BUILD_DIR

# Create build system
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..

# Build targets
make spookshow
make spookshow_tests
make spookshow_examples

# Run unit tests
./spookshow_tests
