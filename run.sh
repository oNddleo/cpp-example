#!/bin/bash
PROJECT_NAME=hello


rm -rf build
mkdir build && cd build
cmake -DCMAKE_C_COMPILER='/opt/homebrew/opt/llvm/bin/clang' -G Ninja ..
ninja
./$PROJECT_NAME