#include <fmt/format.h>
#include <CLI/CLI.hpp>
#include <grpcpp/grpcpp.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/health_check_service_interface.h>
#include <mlpack/core.hpp>
#include <mlpack/methods/softmax_regression/softmax_regression.hpp>

#include "iris.grpc.pb.h"

std::string GetSpecieName(size_t prediction)
{
    switch (prediction)
    {
    case 0:
        return "Setosa";
    case 1:
        return "Versicolor";
    case 2:
        return "Virginica";
    default:
        return "Unknown";
    }
}

class IrisClassifierService final : public iris::IrisClassifier::Service
{
public:
    IrisClassifierService(mlpack::SoftmaxRegression<arma::dmat> &smr)
        : m_smr(std::move(smr))
    {
    }

    ~IrisClassifierService() override = default;

    grpc::Status Classify(grpc::ServerContext *context, const iris::ClassifyRequest *request, iris::ClassifyResponse *response) override
    {
        arma::dmat inputData(4, 1);
        inputData(0, 0) = request->petallength();
        inputData(1, 0) = request->petalwidth();
        inputData(2, 0) = request->sepallength();
        inputData(3, 0) = request->sepallength();

        arma::Row<size_t> predictions;
        try
        {
            m_smr.Classify(inputData, predictions);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return grpc::Status(grpc::StatusCode::INTERNAL, "Error during classification");
        }
        response->set_specie(GetSpecieName(predictions(0)));
        return grpc::Status::OK;
    }

private:
    mlpack::SoftmaxRegression<arma::dmat> m_smr;
};

void RunServer(uint16_t port, std::filesystem::path modelFilePath)
{
    mlpack::SoftmaxRegression smr;
    if (!mlpack::data::Load(modelFilePath.string(), "model", smr))
    {
        std::cerr << "Failed to load model from: " << modelFilePath << '\n';
        exit(EXIT_FAILURE);
    }

    std::string server_address = fmt::format("0.0.0.0:{}", port);
    IrisClassifierService service{smr};

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