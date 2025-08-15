#!/bin/bash
# Bash script to test gRPC service client
set -e

PRESET="CI"
CONFIG="Release"
BUILD_DIR="build/$PRESET"

$BUILD_DIR/service/$CONFIG/client -p 50051
