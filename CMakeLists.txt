cmake_minimum_required(VERSION 3.28)
project(MachineLearningPipeline)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set the C++ compiler flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)
endif()
option(protobuf_MODULE_COMPATIBLE TRUE)

# Download CPM.cmake
file(DOWNLOAD
    https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.40.2/CPM.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
    EXPECTED_HASH SHA256=c8cdc32c03816538ce22781ed72964dc864b2a34a310d3b7104812a5ca2d835d
)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)

# Option to build tests - enabled by default
option(BUILD_TESTS "Build unit tests" ON)

# Add lightweight dependencies using CPM
CPMAddPackage(
    NAME CLI11
    GITHUB_REPOSITORY CLIUtils/CLI11
    GIT_TAG main
    OPTIONS
      "CLI11_BUILD_EXAMPLES OFF" 
      "CLI11_BUILD_TESTS OFF"
)

CPMAddPackage(
    NAME HighFive
    GITHUB_REPOSITORY highfive-devs/highfive
    GIT_TAG main
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
    NAME sqlpp23
    GITHUB_REPOSITORY rbock/sqlpp23
    GIT_TAG main
    OPTIONS
        "BUILD_TESTING OFF"
        "BUILD_POSTGRESQL_CONNECTOR ON"
        "BUILD_MYSQL_CONNECTOR OFF"
        "BUILD_MARIADB_CONNECTOR OFF"
        "BUILD_SQLITE3_CONNECTOR OFF"
)

# Add GTest for unit testing
if(BUILD_TESTS)
    CPMAddPackage(
        NAME googletest
        GITHUB_REPOSITORY google/googletest
        GIT_TAG main
        OPTIONS
            "BUILD_GMOCK OFF"
            "INSTALL_GTEST OFF"
    )
    if(googletest_ADDED)
        enable_testing()
        add_subdirectory(tests)
        message(STATUS "Unit tests enabled via CPM")
    else()
        message(STATUS "GTest not available, tests disabled")
    endif()
endif()

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
