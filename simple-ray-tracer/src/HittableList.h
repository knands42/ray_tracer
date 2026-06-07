#pragma once

#include "Hittable.h"
#include "memory"

#include <vector>

#include "Interval.h"
#include "Ray.h"

class HittableList : public Hittable
{
public:
    std::vector<std::shared_ptr<Hittable> > objects;

    HittableList() = default;

    explicit HittableList(const std::shared_ptr<Hittable> &object) { add(object); }

    void clear() { objects.clear(); }

    void add(const std::shared_ptr<Hittable> &object)
    {
        objects.push_back(object);
    }

    auto hit(const Ray &ray, const Interval ray_t, HitRecord &rec) const -> bool override
    {
        HitRecord temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_t.max;

        for (const auto &object: objects)
        {
            if (object->hit(ray, Interval(ray_t.min, closest_so_far), temp_rec))
            {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }

        return hit_anything;
    }
};
