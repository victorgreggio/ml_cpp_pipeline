include(proto/grpc.cmake)

# Add the executable target
add_executable(service 
    main.cpp)

# Link the libraries
target_link_libraries(service
    PRIVATE
    fmt::fmt
    CLI11::CLI11
    ${GRPC_LIBRARIES}
    grpc++_reflection
    iris_grpc_proto
    armadillo
    protobuf::libprotobuf
)

target_include_directories(service PRIVATE ${GRPC_INCLUDE_DIRS})
target_compile_options(service PRIVATE ${GRPC_CFLAGS_OTHER})

# Add mlpack compile flags if found via pkg-config
if(MLPACK_FOUND)
    target_include_directories(service PRIVATE ${MLPACK_INCLUDE_DIRS})
    target_compile_options(service PRIVATE ${MLPACK_CFLAGS_OTHER})
    target_link_libraries(service PRIVATE ${MLPACK_LIBRARIES})
endif()

# Add the executable target
add_executable(client 
    client.cpp)

# Link the libraries
target_link_libraries(client
    PRIVATE
    fmt::fmt
    CLI11::CLI11
    ${GRPC_LIBRARIES}
    grpc++_reflection
    iris_grpc_proto
    protobuf::libprotobuf
)

target_include_directories(client PRIVATE ${GRPC_INCLUDE_DIRS})
target_compile_options(client PRIVATE ${GRPC_CFLAGS_OTHER})