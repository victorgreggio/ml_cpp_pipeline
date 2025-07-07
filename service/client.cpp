#include <iostream>

#include <fmt/format.h>
#include <CLI/CLI.hpp>
#include <grpcpp/grpcpp.h>

#include "iris.grpc.pb.h"

class IrisClassifierClient {
public:
    IrisClassifierClient(const std::string& server_address) : m_server_address(server_address) {
        // Initialize gRPC channel
        m_channel = grpc::CreateChannel(m_server_address, grpc::InsecureChannelCredentials());
        m_stub = iris::IrisClassifier::NewStub(m_channel);
    }

    void Classify(const iris::ClassifyRequest& request, iris::ClassifyResponse& response) {
        grpc::ClientContext context;
        grpc::Status status = m_stub->Classify(&context, request, &response);

        if (status.ok()) {
            std::cout << "Classification result: " << response.specie() << std::endl;
        } else {
            std::cerr << "gRPC call failed: " << status.error_message() << std::endl;
        }
    }
private:
    std::string m_server_address;
    std::shared_ptr<grpc::Channel> m_channel;
    std::unique_ptr<iris::IrisClassifier::Stub> m_stub; 
};

int main(int argc, char* argv[]) {
    CLI::App app{"Server for Iris dataset"};
    argv = app.ensure_utf8(argv);

    std::string hostname;
    app.add_option("hostname", hostname, "Hostname of the server")
        ->default_val("localhost")
        ->check(CLI::ExistingFile);

    uint16_t port;
    app.add_option("-p,--port", port, "TCP port to listen on")
        ->default_val(50051)
        ->capture_default_str();

    CLI11_PARSE(app, argc, argv);

    std::string server_address = fmt::format("{}:{}", hostname, port);
    IrisClassifierClient client(server_address);

    iris::ClassifyRequest request;
    request.set_petallength(5.1f);
    request.set_petalwidth(1.8f);
    request.set_sepallength(7.0f);
    request.set_sepalwidth(3.2f);

    iris::ClassifyResponse response;
    client.Classify(request, response);

    iris::ClassifyRequest request2;
    request2.set_petallength(4.9f);
    request2.set_petalwidth(1.5f);
    request2.set_sepallength(6.4f);
    request2.set_sepalwidth(3.1f);
    iris::ClassifyResponse response2;
    client.Classify(request2, response2);

    return EXIT_SUCCESS;
}