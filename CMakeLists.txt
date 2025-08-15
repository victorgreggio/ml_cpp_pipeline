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
    GIT_TAG main
)

CPMAddPackage(
    NAME HighFive
    GITHUB_REPOSITORY BlueBrain/HighFive
    GIT_TAG master
    OPTIONS 
        "HIGHFIVE_EXAMPLES OFF"
        "HIGHFIVE_UNIT_TESTS OFF"
        "HIGHFIVE_BUILD_DOCS OFF"
        "HIGHFIVE_USE_BOOST OFF"
        "HIGHFIVE_USE_EIGEN OFF"
        "HIGHFIVE_USE_OPENCV OFF"
        "HIGHFIVE_USE_XTENSOR OFF"
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

# Find and use PkgConfig
find_package(PkgConfig REQUIRED)

# Use system packages for complex dependencies
pkg_check_modules(GRPC REQUIRED grpc++ grpc)
find_package(Protobuf REQUIRED)
find_package(Threads REQUIRED)
find_package(PostgreSQL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(MLPACK mlpack)

# Add the executable targets
add_subdirectory(extractor)
add_subdirectory(wrangler)
add_subdirectory(trainer)
add_subdirectory(service)
