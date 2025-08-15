#include <sstream>
#include <CLI/CLI.hpp>
#include <grpcpp/grpcpp.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/health_check_service_interface.h>

#include "ClassifierService.h"

void RunServer(uint16_t port, std::filesystem::path modelFilePath)
{
    std::stringstream ss;
    ss << "0.0.0.0:" << port;
    std::string server_address = ss.str();
    IrisClassifierService service(modelFilePath);

    grpc::EnableDefaultHealthCheckService(true);
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();
    grpc::ServerBuilder builder;

    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    server->Wait();
}

int main(int argc, char *argv[])
{
    CLI::App app{"Server for Iris dataset"};
    argv = app.ensure_utf8(argv);

    std::string inputFilePath;
    app.add_option("-i,--input", inputFilePath, "Input file path")
        ->default_val("model.xml")
        ->capture_default_str();

    uint16_t port;
    app.add_option("-p,--port", port, "TCP port to listen on")
        ->default_val(50051)
        ->capture_default_str();

    CLI11_PARSE(app, argc, argv);

    std::filesystem::path inputPath(inputFilePath);
    if (inputPath.is_relative())
    {
        inputPath = std::filesystem::current_path() / inputPath;
    }

    if (!std::filesystem::exists(inputPath))
    {
        std::cerr << "Input file does not exist: " << inputPath << '\n';
        return EXIT_FAILURE;
    }

    RunServer(port, inputPath);
    return EXIT_SUCCESS;
}