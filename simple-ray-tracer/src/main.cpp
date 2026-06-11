#include "HittableList.h"
#include "Sphere.h"
#include "Vec3.h"
#include <memory>
#include "Camera.h"

int main()
{
    // Camera
    Camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 10;
    cam.max_depth = 100;


    // Materials
    auto material_ground = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    auto material_center = std::make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
    auto material_left = std::make_shared<Metal>(Color(0.8, 0.8, 0.8), 0.3);
    auto material_right = std::make_shared<Metal>(Color(0.8, 0.6, 0.2), 1.0);

    // World
    HittableList world;
    world.add(make_shared<Sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<Sphere>(point3(0.0, 0.0, -1.2), 0.5, material_center));
    world.add(make_shared<Sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<Sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

    cam.render(world);
}
