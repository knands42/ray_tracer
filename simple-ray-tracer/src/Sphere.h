#pragma once

#include "Hittable.h"
#include "Ray.h"

class Sphere : public Hittable
{
public:
    Sphere(const point3 &center, double radius) : center(center), radius(std::fmax(0, radius))
    {
    }

    auto hit(const Ray &ray, const Interval ray_t, hit_record &hit_rec) const -> bool override
    {
        const Vec3 from_camera_to_center = center - ray.origin();
        const auto a = ray.direction().length_squared();
        const auto h = dot(ray.direction(), from_camera_to_center);
        const auto c = from_camera_to_center.length_squared() - (radius * radius);

        const auto discriminant = (h * h) - (a * c);
        if (discriminant < 0)
        {
            return false;
        }

        const auto discriminant_sqrt = std::sqrt(discriminant);

        auto root = (h - discriminant_sqrt) / a;
        if (!ray_t.surrounds(root))
        {
            root = (h + discriminant_sqrt) / a;
            if (!ray_t.surrounds(root))
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
