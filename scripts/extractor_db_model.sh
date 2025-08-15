#!/bin/bash
# Bash script to setup PostgreSQL iris database and schema
set -e

PRESET="Debug"

cmake --preset $PRESET

DDL_TO_CPP_PATH="build/$PRESET/_deps/sqlpp23-src/scripts/sqlpp23-ddl2cpp"




echo "Database model exported."
