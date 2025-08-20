#!/bin/bash
# Bash script to test gRPC service client
set -e

PRESET="Debug"

(cd ".." && build/$PRESET/service/client -p 50051)
