find_path(MLPACK_INCLUDE_DIRS "mlpack.hpp")

# Add the executable target
add_executable(trainer main.cpp)

target_include_directories(trainer 
    PRIVATE 
    ${MLPACK_INCLUDE_DIRS}
    ${ARMADILLO_INCLUDE_DIRS})

# Link the libraries
target_link_libraries(trainer
    PRIVATE
    CLI11::CLI11
    ${ARMADILLO_LIBRARIES}
)
