# Add the executable target
add_executable(trainer main.cpp)

# Link the libraries
target_link_libraries(trainer
    PRIVATE
    CLI11::CLI11
    armadillo
)

# Add mlpack compile flags if found via pkg-config
if(MLPACK_FOUND)
    target_include_directories(trainer PRIVATE ${MLPACK_INCLUDE_DIRS})
    target_compile_options(trainer PRIVATE ${MLPACK_CFLAGS_OTHER})
    target_link_libraries(trainer PRIVATE ${MLPACK_LIBRARIES})
endif()
