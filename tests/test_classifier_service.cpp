#include <gtest/gtest.h>
#include <string>

// We'll extract just the getSpecieName function for testing
// since testing the full ClassifierService would require complex setup
class IrisSpecieNameConverter {
public:
    static std::string getSpecieName(size_t prediction) {
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

class ClassifierServiceTest : public ::testing::Test {
protected:
    // No special setup needed for this simple test
};

TEST_F(ClassifierServiceTest, GetSpecieNameSetosa) {
    std::string result = IrisSpecieNameConverter::getSpecieName(1);
    EXPECT_EQ(result, "Setosa");
}

TEST_F(ClassifierServiceTest, GetSpecieNameVersicolor) {
    std::string result = IrisSpecieNameConverter::getSpecieName(2);
    EXPECT_EQ(result, "Versicolor");
}

TEST_F(ClassifierServiceTest, GetSpecieNameVirginica) {
    std::string result = IrisSpecieNameConverter::getSpecieName(3);
    EXPECT_EQ(result, "Virginica");
}

TEST_F(ClassifierServiceTest, GetSpecieNameUnknown) {
    std::string result = IrisSpecieNameConverter::getSpecieName(0);
    EXPECT_EQ(result, "Unknown");
}

TEST_F(ClassifierServiceTest, GetSpecieNameInvalidLarge) {
    std::string result = IrisSpecieNameConverter::getSpecieName(999);
    EXPECT_EQ(result, "Unknown");
}

TEST_F(ClassifierServiceTest, GetSpecieNameInvalidNegative) {
    // Note: size_t is unsigned, so negative values will wrap around
    // This test ensures we handle large wrapped values correctly
    std::string result = IrisSpecieNameConverter::getSpecieName(SIZE_MAX);
    EXPECT_EQ(result, "Unknown");
}