include(proto/grpc.cmake)

# Add the executable target
add_executable(service 
    main.cpp)

target_include_directories(service 
    PRIVATE 
    ${MLPACK_INCLUDE_DIRS}
    ${ARMADILLO_INCLUDE_DIRS}
)

# Link the libraries
target_link_libraries(service
    PRIVATE
    fmt::fmt
    CLI11::CLI11
    gRPC::gpr 
    gRPC::grpc 
    gRPC::grpc++ 
    gRPC::grpc++_alts
    iris_grpc_proto
    ${ARMADILLO_LIBRARIES}
)


# Add the executable target
add_executable(client 
    client.cpp)

# Link the libraries
target_link_libraries(client
    PRIVATE
    fmt::fmt
    CLI11::CLI11
    gRPC::gpr 
    gRPC::grpc 
    gRPC::grpc++ 
    gRPC::grpc++_alts
    iris_grpc_proto
)