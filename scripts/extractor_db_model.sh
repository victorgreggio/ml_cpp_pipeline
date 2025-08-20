#!/usr/bin/env bash
set -e
PRESET=Debug

# Check for Python
if ! command -v python &> /dev/null; then
    echo "Python is not installed. Please install Python to run this script."
    exit 1
fi

# Check for pip
if ! command -v pip &> /dev/null; then
    echo "pip is not available. Please ensure pip is installed with Python."
    exit 1
fi

# Install required Python package
pip install --user pyparsing

# Get parent directory of this script
parentPath="$(dirname "$(realpath "$0")")/.."

# Define the path to the Python script
(cd ".." && cmake --preset $PRESET)
scriptPath="$parentPath/build/$PRESET/_deps/sqlpp23-src/scripts/sqlpp23-ddl2cpp"

# Check if the Python script exists
if [ ! -f "$scriptPath" ]; then
    echo "The Python script sqlpp23-ddl2cpp does not exist at the expected path: $scriptPath"
    exit 1
fi

# Define the database model file
dbModelFile="$parentPath/data/db/ddl.sql"

# Check if the database model file exists
if [ ! -f "$dbModelFile" ]; then
    echo "The database model file ddl.sql does not exist at the expected path: $dbModelFile"
    exit 1
fi

# Define namespace
namespace="iris"

# Define the header file path
headerFile="extractor/$namespace.h"

# Define the target path for the output
targetPath="$parentPath/$headerFile"

echo "Running the Python script to process the database model..."
echo "Using Python: $(which python)"
echo "Using script: $scriptPath"
echo "Using database model file: $dbModelFile"
echo "Using target path: $targetPath"
echo "Using namespace: $namespace"

python "$scriptPath" --path-to-ddl "$dbModelFile" --path-to-header  "$targetPath" --namespace "$namespace"
status=$?
if [ $status -ne 0 ]; then
    echo "The Python script encountered an error while processing the database model."
    exit 1
fi
echo "Database model processed successfully."


    exit 1


}