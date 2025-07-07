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