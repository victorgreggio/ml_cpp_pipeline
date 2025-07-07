#ifndef IRIS_H
#define IRIS_H

#include <string>

enum class IrisType
{
    Setosa = 1,
    Versicolor = 2,
    Virginica = 3
};

struct Iris
{
    double sepalLength;
    double sepalWidth;
    double petalLength;
    double petalWidth;
    IrisType type;
};

#endif // IRIS_H