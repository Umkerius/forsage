#!/usr/bin/env sh
set -evx
env | sort

echo $(pwd)
mkdir build || true
echo $(pwd)
cd build
echo $(pwd)
cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE ..
cmake --build .
ctest -V