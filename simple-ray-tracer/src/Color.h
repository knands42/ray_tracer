#pragma once

#include "Interval.h"
#include "Vec3.h"

using Color = Vec3;

inline double linear_to_gamma(double linear_component)
{
    if (linear_component > 0)
    {
        return std::sqrt(linear_component);
    }
    return 0;
}

inline void write_color(std::ostream &out, const Color &pixel_color)
{
    auto red = pixel_color.x();
    auto green = pixel_color.y();
    auto blue = pixel_color.z();

    // Apply a linear to gamma transform for gamma 2
    red = linear_to_gamma(red);
    green = linear_to_gamma(green);
    blue = linear_to_gamma(blue);

    // Translate the [0,1] component values to the byte range [0,255].
    static const Interval intensity(0.000, 0.999);
    const int r_byte = static_cast<int>(256 * intensity.clamp(red));
    const int g_byte = static_cast<int>(256 * intensity.clamp(green));
    const int b_byte = static_cast<int>(256 * intensity.clamp(blue));

    out << r_byte << ' ' << g_byte << ' ' << b_byte << '\n';
}
