#!/bin/sh

mkdir -p build && cd build
cmake -DCMAKE_CXX_COMPILER=clang++ ..
make -j4
