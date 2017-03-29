#!/usr/bin/env sh
set -evx
env | sort

mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE ..
cmake --build .
ctest -V