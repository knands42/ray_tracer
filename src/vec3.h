#pragma once

#include <cmath>
#include <iostream>

class vec3
{
public:
    double e[3];

    vec3() : e{0, 0, 0}
    {
    }

    vec3(double e0, double e1, double e2) : e{e0, e1, e2}
    {
    }

    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    auto operator-() const -> vec3 { return vec3(-e[0], -e[1], -e[2]); }
    auto operator[](int i) const -> double { return e[i]; }
    auto operator[](int i) -> double & { return e[i]; }

    auto operator+=(const vec3 &v) -> vec3 &
    {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    auto operator*=(double t) -> vec3 &
    {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    auto operator/=(double t) -> vec3 & { return *this *= 1 / t; }

    auto length() const -> double { return std::sqrt(length_squared()); }

    double length_squared() const
    {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }
};

using point3 = vec3;

// Vector Utility Functions

inline auto operator<<(std::ostream &out, const vec3 &v) -> std::ostream &
{
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline auto operator+(const vec3 &u, const vec3 &v) -> vec3
{
    return {u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]};
}

inline auto operator-(const vec3 &u, const vec3 &v) -> vec3
{
    return {u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]};
}

inline auto operator*(const vec3 &u, const vec3 &v) -> vec3
{
    return {u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]};
}

inline auto operator*(double t, const vec3 &v) -> vec3
{
    return {t * v.e[0], t * v.e[1], t * v.e[2]};
}

inline auto operator*(const vec3 &v, double t) -> vec3 { return t * v; }

inline auto operator/(const vec3 &v, double t) -> vec3 { return (1 / t) * v; }

inline auto dot(const vec3 &u, const vec3 &v) -> double
{
    return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3 &vec1, const vec3 &vec2)
{
    return {
        (vec1.e[1] * vec2.e[2]) - (vec1.e[2] * vec2.e[1]),
        (vec1.e[2] * vec2.e[0]) - (vec1.e[0] * vec2.e[2]),
        (vec1.e[0] * vec2.e[1]) - (vec1.e[1] * vec2.e[0])
    };
}

inline auto unit_vector(const vec3 &v) -> vec3 { return v / v.length(); }
