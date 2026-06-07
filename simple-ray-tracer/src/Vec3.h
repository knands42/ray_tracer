#pragma once

#include <cmath>
#include <complex>
#include <iostream>

class Vec3
{
public:
    double e[3];

    Vec3() : e{0, 0, 0}
    {
    }

    Vec3(const double e0, const double e1, const double e2) : e{e0, e1, e2}
    {
    }

    [[nodiscard]] auto x() const -> double { return e[0]; }
    [[nodiscard]] auto y() const -> double { return e[1]; }
    [[nodiscard]] auto z() const -> double { return e[2]; }

    auto operator-() const -> Vec3 { return {-e[0], -e[1], -e[2]}; }
    auto operator[](const int index) const -> double { return e[index]; }
    auto operator[](const int index) -> double & { return e[index]; }

    auto operator+=(const Vec3 &vec) -> Vec3 &
    {
        e[0] += vec.e[0];
        e[1] += vec.e[1];
        e[2] += vec.e[2];
        return *this;
    }

    auto operator*=(const double scalar) -> Vec3 &
    {
        e[0] *= scalar;
        e[1] *= scalar;
        e[2] *= scalar;
        return *this;
    }

    auto operator/=(const double scalar) -> Vec3 & { return *this *= 1 / scalar; }

    [[nodiscard]] auto length() const -> double { return std::sqrt(length_squared()); }

    /*
     * Same as dot product
     */
    [[nodiscard]] double length_squared() const
    {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    static auto random() -> Vec3
    {
        return {random_double(), random_double(), random_double()};
    }

    static auto random(const double min, const double max) -> Vec3
    {
        return {
            random_double(min, max), random_double(min, max),
            random_double(min, max)
        };
    }
};

using point3 = Vec3;

// Vector Utility Functions

inline auto operator<<(std::ostream &out, const Vec3 &v) -> std::ostream &
{
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline auto operator+(const Vec3 &u, const Vec3 &v) -> Vec3
{
    return {u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]};
}

inline auto operator-(const Vec3 &u, const Vec3 &v) -> Vec3
{
    return {u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]};
}

inline auto operator*(const Vec3 &u, const Vec3 &v) -> Vec3
{
    return {u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]};
}

inline auto operator*(double t, const Vec3 &v) -> Vec3
{
    return {t * v.e[0], t * v.e[1], t * v.e[2]};
}

inline auto operator*(const Vec3 &v, double t) -> Vec3 { return t * v; }

inline auto operator/(const Vec3 &v, double t) -> Vec3 { return (1 / t) * v; }

inline auto dot(const Vec3 &u, const Vec3 &v) -> double
{
    return (u.e[0] * v.e[0]) + (u.e[1] * v.e[1]) + (u.e[2] * v.e[2]);
}

inline auto cross(const Vec3 &vec1, const Vec3 &vec2) -> Vec3
{
    return {
        (vec1.e[1] * vec2.e[2]) - (vec1.e[2] * vec2.e[1]),
        (vec1.e[2] * vec2.e[0]) - (vec1.e[0] * vec2.e[2]),
        (vec1.e[0] * vec2.e[1]) - (vec1.e[1] * vec2.e[0])
    };
}

inline auto unit_vector(const Vec3 &v) -> Vec3 { return v / v.length(); }

/// Returns a random unit vector uniformly distributed over the full unit sphere.
/// Uses rejection sampling: picks random points in [-1,1]³ and normalizes those inside the sphere.
inline auto random_unit_vector() -> Vec3
{
    while (true)
    {
        auto vec = Vec3::random(-1, 1);
        const auto p_squared_length = vec.length_squared();
        if (auto minimum = 1e-160; minimum < p_squared_length && p_squared_length <= 1)
        {
            return vec / sqrt(p_squared_length);
        }
    }
}

/// Returns a random unit vector on the same hemisphere as `normal` (dot product > 0).
/// Guarantees the scattered direction always points away from the surface.
inline auto random_on_hemisphere(const Vec3 &normal) -> Vec3
{
    const Vec3 on_unit_sphere = random_unit_vector();
    if (dot(on_unit_sphere, normal) > 0.0)
    {
        return on_unit_sphere;
    }
    return -on_unit_sphere;
}
