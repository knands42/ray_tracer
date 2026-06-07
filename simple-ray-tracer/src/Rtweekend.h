#pragma once

#include <cmath>
#include <limits>
#include "random"

// Constants

constexpr double infinity = std::numeric_limits<double>::infinity();

// Utility Functions

inline auto degrees_to_radians(double degrees) -> double
{
    return degrees * M_PI / 180.0;
}

inline auto random_double() -> double
{
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator = std::mt19937(std::random_device{}());
    return distribution(generator);
}

inline auto random_double(double min, double max) -> double
{
    return min + ((max - min) * random_double());
}
