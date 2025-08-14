# Find GTest
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTEST REQUIRED gtest>=1.10.0)

# Enable testing
enable_testing()

# Create test executable for CsvWriter
add_executable(csv_writer_test test_csv_writer.cpp)
target_include_directories(csv_writer_test PRIVATE 
    ../wrangler
    ${GTEST_INCLUDE_DIRS}
)
target_link_libraries(csv_writer_test PRIVATE 
    ${GTEST_LIBRARIES} 
    gtest_main
    pthread
)
target_compile_options(csv_writer_test PRIVATE ${GTEST_CFLAGS_OTHER})
add_test(NAME CsvWriterTest COMMAND csv_writer_test)

# Create test executable for Iris struct and enum
add_executable(iris_test test_iris.cpp)
target_include_directories(iris_test PRIVATE 
    ../wrangler
    ${GTEST_INCLUDE_DIRS}
)
target_link_libraries(iris_test PRIVATE 
    ${GTEST_LIBRARIES} 
    gtest_main
    pthread
)
target_compile_options(iris_test PRIVATE ${GTEST_CFLAGS_OTHER})
add_test(NAME IrisTest COMMAND iris_test)

# Create test executable for ClassifierService (getSpecieName function)
add_executable(classifier_service_test test_classifier_service.cpp)
target_include_directories(classifier_service_test PRIVATE 
    ${GTEST_INCLUDE_DIRS}
)
target_link_libraries(classifier_service_test PRIVATE 
    ${GTEST_LIBRARIES} 
    gtest_main
    pthread
)
target_compile_options(classifier_service_test PRIVATE ${GTEST_CFLAGS_OTHER})
add_test(NAME ClassifierServiceTest COMMAND classifier_service_test)