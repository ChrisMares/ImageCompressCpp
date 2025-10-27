#!/bin/bash
set -e

echo "--- Configuring Windows Release Build ---"

cmake -B build-windows \
      -D CMAKE_BUILD_TYPE=Release \
      -D CMAKE_TOOLCHAIN_FILE=windows-toolchain.cmake \
      -S .

echo "--- Building Windows Release ---"
cmake --build build-windows
