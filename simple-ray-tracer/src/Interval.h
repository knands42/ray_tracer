#pragma once
#include "Rtweekend.h"

class Interval
{
public:
    double min, max;

    Interval() : min(+infinity), max(-infinity)
    {
    }

    Interval(const double min, const double max) : min(min), max(max)
    {
    }

    [[nodiscard]] auto size() const -> double { return max - min; }

    [[nodiscard]] auto contains(const double root) const -> bool { return min <= root && root <= max; }

    [[nodiscard]] auto surrounds(const double root) const -> bool { return min < root && root < max; }

    [[nodiscard]] auto clamp(const double root) const -> double
    {
        if (root < min)
        {
            return min;
        }
        if (root > max)
        {
            return max;
        }
        return root;
    }

    static const Interval empty, universe;
};

const Interval Interval::empty = Interval(+infinity, -infinity);
const Interval Interval::universe = Interval(-infinity, +infinity);
