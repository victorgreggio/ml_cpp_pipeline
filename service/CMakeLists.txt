include(proto/grpc.cmake)

# Add the executable target
add_executable(service 
    main.cpp)

# Link the libraries
target_link_libraries(service
    PRIVATE
    iris_grpc_proto
    CLI11::CLI11
    protobuf::libprotobuf
    grpc++_reflection    
    ${GRPC_LIBRARIES}
)

target_include_directories(service PRIVATE ${GRPC_INCLUDE_DIRS})
target_compile_options(service PRIVATE ${GRPC_CFLAGS_OTHER})

# Add mlpack compile flags if found via pkg-config
if(MLPACK_FOUND)
    set(_mlpack_valid_includes)
    foreach(_inc IN LISTS MLPACK_INCLUDE_DIRS)
        if(IS_DIRECTORY "${_inc}" AND NOT _inc MATCHES "^/builddir/")
            list(APPEND _mlpack_valid_includes "${_inc}")
        endif()
    endforeach()
    if(_mlpack_valid_includes)
        target_include_directories(service PRIVATE ${_mlpack_valid_includes})
        target_compile_options(service PRIVATE ${MLPACK_CFLAGS_OTHER})
        target_link_libraries(service PRIVATE ${MLPACK_LIBRARIES})
    endif()
endif()

# Add the executable target
add_executable(client 
    client.cpp)

# Link the libraries
target_link_libraries(client
    PRIVATE
    iris_grpc_proto
    CLI11::CLI11
    protobuf::libprotobuf
    grpc++_reflection
    ${GRPC_LIBRARIES}
)

target_include_directories(client PRIVATE ${GRPC_INCLUDE_DIRS})
target_compile_options(client PRIVATE ${GRPC_CFLAGS_OTHER})