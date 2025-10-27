#!/bin/bash
set -e

echo "--- Configuring Linux Debug Build ---"

cmake -B build-linux-debug \
      -D CMAKE_BUILD_TYPE=Debug \
      -S .

echo "--- Building Linux Debug ---"
cmake --build build-linux-debug