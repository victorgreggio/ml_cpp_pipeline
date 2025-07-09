#include <CLI/CLI.hpp>
#include <highfive/H5DataSet.hpp>
#include <highfive/H5DataSpace.hpp>
#include <highfive/H5File.hpp>
#include "Iris.h"
#include "CsvWriter.h"

int main(int argc, char *argv[])
{
    CLI::App app{"Wrangler for Iris dataset"};
    argv = app.ensure_utf8(argv);

    std::string inputFilePath;
    app.add_option("-i,--input", inputFilePath, "Input file path")
        ->default_val("input.hdf")
        ->capture_default_str();

    std::string outputFilePath;
    app.add_option("-o,--output", outputFilePath, "Output file path")
        ->default_val("output.csv")
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
    if (!std::filesystem::exists(outputFolderPath.remove_filename()))
    {
        std::cerr << "Output path does not exists: " << outputFolderPath << '\n';
        return EXIT_FAILURE;
    }
    try
    {
        std::vector<Iris> irisData;

        HighFive::File inputFile(inputPath.string(), HighFive::File::ReadOnly);
        auto &species = inputFile.listObjectNames();
        for (const auto &specie : species)
        {
            auto group = inputFile.getGroup(specie);

            auto dataSetNames = group.listObjectNames();
            for (const auto &dataSetName : dataSetNames)
            {
                auto dataSet = group.getDataSet(dataSetName);
                std::vector<double> measurements;
                dataSet.read(measurements);

                std::string specieName = specie;
                std::transform(specie.begin(), specie.end(), specieName.begin(),
                               [](unsigned char c)
                               { return std::tolower(c); });

                Iris iris;
                if (specieName.find("setosa") != std::string::npos)
                {
                    iris.type = IrisType::Setosa;
                }
                else if (specieName.find("versicolor") != std::string::npos)
                {
                    iris.type = IrisType::Versicolor;
                }
                else if (specieName.find("virginica") != std::string::npos)
                {
                    iris.type = IrisType::Virginica;
                }
                else
                {
                    std::cerr << "Unknown species: " << specieName << '\n';
                    continue;
                }

                iris.petalLength = measurements[0];
                iris.petalWidth = measurements[1];
                iris.sepalLength = measurements[2];
                iris.sepalWidth = measurements[3];
                irisData.push_back(iris);
            }
        }

        csv::CsvWriter csvWriter(outputPath.string());
        csvWriter << "sepalLength" << "sepalWidth" << "petalLength" << "petalWidth" << "type";
        csvWriter.endrow();

        for (const auto &iris : irisData)
        {
            csvWriter << iris.sepalLength << iris.sepalWidth
                      << iris.petalLength << iris.petalWidth
                      << static_cast<int>(iris.type);
            csvWriter.endrow();
        }
        csvWriter.flush();

        std::cout << "Data wrangling completed successfully. Output written to: " << outputPath << '\n';
        return EXIT_SUCCESS;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
}