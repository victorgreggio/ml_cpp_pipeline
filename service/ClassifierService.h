#ifndef CLASSIFIER_SERVICE_H
#define CLASSIFIER_SERVICE_H

#include <mlpack/core.hpp>
#include <mlpack/methods/softmax_regression/softmax_regression.hpp>

#include "iris.grpc.pb.h"

class IrisClassifierService final : public iris::IrisClassifier::Service
{
public:
    IrisClassifierService(std::filesystem::path modelFilePath)
    {
        if (!mlpack::data::Load(modelFilePath.string(), "model", m_smr))
        {
            std::cerr << "Failed to load model from: " << modelFilePath << '\n';
            exit(EXIT_FAILURE);
        }
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
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            return grpc::Status(grpc::StatusCode::INTERNAL, "Error during classification");
        }
        response->set_specie(getSpecieName(predictions(0)));
        return grpc::Status::OK;
    }

private:
    mlpack::SoftmaxRegression<arma::dmat> m_smr;

    inline std::string getSpecieName(size_t prediction)
    {
        switch (prediction)
        {
        case 1:
            return "Setosa";
        case 2:
            return "Versicolor";
        case 3:
            return "Virginica";
        default:
            return "Unknown";
        }
    }
};

#endif // CLASSIFIER_SERVICE_H