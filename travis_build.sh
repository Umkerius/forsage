#!/usr/bin/env sh
set -evx
env | sort

mkdir build || true
cd build
cmake -DCMAKE_CXX_FLAGS=$CXX_FLAGS -DCMAKE_BUILD_TYPE=$BUILD_TYPE ..
cmake --build .
ctest -V