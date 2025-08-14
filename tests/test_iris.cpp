#include <gtest/gtest.h>
#include "Iris.h"

class IrisTest : public ::testing::Test {
protected:
    // No special setup needed
};

TEST_F(IrisTest, IrisTypeEnumValues) {
    EXPECT_EQ(static_cast<int>(IrisType::Setosa), 1);
    EXPECT_EQ(static_cast<int>(IrisType::Versicolor), 2);
    EXPECT_EQ(static_cast<int>(IrisType::Virginica), 3);
}

TEST_F(IrisTest, IrisStructInitialization) {
    Iris iris;
    iris.sepalLength = 5.1;
    iris.sepalWidth = 3.5;
    iris.petalLength = 1.4;
    iris.petalWidth = 0.2;
    iris.type = IrisType::Setosa;

    EXPECT_DOUBLE_EQ(iris.sepalLength, 5.1);
    EXPECT_DOUBLE_EQ(iris.sepalWidth, 3.5);
    EXPECT_DOUBLE_EQ(iris.petalLength, 1.4);
    EXPECT_DOUBLE_EQ(iris.petalWidth, 0.2);
    EXPECT_EQ(iris.type, IrisType::Setosa);
}

TEST_F(IrisTest, IrisStructCopy) {
    Iris iris1;
    iris1.sepalLength = 6.2;
    iris1.sepalWidth = 2.9;
    iris1.petalLength = 4.3;
    iris1.petalWidth = 1.3;
    iris1.type = IrisType::Versicolor;

    Iris iris2 = iris1;  // Copy construction

    EXPECT_DOUBLE_EQ(iris2.sepalLength, 6.2);
    EXPECT_DOUBLE_EQ(iris2.sepalWidth, 2.9);
    EXPECT_DOUBLE_EQ(iris2.petalLength, 4.3);
    EXPECT_DOUBLE_EQ(iris2.petalWidth, 1.3);
    EXPECT_EQ(iris2.type, IrisType::Versicolor);
}

TEST_F(IrisTest, IrisTypeComparison) {
    IrisType type1 = IrisType::Setosa;
    IrisType type2 = IrisType::Setosa;
    IrisType type3 = IrisType::Virginica;

    EXPECT_EQ(type1, type2);
    EXPECT_NE(type1, type3);
    EXPECT_NE(type2, type3);
}