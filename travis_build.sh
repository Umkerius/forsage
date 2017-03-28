#!/bin/bash

echo "Compiler: `$COMPILER --version`"

mkdir build
cd build
cmake ..
cmake --build . 