#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <sstream>
#include "CsvWriter.h"

class CsvWriterTest : public ::testing::Test {
protected:
    void SetUp() override {
        testFile = "/tmp/test_output.csv";
        // Clean up any existing test file
        std::filesystem::remove(testFile);
    }

    void TearDown() override {
        // Clean up test file
        std::filesystem::remove(testFile);
    }

    std::string readFileContent(const std::string& filename) {
        std::ifstream file(filename);
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    std::string testFile;
};

TEST_F(CsvWriterTest, BasicWriting) {
    {
        csv::CsvWriter writer(testFile);
        writer << "header1" << "header2" << "header3";
        writer.endrow();
        writer << "value1" << "value2" << "value3";
        writer.endrow();
    } // Writer destructor called here, file should be flushed and closed

    std::string content = readFileContent(testFile);
    std::string expected = "\"header1\",\"header2\",\"header3\"\n\"value1\",\"value2\",\"value3\"\n";
    EXPECT_EQ(content, expected);
}

TEST_F(CsvWriterTest, NumericValues) {
    {
        csv::CsvWriter writer(testFile);
        writer << "int" << "double" << "float";
        writer.endrow();
        writer << 42 << 3.14159 << 2.71f;
        writer.endrow();
    }

    std::string content = readFileContent(testFile);
    std::string expected = "\"int\",\"double\",\"float\"\n42,3.14159,2.71\n";
    EXPECT_EQ(content, expected);
}

TEST_F(CsvWriterTest, EscapingSpecialCharacters) {
    {
        csv::CsvWriter writer(testFile);
        writer << "field with \"quotes\"" << "normal field";
        writer.endrow();
    }

    std::string content = readFileContent(testFile);
    std::string expected = "\"field with \"\"quotes\"\"\",\"normal field\"\n";
    EXPECT_EQ(content, expected);
}

TEST_F(CsvWriterTest, CustomSeparator) {
    {
        csv::CsvWriter writer(testFile, ";");
        writer << "field1" << "field2" << "field3";
        writer.endrow();
        writer << "value1" << "value2" << "value3";
        writer.endrow();
    }

    std::string content = readFileContent(testFile);
    std::string expected = "\"field1\";\"field2\";\"field3\"\n\"value1\";\"value2\";\"value3\"\n";
    EXPECT_EQ(content, expected);
}

TEST_F(CsvWriterTest, EndrowManipulator) {
    {
        csv::CsvWriter writer(testFile);
        writer << "field1" << "field2" << csv::endrow;
        writer << "value1" << "value2" << csv::endrow;
    }

    std::string content = readFileContent(testFile);
    std::string expected = "\"field1\",\"field2\"\n\"value1\",\"value2\"\n";
    EXPECT_EQ(content, expected);
}

TEST_F(CsvWriterTest, FlushManipulator) {
    {
        csv::CsvWriter writer(testFile);
        writer << "field1" << "field2" << csv::flush;
        // File should be flushed but not closed, we can continue writing
        writer << csv::endrow;
        writer << "value1" << "value2" << csv::endrow;
    }

    std::string content = readFileContent(testFile);
    std::string expected = "\"field1\",\"field2\"\n\"value1\",\"value2\"\n";
    EXPECT_EQ(content, expected);
}

TEST_F(CsvWriterTest, MixedDataTypes) {
    {
        csv::CsvWriter writer(testFile);
        writer << "sepalLength" << "sepalWidth" << "petalLength" << "petalWidth" << "type";
        writer.endrow();
        writer << 5.1 << 3.5 << 1.4 << 0.2 << 1; // Simulate Iris data
        writer.endrow();
    }

    std::string content = readFileContent(testFile);
    std::string expected = "\"sepalLength\",\"sepalWidth\",\"petalLength\",\"petalWidth\",\"type\"\n5.1,3.5,1.4,0.2,1\n";
    EXPECT_EQ(content, expected);
}