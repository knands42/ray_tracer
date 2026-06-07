#include "HittableList.h"
#include "Sphere.h"
#include "Vec3.h"
#include <memory>
#include "Camera.h"

int main()
{
    Camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 10;
    cam.max_depth = 100;

    // World
    HittableList world;
    world.add(std::make_shared<Sphere>(cam.viewport_center, 0.5));
    world.add(std::make_shared<Sphere>(point3(0, -100.5, -1), 100));

    cam.render(world);
}
