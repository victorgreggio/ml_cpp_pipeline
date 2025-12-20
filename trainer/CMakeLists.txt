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
    set(_mlpack_valid_includes)
    foreach(_inc IN LISTS MLPACK_INCLUDE_DIRS)
        if(IS_DIRECTORY "${_inc}" AND NOT _inc MATCHES "^/builddir/")
            list(APPEND _mlpack_valid_includes "${_inc}")
        endif()
    endforeach()
    if(_mlpack_valid_includes)
        target_include_directories(trainer PRIVATE ${_mlpack_valid_includes})
        target_compile_options(trainer PRIVATE ${MLPACK_CFLAGS_OTHER})
        target_link_libraries(trainer PRIVATE ${MLPACK_LIBRARIES})
    endif()
endif()
