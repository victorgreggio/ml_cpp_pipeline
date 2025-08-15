#!/bin/bash
# Bash script to run the full ML pipeline and start gRPC service
set -e

PRESET="Debug"
BUILD_DIR="build/$PRESET"
DATA_DIR="data"

# Step 1: Extract data
$BUILD_DIR/extractor/extractor -o $DATA_DIR/output.hdf
# Step 2: Wrangle data
$BUILD_DIR/wrangler/wrangler -i $DATA_DIR/output.hdf -o $DATA_DIR/output.csv
# Step 3: Train model
$BUILD_DIR/trainer/trainer -i $DATA_DIR/output.csv -o $DATA_DIR/output.xml
# Step 4: Start gRPC service
$BUILD_DIR/service/service -i $DATA_DIR/output.xml
