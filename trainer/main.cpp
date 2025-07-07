#include <CLI/CLI.hpp>
#include <mlpack/core.hpp>
#include <mlpack/methods/softmax_regression/softmax_regression.hpp>

#define ACCURACY_THRESHOLD 90.0

int main(int argc, char *argv[])
{
    CLI::App app{"Trainer for Iris dataset"};
    argv = app.ensure_utf8(argv);

    std::string inputFilePath;
    app.add_option("-i,--input", inputFilePath, "Input file path")
        ->default_val("input.csv")
        ->capture_default_str();

    std::string outputFilePath;
    app.add_option("-o,--output", outputFilePath, "Output file path")
        ->default_val("output.xml")
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

    std::filesystem::path outputPath(outputFilePath);
    if (outputPath.is_relative())
    {
        outputPath = std::filesystem::current_path() / outputPath;
    }
       
    auto outputFolderPath = outputPath;
    outputFolderPath.remove_filename();
    if (!std::filesystem::exists(outputFolderPath))
    {
        std::cerr << "Output path does not exists: " << outputFolderPath << '\n';
        return EXIT_FAILURE;
    }

    // Load the dataset
    arma::mat dataset;
    mlpack::data::DatasetInfo info;
    if (!mlpack::data::Load(inputPath.string(), dataset, info))
    {
        std::cerr << "Failed to load dataset from: " << inputPath << '\n';
        return EXIT_FAILURE;
    }

    auto numOfClasses = info.NumMappings(4);
    std::cout << "Number of dimensions: " << info.Dimensionality() << std::endl;
    std::cout << "Number of classes: " << numOfClasses << std::endl;

    // Extract the last row which contains labels
    auto labels = arma::conv_to<arma::Row<size_t>>::from(dataset.row(dataset.n_rows - 1));
    dataset.shed_row(dataset.n_rows  -1);

    // Split the dataset into training and testing sets
    const double testRatio = 0.2; // 20% for testing
    arma::mat trainDataset, testDateset;
    arma::Row<size_t> trainLabels, testLabels;
    mlpack::data::Split(dataset, labels, trainDataset, testDateset, trainLabels, testLabels, testRatio);

    // Train the model
    mlpack::SoftmaxRegression smr(dataset.n_cols, numOfClasses);
    smr.Train(trainDataset, trainLabels, numOfClasses);
    std::cout << "Model trained successfully." << std::endl;

    // Evaluate the model
    arma::Row<size_t> predictions;
    smr.Classify(testDateset, predictions);
    auto correctPredictions = arma::accu(predictions == testLabels);
    auto accuracy = static_cast<double>(correctPredictions) / testLabels.n_elem * 100.0;
    std::cout << "Model accuracy: " << accuracy << "%" << std::endl;
    
    // Check if the accuracy is below a threshold
    if (accuracy < ACCURACY_THRESHOLD)
    {
        std::cerr << "Model accuracy is below 90%. Consider retraining with more data or adjusting parameters." << std::endl;
        return EXIT_FAILURE;
    }

    // Save the model
    std::string modelFilePath = outputPath.string();
    if (!mlpack::data::Save(modelFilePath, "model", smr))
    {
        std::cerr << "Failed to save model to: " << modelFilePath << '\n';
        return EXIT_FAILURE;
    }

    std::cout << "Model saved to: " << modelFilePath << std::endl;
    return EXIT_SUCCESS;    
}