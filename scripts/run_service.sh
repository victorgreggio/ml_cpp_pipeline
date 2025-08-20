#!/bin/bash
# Bash script to run the full ML pipeline and start gRPC service
set -e

HOST_ARG="$1"
if [ -z "$HOST_ARG" ]; then
	HOST_ARG="localhost"
fi

PRESET="Debug"
DATA_DIR="data"

# Step 1: Extract data
(cd ".." && build/$PRESET/extractor/extractor -H "$HOST_ARG" -o $DATA_DIR/output.hdf)
# Step 2: Wrangle data
(cd ".." && build/$PRESET/wrangler/wrangler -i $DATA_DIR/output.hdf -o $DATA_DIR/output.csv)
# Step 3: Train model
(cd ".." && build/$PRESET/trainer/trainer -i $DATA_DIR/output.csv -o $DATA_DIR/output.xml)
# Step 4: Start gRPC service
(cd ".." && build/$PRESET/service/service -i $DATA_DIR/output.xml)
