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

# Option to build tests - enabled by default if GTest is available
option(BUILD_TESTS "Build unit tests" ON)

# Find the packages library
find_package(fmt REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(Threads REQUIRED)
find_package(CLI11 CONFIG REQUIRED)
find_package(HighFive CONFIG REQUIRED)
find_package(Sqlpp11 REQUIRED COMPONENTS PostgreSQL)
find_package(Armadillo REQUIRED)

# Add the executable targets
add_subdirectory(extractor)
add_subdirectory(wrangler)
add_subdirectory(trainer)
add_subdirectory(service)

# Add tests if requested and GTest is available
if(BUILD_TESTS)
  find_package(PkgConfig QUIET)
  if(PkgConfig_FOUND)
    pkg_check_modules(GTEST gtest>=1.10.0)
    if(GTEST_FOUND)
      enable_testing()
      add_subdirectory(tests)
      message(STATUS "Unit tests enabled")
    else()
      message(STATUS "GTest not found via pkg-config, tests disabled")
    endif()
  else()
    # Try to find GTest via CMake
    find_package(GTest QUIET)
    if(GTest_FOUND)
      enable_testing()
      add_subdirectory(tests)
      message(STATUS "Unit tests enabled")
    else()
      message(STATUS "GTest not found, tests disabled")
    endif()
  endif()
endif()