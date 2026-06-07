# pragma once

#include <memory>

#include "Interval.h"
#include "Ray.h"
#include "Vec3.h"

class Material;

class HitRecord
{
public:
    point3 point;
    Vec3 normal;
    std::shared_ptr<Material> mat;
    double t{};
    bool front_face{};

    void set_face_normal(const Ray &ray, const Vec3 &outward_normal)
    {
        front_face = dot(ray.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable
{
public:
    virtual ~Hittable() = default;

    virtual auto hit(const Ray &ray, Interval ray_t, HitRecord &rec) const -> bool = 0;
};
