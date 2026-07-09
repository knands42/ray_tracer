#pragma once

#include "Vec3.h"

class Ray
{
public:
    Ray() = default;

    Ray(const point3 &origin, const Vec3 &direction)
        : orig(origin), dir(direction)
    {
    }

    [[nodiscard]] auto origin() const -> const point3 & { return orig; }
    [[nodiscard]] auto direction() const -> const Vec3 & { return dir; }
    [[nodiscard]] auto at(const double t) const -> point3 { return orig + t * dir; }

private:
    point3 orig;
    Vec3 dir;
};
