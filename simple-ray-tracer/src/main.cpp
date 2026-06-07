#include "hittable_list.h"
#include "sphere.h"
#include "vec3.h"
#include <memory>
#include "camera.h"

int main()
{
    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 10;
    cam.max_depth = 100;

    // World
    hittable_list world;
    world.add(std::make_shared<sphere>(cam.viewport_center, 0.5));
    world.add(std::make_shared<sphere>(point3(0, -100.5, -1), 100));

    cam.render(world);
}
