#pragma once

#include "Hittable.h"
#include "Ray.h"

class Sphere : public Hittable
{
public:
    Sphere(const point3 &center, double radius) : center(center), radius(std::fmax(0, radius))
    {
        // TODO: Initialize the material pointer
    }

    auto hit(const Ray &ray, const Interval root_interval, HitRecord &hit_rec) const -> bool override
    {
        const Vec3 from_origin_to_sphere_center = center - ray.origin();
        const auto a = ray.direction().length_squared();
        const auto h = dot(ray.direction(), from_origin_to_sphere_center);
        const auto c = from_origin_to_sphere_center.length_squared() - (radius * radius);

        const auto discriminant = (h * h) - (a * c);
        if (discriminant < 0)
        {
            return false;
        }

        const auto discriminant_sqrt = std::sqrt(discriminant);

        auto root = (h - discriminant_sqrt) / a;
        if (!root_interval.surrounds(root))
        {
            root = (h + discriminant_sqrt) / a;
            if (!root_interval.surrounds(root))
            {
                return false;
            }
        }

        hit_rec.t = root;
        hit_rec.point = ray.at(hit_rec.t);
        const Vec3 outward_normal = (hit_rec.point - center) / radius;
        hit_rec.set_face_normal(ray, outward_normal);

        return true;
    }

private:
    point3 center;
    double radius;
};
