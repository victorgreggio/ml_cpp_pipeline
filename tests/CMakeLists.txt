# Try to find GTest through different methods
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
  pkg_check_modules(GTEST gtest>=1.10.0)
endif()

if(NOT GTEST_FOUND)
  find_package(GTest QUIET)
  if(GTest_FOUND)
    # Use CMake GTest targets
    set(GTEST_LIBRARIES GTest::gtest GTest::gtest_main)
    set(GTEST_INCLUDE_DIRS "")
    set(GTEST_CFLAGS_OTHER "")
  endif()
endif()

if(NOT GTEST_FOUND AND NOT GTest_FOUND)
  message(FATAL_ERROR "GTest not found. Please install Google Test.")
endif()

# Create test executable for CsvWriter
add_executable(csv_writer_test test_csv_writer.cpp)
target_include_directories(csv_writer_test PRIVATE 
    ../wrangler
    ${GTEST_INCLUDE_DIRS}
)
target_link_libraries(csv_writer_test PRIVATE 
    ${GTEST_LIBRARIES} 
    pthread
)
if(GTEST_CFLAGS_OTHER)
  target_compile_options(csv_writer_test PRIVATE ${GTEST_CFLAGS_OTHER})
endif()
add_test(NAME CsvWriterTest COMMAND csv_writer_test)

# Create test executable for Iris struct and enum
add_executable(iris_test test_iris.cpp)
target_include_directories(iris_test PRIVATE 
    ../wrangler
    ${GTEST_INCLUDE_DIRS}
)
target_link_libraries(iris_test PRIVATE 
    ${GTEST_LIBRARIES} 
    pthread
)
if(GTEST_CFLAGS_OTHER)
  target_compile_options(iris_test PRIVATE ${GTEST_CFLAGS_OTHER})
endif()
add_test(NAME IrisTest COMMAND iris_test)

# Create test executable for ClassifierService (getSpecieName function)
add_executable(classifier_service_test test_classifier_service.cpp)
target_include_directories(classifier_service_test PRIVATE 
    ${GTEST_INCLUDE_DIRS}
)
target_link_libraries(classifier_service_test PRIVATE 
    ${GTEST_LIBRARIES} 
    pthread
)
if(GTEST_CFLAGS_OTHER)
  target_compile_options(classifier_service_test PRIVATE ${GTEST_CFLAGS_OTHER})
endif()
add_test(NAME ClassifierServiceTest COMMAND classifier_service_test)