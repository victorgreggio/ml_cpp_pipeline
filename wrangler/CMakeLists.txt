# Add the executable target
add_executable(wrangler 
    main.cpp)

# Link the libraries
target_link_libraries(wrangler 
    PRIVATE
    CLI11::CLI11
    HighFive
)