#pragma once

#include "hittable.h"
#include "ray.h"

class sphere : public hittable
{
public:
    sphere(const point3 &center, double radius) : center(center), radius(std::fmax(0, radius))
    {
    }

    auto hit(const ray &r, const interval ray_t, hit_record &hit_rec) const -> bool override
    {
        const vec3 from_camera_to_center = center - r.origin();
        auto a = r.direction().length_squared();
        auto h = dot(r.direction(), from_camera_to_center);
        auto c = from_camera_to_center.length_squared() - (radius * radius);

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
        hit_rec.p = r.at(hit_rec.t);
        const vec3 outward_normal = (hit_rec.p - center) / radius;
        hit_rec.set_face_normal(r, outward_normal);

        return true;
    }

private:
    point3 center;
    double radius;
};
