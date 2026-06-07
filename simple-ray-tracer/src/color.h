#pragma once

#include "interval.h"
#include "vec3.h"

using color = vec3;

inline void write_color(std::ostream &out, const color &pixel_color) {
  const auto r = pixel_color.x();
  const auto g = pixel_color.y();
  const auto b = pixel_color.z();

  static const interval intensity(0.000, 0.999);
  const int r_byte = int(256 * intensity.clamp(r));
  const int g_byte = int(256 * intensity.clamp(g));
  const int b_byte = int(256 * intensity.clamp(b));

  out << r_byte << ' ' << g_byte << ' ' << b_byte << '\n';
}
