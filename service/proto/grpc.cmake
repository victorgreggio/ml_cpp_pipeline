# Use system protobuf and gRPC
find_program(_PROTOBUF_PROTOC protoc)
find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)

# Proto file
get_filename_component(iris_proto "proto/iris.proto" ABSOLUTE)
get_filename_component(iris_proto_path "${iris_proto}" PATH)

# Generated sources
set(iris_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/iris.pb.cc")
set(iris_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/iris.pb.h")
set(iris_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/iris.grpc.pb.cc")
set(iris_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/iris.grpc.pb.h")
add_custom_command(
      OUTPUT "${iris_proto_srcs}" "${iris_proto_hdrs}" "${iris_grpc_srcs}" "${iris_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${iris_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${iris_proto}"
      DEPENDS "${iris_proto}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# iris_grpc_proto
add_library(iris_grpc_proto
  ${iris_grpc_srcs}
  ${iris_grpc_hdrs}
  ${iris_proto_srcs}
  ${iris_proto_hdrs})

# Link with system protobuf library
target_link_libraries(iris_grpc_proto
  protobuf::libprotobuf
  ${GRPC_LIBRARIES}
  grpc++_reflection
)

target_include_directories(iris_grpc_proto PRIVATE ${GRPC_INCLUDE_DIRS})
target_compile_options(iris_grpc_proto PRIVATE ${GRPC_CFLAGS_OTHER})