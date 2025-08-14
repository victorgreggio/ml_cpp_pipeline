cmake_minimum_required(VERSION 3.20)
project(MachineLearningPipeline)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set the C++ compiler flags
if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
endif()
option(protobuf_MODULE_COMPATIBLE TRUE)

# Download CPM.cmake
file(DOWNLOAD
    https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.40.2/CPM.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
    EXPECTED_HASH SHA256=c8cdc32c03816538ce22781ed72964dc864b2a34a310d3b7104812a5ca2d835d
)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)

# Add lightweight dependencies using CPM
CPMAddPackage("gh:fmtlib/fmt#10.2.1")

CPMAddPackage(
    NAME CLI11
    GITHUB_REPOSITORY CLIUtils/CLI11
    VERSION 2.4.1
)

CPMAddPackage(
    NAME HighFive
    GITHUB_REPOSITORY BlueBrain/HighFive
    VERSION 2.9.0
    OPTIONS 
        "HIGHFIVE_EXAMPLES OFF"
        "HIGHFIVE_UNIT_TESTS OFF"
        "HIGHFIVE_BUILD_DOCS OFF"
        "HIGHFIVE_USE_BOOST OFF"
        "HIGHFIVE_USE_EIGEN OFF"
        "HIGHFIVE_USE_OPENCV OFF"
        "HIGHFIVE_USE_XTENSOR OFF"
)

# Use system packages for complex dependencies
find_package(Threads REQUIRED)
find_package(PostgreSQL REQUIRED)
find_package(PkgConfig REQUIRED)

# Try to find system packages first, fallback to building with CPM
find_package(Armadillo QUIET)
if(NOT Armadillo_FOUND)
    message(STATUS "Armadillo not found in system, building with CPM")
    CPMAddPackage(
        NAME armadillo
        GITHUB_REPOSITORY conradsnicta/armadillo-code
        GIT_TAG 12.8.x
        OPTIONS "BUILD_SHARED_LIBS OFF"
    )
endif()

# For sqlpp11, try a different approach
CPMAddPackage(
    NAME date
    GITHUB_REPOSITORY HowardHinnant/date
    VERSION 3.0.1
    OPTIONS "BUILD_TZ_LIB OFF"
)

CPMAddPackage(
    NAME sqlpp11
    GITHUB_REPOSITORY rbock/sqlpp11
    GIT_TAG main
    OPTIONS
        "BUILD_TESTING OFF"
        "SQLPP11_TESTS OFF"
        "USE_MARIADB OFF"
        "USE_MYSQL OFF"
        "USE_SQLITE3 OFF"
)

# Use system gRPC and protobuf
pkg_check_modules(GRPC REQUIRED grpc++ grpc)
find_package(Protobuf REQUIRED)

# For mlpack, try to use system package first
find_package(PkgConfig REQUIRED)
pkg_check_modules(MLPACK mlpack)
if(NOT MLPACK_FOUND)
    message(STATUS "MLPack not found in system, will try to build with CPM")
    # Add dependencies for mlpack
    find_package(LAPACK REQUIRED)
    find_package(BLAS REQUIRED)
    
    CPMAddPackage(
        NAME mlpack
        GITHUB_REPOSITORY mlpack/mlpack
        VERSION 4.3.0
        OPTIONS
            "BUILD_TESTS OFF"
            "BUILD_CLI_EXECUTABLES OFF"
            "BUILD_PYTHON_BINDINGS OFF"
            "BUILD_JULIA_BINDINGS OFF"
            "BUILD_GO_BINDINGS OFF"
            "BUILD_R_BINDINGS OFF"
            "BUILD_SHARED_LIBS OFF"
    )
endif()

# Add the executable targets
add_subdirectory(extractor)
add_subdirectory(wrangler)
add_subdirectory(trainer)
add_subdirectory(service)