#!/usr/bin/env sh
set -evx
env | sort

mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE -Dadditional_cxx_flags=$CXX_FLAGS ..
cmake --build .
ctest -V