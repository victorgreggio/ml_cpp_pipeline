include(proto/grpc.cmake)

# Add the executable target
add_executable(service 
    main.cpp)

# Set C++20 for compatibility with rapidjson in MLPack
set_property(TARGET service PROPERTY CXX_STANDARD 20)

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
    target_include_directories(service PRIVATE ${MLPACK_INCLUDE_DIRS})
    target_compile_options(service PRIVATE ${MLPACK_CFLAGS_OTHER})
    target_link_libraries(service PRIVATE ${MLPACK_LIBRARIES})
endif()

# Add the executable target
add_executable(client 
    client.cpp)

# Set C++20 for compatibility with rapidjson in MLPack (if used)
set_property(TARGET client PROPERTY CXX_STANDARD 20)

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