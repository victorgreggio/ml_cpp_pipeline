# Add the executable target
add_executable(extractor 
    main.cpp)

# Link the libraries
target_link_libraries(extractor 
    PRIVATE
    CLI11::CLI11
    HighFive
    sqlpp23::sqlpp23
    PostgreSQL::PostgreSQL)