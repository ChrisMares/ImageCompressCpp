#!/bin/bash
set -e

echo "--- Configuring Linux Release Build ---"

cmake -B build-linux-release \
      -D CMAKE_BUILD_TYPE=Release \
      -S .

echo "--- Building Linux Release ---"
cmake --build build-linux-release

