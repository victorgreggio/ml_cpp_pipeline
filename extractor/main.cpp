#include <CLI/CLI.hpp>
#include <highfive/H5DataSet.hpp>
#include <highfive/H5DataSpace.hpp>
#include <highfive/H5File.hpp>
#include <sqlpp23/sqlpp23.h>
#include <sqlpp23/postgresql/postgresql.h>

#include "iris.h"

int main(int argc, char *argv[])
{
    CLI::App app{"Extractor for Iris dataset"};
    argv = app.ensure_utf8(argv);

    std::string host;
    app.add_option("-H,--host", host, "Database host")
        ->default_val("localhost")
        ->capture_default_str();

    std::string user;
    app.add_option("-u,--user", user, "Database user")
        ->default_val("iris")
        ->capture_default_str();

    std::string password;
    app.add_option("-p,--password", password, "Database password")
        ->default_val("iris")
        ->capture_default_str();

    int port;
    app.add_option("-P,--port", port, "Database port")
        ->default_val(5432)
        ->capture_default_str();

    std::string dbname;
    app.add_option("-d,--dbname", dbname, "Database name")
        ->default_val("iris")
        ->capture_default_str();

    std::string outputFilePath;
    app.add_option("-o,--output", outputFilePath, "Output file path")
        ->default_val("output.hdf")
        ->capture_default_str();

    CLI11_PARSE(app, argc, argv);

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

    sqlpp::postgresql::connection_config config;
    config.dbname = dbname;
    config.user = user;
    config.password = password;
    config.host = host;
    config.port = port;
    // config.debug = true;

    try
    {
        HighFive::File file(outputPath.string(), HighFive::File::ReadWrite | HighFive::File::Create | HighFive::File::Truncate);

        sqlpp::postgresql::connection connection(config);

        iris::Iris iris{};
        std::map<std::string, HighFive::Group> groups;
        for (const auto &specie : connection(select(iris.specie).group_by(iris.specie).from(iris)))
        {
            std::string specieName = std::string(specie.specie.value());
            if (groups.find(specieName) == groups.end())
            {
                auto group = file.createGroup(specieName);
                groups[specieName] = group;
            }
        }
        
        for (const auto &row : connection(select(iris.id, iris.petalL, iris.petalW, iris.sepalL, iris.sepalW, iris.specie).from(iris)))
        {
            std::string specieName = std::string(row.specie.value());
            auto &group = groups[specieName];

            std::string dataSetname = "measurement_" + std::to_string(row.id);
            std::vector<double> measurements{row.petalL.value(), row.petalW.value(), row.sepalL.value(), row.sepalW.value()};
            std::vector<size_t> dims{measurements.size()};

            auto dataSet = group.createDataSet<double>(dataSetname, HighFive::DataSpace(dims));
            dataSet.write(measurements);
        }

        file.flush();
        std::cout << "Data successfully extracted to " << outputPath << '\n';
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}