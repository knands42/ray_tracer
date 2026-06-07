#pragma once
#include "Color.h"
#include "Ray.h"

class HitRecord;

class Material
{
public:
    virtual ~Material() = default;

    virtual auto scatter(
        const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered
    ) const -> bool
    {
        return false;
    }
};
