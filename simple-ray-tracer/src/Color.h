#pragma once

#include "Interval.h"
#include "Vec3.h"

using color = Vec3;

inline void write_color(std::ostream &out, const color &pixel_color)
{
    const auto red = pixel_color.x();
    const auto green = pixel_color.y();
    const auto blue = pixel_color.z();

    static const Interval intensity(0.000, 0.999);
    const int r_byte = static_cast<int>(256 * intensity.clamp(red));
    const int g_byte = static_cast<int>(256 * intensity.clamp(green));
    const int b_byte = static_cast<int>(256 * intensity.clamp(blue));

    out << r_byte << ' ' << g_byte << ' ' << b_byte << '\n';
}
