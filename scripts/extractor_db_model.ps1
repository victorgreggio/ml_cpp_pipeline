# NOTE: This script is no longer needed with CPM.cmake migration
# The iris.h file has been pre-generated and is committed to the repository.
#
# If you need to regenerate iris.h after database schema changes:
# 1. Install sqlpp11-ddl2cpp tool manually:
#    pip install pyparsing
#    git clone https://github.com/rbock/sqlpp11.git
#    cd sqlpp11/scripts
# 2. Run: python sqlpp11-ddl2cpp <path-to-ddl.sql> <output-path> <namespace>
#
# Example:
# python sqlpp11-ddl2cpp data/db/ddl.sql extractor/iris iris

Write-Host "This script is deprecated. The iris.h file is pre-generated and committed."
Write-Host "If database schema changes, manually regenerate using sqlpp11-ddl2cpp tool."
Write-Host "See comments in this script for instructions."
exit 0