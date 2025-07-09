Param(
    [Parameter(Mandatory = $false)]
    [string]$Configuration = "Debug"
)

$vswherePath = "C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe"
# Check if vswhere is installed
if (-not (Test-Path $vswherePath)) {
    Write-Host "Visual Studio Build Tools are not installed. Please install Visual Studio Build Tools to run this script. https://aka.ms/vs/17/release/vs_BuildTools.exe"
    exit 1
}

# Get the installation path of Visual Studio Build Tools
$buildToolsInstallPath = & $vswherePath -products Microsoft.VisualStudio.Product.BuildTools -property installationPath
if (-not $buildToolsInstallPath) {
    Write-Host "Visual Studio Build Tools installation path could not be determined."
    exit 1
}

# Get the version of Visual Studio Build Tools
$buildToolsVersion = & $vswherePath -products Microsoft.VisualStudio.Product.BuildTools -property installationVersion
if (-not $buildToolsVersion) {
    Write-Host "Visual Studio Build Tools version could not be determined."
    exit 1
}

# Define the path to the CMake executable
$cmakePath = Join-Path $buildToolsInstallPath "Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"
if (-not (Test-Path $cmakePath)) {  
    Write-Host "CMake is not installed. Please install CMake to run this script."
    exit 1
}

# Define the path to the VCPKG executable
if ($buildToolsVersion -ge "17.0") {
    # For Visual Studio 2022 and later, VCPKG is included with the Build Tools
    $vcpkgPath = Join-Path $buildToolsInstallPath "VC\vcpkg\vcpkg.exe"
    if(-not (Test-Path $vcpkgPath)) {
        Write-Host "VCPKG is not installed. Please install VCPKG to run this script."
        exit 1
    }

    if ($Configuration -eq "Release") {
        $presetName = "VS17R"
    } else {
        $presetName = "VS17D"
    }
} else {
    # For Visual Studio 2019 and earlier, check if VCPKG is in the PATH
    $vcpkgApp = Get-Command vcpkg -ErrorAction SilentlyContinue
    if (-not $vcpkgApp) {
        Write-Host "VCPKG is not installed(or is not in the PATH). Please install VCPKG to run this script. https://learn.microsoft.com/en-us/vcpkg/get_started/get-started"
        exit 1
    }
    $vcpkgPath = $vcpkgApp.Source

    if ($Configuration -eq "Release") {
        $presetName = "VS16R"
    } else {
        $presetName = "VS16D"
    }
}

# Define VCPKG_ROOT environment variable
$env:VCPKG_ROOT = Split-Path -Path $vcpkgPath -Parent

# Get the root path of the project
$projectRoot = Split-Path -Path $PSScriptRoot -Parent

# Define the build directory
$buildDir = Join-Path -Path $projectRoot -ChildPath "build/$presetName"

& $cmakePath --preset $presetName
if ($LASTEXITCODE -ne 0) {
    Write-Host "CMake preset failed."
    exit 1
}

& $cmakePath --build $buildDir
if ($LASTEXITCODE -ne 0) {
    Write-Host "CMake build failed."
    exit 1
}