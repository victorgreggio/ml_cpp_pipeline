#!/bin/bash
# Bash build script for Linux (CI or local)
set -e

PRESET="Debug"

# Use Debug preset for Linux
(cd ".." && cmake --preset $PRESET)
(cd ".." && cmake --build build --preset $PRESET)

echo "Build completed."
