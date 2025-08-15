# Add the executable target
add_executable(extractor 
    main.cpp)

# Add include directories for SQLPP20 PostgreSQL connector
target_include_directories(extractor PRIVATE 
    ${sqlpp20_SOURCE_DIR}/connectors/postgresql/include)

# Link the libraries
target_link_libraries(extractor 
    PRIVATE
    CLI11::CLI11
    HighFive
    sqlpp20
    PostgreSQL::PostgreSQL)