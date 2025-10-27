#!/bin/bash
# This script builds all target platforms
set -e

echo "--- CLEANING OLD BUILD DIRECTORIES ---"
rm -rf build-linux-debug
rm -rf build-linux-release
rm -rf build-windows
echo "--- Old directories cleaned ---"
echo ""

echo "--- STARTING ALL BUILDS ---"
echo ""

# Build Linux Debug
./build_linux_debug.sh
echo ""
echo "--- Linux Debug build complete ---"
echo ""

# Build Linux Release
./build_linux_release.sh
echo ""
echo "--- Linux Release build complete ---"
echo ""

# Build Windows Release
./build_win_release.sh
echo ""
echo "--- Windows Release build complete ---"
echo ""
echo "--- ALL BUILDS FINISHED ---"