#pragma once

#include "hittable.h"
#include "ray.h"

class sphere : public hittable
{
public:
    sphere(const point3 &center, double radius) : center(center), radius(std::fmax(0, radius))
    {
    }

    bool hit(const ray &r, interval ray_t, hit_record &hit_rec) const override
    {
        vec3 oc = center - r.origin();
        auto a = r.direction().length_squared();
        auto h = dot(r.direction(), oc);
        auto c = oc.length_squared() - radius * radius;

        auto discriminant = h * h - a * c;
        if (discriminant < 0)
        {
            return false;
        }

        auto discriminant_sqrt = std::sqrt(discriminant);

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
        vec3 outward_normal = (hit_rec.p - center) / radius;
        hit_rec.set_face_normal(r, outward_normal);

        return true;
    }

private:
    point3 center;
    double radius;
};
