#pragma once

#include "Color.h"
#include "Hittable.h"
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

class Lambertian : public Material
{
public:
    Lambertian(const Color &albedo) : albedo(albedo)
    {
    }

    // Lambertian (diffuse) reflectance can either always scatter and attenuate light according to its reflectance R,
    // or it can sometimes scatter (with probability 1−R) with no attenuation (where a ray that isn't scattered is just absorbed into the material).
    // It could also be a mixture of both those strategies.
    // I will choose to always scatter, so implementing Lambertian materials becomes a simple task
    auto scatter(
        const Ray &rIn, const HitRecord &hitRec, Color &attenuation, Ray &scattered
    ) const -> bool override
    {
        auto scatterDirection = hitRec.normal + random_unit_vector();
        if (scatterDirection.nearZero()) scatterDirection = hitRec.normal;

        scattered = Ray(hitRec.point, scatterDirection);
        attenuation = albedo;
        return true;
    }

private:
    Color albedo;
};

class Metal : public Material
{
public:
    Metal(const Color &albedo) : albedo(albedo)
    {
    }

    auto scatter(const Ray &rIn, const HitRecord &hitRec, Color &attenuation, Ray &scattered)
    const -> bool override
    {
        const Vec3 reflected = reflect(rIn.direction(), hitRec.normal);
        scattered = Ray(hitRec.point, reflected);
        attenuation = albedo;
        return true;
    }

private:
    Color albedo;
};
