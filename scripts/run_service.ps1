Param(
    [Parameter(Mandatory = $false)]
    [string]$Configuration = "Debug"
)

# Get the root path of the project
$projectRoot = Split-Path -Path $PSScriptRoot -Parent

# Run build.ps1
$buildScriptPath = Join-Path -Path $projectRoot -ChildPath "scripts/build.ps1"
if (-not (Test-Path $buildScriptPath)) {
    Write-Host "Build script not found at $buildScriptPath. Please ensure the script exists."
    exit 1
}

& $buildScriptPath -Configuration $Configuration
if ($LASTEXITCODE -ne 0) {
    Write-Host "Build script failed. Please check the output for errors."
    exit 1
}

$vswherePath = "C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe"
# Check if vswhere is installed
if (-not (Test-Path $vswherePath)) {
    Write-Host "Visual Studio Build Tools are not installed. Please install Visual Studio Build Tools to run this script. https://aka.ms/vs/17/release/vs_BuildTools.exe"
    exit 1
}

# Get the version of Visual Studio Build Tools
$buildToolsVersion = & $vswherePath -products Microsoft.VisualStudio.Product.BuildTools -property installationVersion
if (-not $buildToolsVersion) {
    Write-Host "Visual Studio Build Tools version could not be determined."
    exit 1
}

# Define the path to the build directory based on the configuration and BuildTools version
if ($buildToolsVersion -ge "17.0") {
    if ($Configuration -eq "Release") {
        $presetName = "VS17R"
    } else {
        $presetName = "VS17D"
    }
    $buildDir = Join-Path -Path $projectRoot -ChildPath "build/$presetName"
} else {
    if ($Configuration -eq "Release") {
        $presetName = "VS16R"
    } else {
        $presetName = "VS16D"
    }
    $buildDir = Join-Path -Path $projectRoot -ChildPath "build/$presetName"
}

# Run extractor, wrangler, trainer, and service executables
$extractorPath = Join-Path -Path $buildDir -ChildPath "extractor/$Configuration/extractor.exe"
$wranglerPath = Join-Path -Path $buildDir -ChildPath "wrangler/$Configuration/wrangler.exe"
$trainerPath = Join-Path -Path $buildDir -ChildPath "trainer/$Configuration/trainer.exe"
$servicePath = Join-Path -Path $buildDir -ChildPath "service/$Configuration/service.exe"

# Check if the executables exist
if (-not (Test-Path $extractorPath)) {
    Write-Host "Extractor executable not found at $extractorPath. Please ensure the build was successful."
    exit 1
}

if (-not (Test-Path $wranglerPath)) {
    Write-Host "Wrangler executable not found at $wranglerPath. Please ensure the build was successful."
    exit 1
}

if (-not (Test-Path $trainerPath)) {
    Write-Host "Trainer executable not found at $trainerPath. Please ensure the build was successful."
    exit 1
}

if (-not (Test-Path $servicePath)) {
    Write-Host "Service executable not found at $servicePath. Please ensure the build was successful."
    exit 1
}

# Define the output paths
$outputHdfPath = Join-Path -Path $projectRoot -ChildPath "data/output.hdf"
$outputCsvPath = Join-Path -Path $projectRoot -ChildPath "data/output.csv"
$outputXmlPath = Join-Path -Path $projectRoot -ChildPath "data/output.xml"

& $extractorPath -o $outputHdfPath
if ($LASTEXITCODE -ne 0) {
    Write-Host "Extractor failed. Please check the output for errors."
    exit 1
}

& $wranglerPath -i $outputHdfPath -o $outputCsvPath
if ($LASTEXITCODE -ne 0) {
    Write-Host "Wrangler failed. Please check the output for errors."
    exit 1
}

& $trainerPath -i $outputCsvPath -o $outputXmlPath
if ($LASTEXITCODE -ne 0) {
    Write-Host "Trainer failed. Please check the output for errors."
    exit 1
}

& $servicePath -i $outputXmlPath
if ($LASTEXITCODE -ne 0) {
    Write-Host "Service failed. Please check the output for errors."
    exit 1
}