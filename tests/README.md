# Unit Tests for ML C++ Pipeline

This directory contains unit tests for the key components of the machine learning pipeline using Google Test framework.

## Tests Included

### 1. CsvWriter Tests (`test_csv_writer.cpp`)
Tests the CSV writing functionality used by the wrangler component:
- Basic CSV writing with headers and data
- Numeric value handling
- Special character escaping (quotes)
- Custom separators
- Stream manipulators (endrow, flush)
- Mixed data types (simulating Iris dataset)

### 2. Iris Struct Tests (`test_iris.cpp`)
Tests the Iris data structure and enum:
- IrisType enum values validation
- Iris struct initialization
- Copy construction behavior
- Type comparison operations

### 3. Classifier Service Tests (`test_classifier_service.cpp`)
Tests the species name conversion logic from the classifier service:
- Correct mapping of prediction values to species names
- Handling of invalid/unknown prediction values
- Edge cases with large numbers and wraparound values

## Running Tests

### Prerequisites
- Google Test framework (installed via system package manager or vcpkg)
- CMake 3.20+
- C++17 compatible compiler

### Build and Run Tests

#### Option 1: As part of main build (if vcpkg dependencies are working)
```bash
cmake --preset CI
cmake --build build --config Release
ctest --test-dir build --verbose
```

#### Option 2: Tests only (when main dependencies have issues)
```bash
# Create test-only build
mkdir build_tests
cd build_tests

# Copy simplified CMakeLists and required headers
cp ../CMakeLists_tests_only.txt CMakeLists.txt
cp -r ../tests .
cp -r ../wrangler .

# Configure and build
cmake .
cmake --build .

# Run tests
ctest --verbose
# or run individual tests
./tests/csv_writer_test
./tests/iris_test
./tests/classifier_service_test
```

### Test Configuration
Tests are automatically discovered and can be disabled by setting `-DBUILD_TESTS=OFF` during CMake configuration.

The test framework tries to find Google Test through:
1. pkg-config (system installation)
2. CMake find_package (vcpkg or other installations)

### Test Coverage
These tests focus on:
- **CsvWriter**: Core functionality that doesn't require external dependencies
- **Iris Types**: Basic data structures used throughout the pipeline
- **Species Mapping**: Pure function logic from the classifier service

More complex integration tests would require:
- Database setup for extractor component
- MLPack model files for full classifier testing
- gRPC server setup for end-to-end testing

## Design Philosophy
These unit tests follow the principle of testing the smallest possible units of functionality that can be tested in isolation. They focus on:
- Pure functions and data structures
- Core business logic
- Error handling and edge cases
- No external dependencies (database, network, large ML libraries)

This approach ensures tests are fast, reliable, and can run in any CI/CD environment without complex setup.