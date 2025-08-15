# Use GTest from CPM (googletest package)
# GTest should be available through the parent CMakeLists.txt via CPM

# Create test executable for CsvWriter
add_executable(csv_writer_test test_csv_writer.cpp)
target_include_directories(csv_writer_test PRIVATE ../wrangler)
target_link_libraries(csv_writer_test PRIVATE gtest gtest_main pthread)
add_test(NAME CsvWriterTest COMMAND csv_writer_test)

# Create test executable for Iris struct and enum
add_executable(iris_test test_iris.cpp)
target_include_directories(iris_test PRIVATE ../wrangler)
target_link_libraries(iris_test PRIVATE gtest gtest_main pthread)
add_test(NAME IrisTest COMMAND iris_test)

# Create test executable for ClassifierService (getSpecieName function)
add_executable(classifier_service_test test_classifier_service.cpp)
target_link_libraries(classifier_service_test PRIVATE gtest gtest_main pthread)
add_test(NAME ClassifierServiceTest COMMAND classifier_service_test)