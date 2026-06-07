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

    Vec3(const double x, const double y, const double z) : e{x, y, z}
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
    [[nodiscard]] auto length_squared() const -> double
    {
        return (e[0] * e[0]) + (e[1] * e[1]) + (e[2] * e[2]);
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

inline auto operator<<(std::ostream &out, const Vec3 &vec) -> std::ostream &
{
    return out << vec.e[0] << ' ' << vec.e[1] << ' ' << vec.e[2];
}

inline auto operator+(const Vec3 &vecU, const Vec3 &vecV) -> Vec3
{
    return {vecU.e[0] + vecV.e[0], vecU.e[1] + vecV.e[1], vecU.e[2] + vecV.e[2]};
}

inline auto operator-(const Vec3 &vecU, const Vec3 &vecV) -> Vec3
{
    return {vecU.e[0] - vecV.e[0], vecU.e[1] - vecV.e[1], vecU.e[2] - vecV.e[2]};
}

inline auto operator*(const Vec3 &vecU, const Vec3 &vecV) -> Vec3
{
    return {vecU.e[0] * vecV.e[0], vecU.e[1] * vecV.e[1], vecU.e[2] * vecV.e[2]};
}

inline auto operator*(double scalar, const Vec3 &vec) -> Vec3
{
    return {scalar * vec.e[0], scalar * vec.e[1], scalar * vec.e[2]};
}

inline auto operator*(const Vec3 &vec, const double scalar) -> Vec3 { return scalar * vec; }

inline auto operator/(const Vec3 &vec, const double scaler) -> Vec3 { return (1 / scaler) * vec; }

inline auto dot(const Vec3 &vecU, const Vec3 &vecV) -> double
{
    return (vecU.e[0] * vecV.e[0]) + (vecU.e[1] * vecV.e[1]) + (vecU.e[2] * vecV.e[2]);
}

inline auto cross(const Vec3 &vecU, const Vec3 &vecV) -> Vec3
{
    return {
        (vecU.e[1] * vecV.e[2]) - (vecU.e[2] * vecV.e[1]),
        (vecU.e[2] * vecV.e[0]) - (vecU.e[0] * vecV.e[2]),
        (vecU.e[0] * vecV.e[1]) - (vecU.e[1] * vecV.e[0])
    };
}

inline auto unit_vector(const Vec3 &vec) -> Vec3 { return vec / vec.length(); }

/// Returns a random unit vector uniformly distributed over the full unit sphere.
/// Uses rejection sampling: picks random points in [-1,1]³ and normalizes those inside the sphere.
inline auto random_unit_vector() -> Vec3
{
    while (true)
    {
        auto vec = Vec3::random(-1, 1);
        const auto p_squared_length = vec.length_squared();
        if (constexpr auto minimum = 1e-160; minimum < p_squared_length && p_squared_length <= 1)
        {
            return vec / sqrt(p_squared_length);
        }
    }
}

/// Returns a random unit vector on the same hemisphere as `normal` (dot product > 0).
/// Guarantees the scattered direction always points away from the surface.
inline auto random_on_hemisphere(const Vec3 &normalVec) -> Vec3
{
    const Vec3 on_unit_sphere = random_unit_vector();
    if (dot(on_unit_sphere, normalVec) > 0.0)
    {
        return on_unit_sphere;
    }
    return -on_unit_sphere;
}
