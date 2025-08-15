#!/bin/bash
# Bash build script for Linux (CI or local)
set -e

PRESET="Debug"

# Use Debug preset for Linux
cmake --preset $PRESET
cmake --build build --config $PRESET

echo "Build completed."
