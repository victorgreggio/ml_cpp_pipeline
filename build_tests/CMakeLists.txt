cmake_minimum_required(VERSION 3.20)
project(MachineLearningPipelineTests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set the C++ compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# Enable testing at project level
enable_testing()

# Only build and run tests for now
add_subdirectory(tests)