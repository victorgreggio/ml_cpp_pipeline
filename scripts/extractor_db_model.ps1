# Find if Python is installed
$pythonPath = Get-Command python -ErrorAction SilentlyContinue
if (-not $pythonPath) {
    Write-Host "Python is not installed. Please install Python to run this script."
    exit 1
}

# Ensure pip is available
$pipPath = Get-Command pip -ErrorAction SilentlyContinue
if (-not $pipPath) {
    Write-Host "pip is not available. Please ensure pip is installed with Python."
    exit 1
}
# Install the required Python package
& $pipPath install pyparsing

# Gets the parent directory of the script's location
$parentPath = Split-Path -Path $PSScriptRoot -Parent

# Define the path to the Python script
$scriptPath = Join-Path $parentPath "build\vcpkg_installed\x64-windows\tools\sqlpp11\sqlpp11-ddl2cpp"

# Check if the Python script exists
if (-not (Test-Path $scriptPath)) {
    Write-Host "The Python script sqlpp11-ddl2cpp does not exist at the expected path: $scriptPath"
    exit 1
}

# Define the database model file
$dbModelFile = Join-Path $parentPath "data\db\ddl.sql"

# Check if the database model file exists
if (-not (Test-Path $dbModelFile)) {
    Write-Host "The database model file ddl.sql does not exist at the expected path: $dbModelFile"
    exit 1
}

# Define namespace
$namespace = "iris"

# Define the header file path
$headerFile = Join-Path "extractor" "$namespace"

# Define the target path for the output
$targetPath = Join-Path $parentPath $headerFile

# Run the Python script with the database model file
try {
    Write-Host "Running the Python script to process the database model..."
    Write-Host "Using Python at: $pythonPath"
    Write-Host "Using script at: $scriptPath"
    Write-Host "Using database model file: $dbModelFile"
    Write-Host "Using target path: $targetPath"
    Write-Host "Using namespace: $namespace"

    & $pythonPath $scriptPath $dbModelFile $targetPath $namespace
    if ($LASTEXITCODE -ne 0) {
        Write-Host "The Python script encountered an error while processing the database model."
        exit 1
    }
    Write-Host "Database model processed successfully."
} catch {
    Write-Host "An error occurred while running the Python script: $_"
    exit 1
}