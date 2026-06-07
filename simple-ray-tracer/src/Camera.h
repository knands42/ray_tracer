#pragma once
#include "Color.h"
#include "Rtweekend.h"
#include "Vec3.h"

class Camera
{
public:
    double aspect_ratio = 16.0 / 9.0; // Ratio of image width over height
    int image_width = 400; // Rendered image width in pixel count
    Vec3 viewport_center = Vec3(0, 0, -1);
    int samples_per_pixel = 10; // Count of random samples for each pixel
    int max_depth = 10; // Maximum number of ray bounces into scene

    void render(const Hittable &world)
    {
        initialize();

        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; j++)
        {
            std::clog << "\rScanline remaining: " << (image_height - j) << std::flush;
            for (int i = 0; i < image_width; i++)
            {
                Color pixel_color(0, 0, 0);
                for (int sample = 0; sample < samples_per_pixel; sample++)
                {
                    Ray ray = get_ray(i, j);
                    pixel_color += ray_color(ray, max_depth, world);
                }
                write_color(std::cout, pixel_samples_scale * pixel_color);
            }
        }

        std::clog << "\rDone.\t\n";
    }

private:
    int image_height = 225; // Rendered image height
    double pixel_samples_scale = 1.0; // Color scale factor for a sum of pixel samples
    point3 pixel00_loc; // Location of pixel 0, 0
    Vec3 pixel_delta_u; // Offset to pixel to the right
    Vec3 pixel_delta_v; // Offset to pixel below
    point3 camera_center;

    void initialize()
    {
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        pixel_samples_scale = 1.0 / samples_per_pixel;

        // Camera
        constexpr auto focal_length = 1.0;
        constexpr auto viewport_height = 2.0;
        const auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);
        camera_center = point3(0, 0, 0);
        viewport_center = Vec3(0, 0, -focal_length);

        // Calculate the vectors across the horizontal and down the vertical viewport
        // edges.
        const auto viewport_u = Vec3(viewport_width, 0, 0);
        const auto viewport_v = Vec3(0, -viewport_height, 0);

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate the location of the upper left pixel.
        const auto viewport_upper_left = camera_center + viewport_center -
                                         viewport_u / 2 - viewport_v / 2;
        pixel00_loc =
                viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

    [[nodiscard]] auto get_ray(const int row, const int col) const -> Ray
    {
        const auto offset = sample_square();
        const auto pixel_sample = pixel00_loc
                                  + ((row + offset.x()) * pixel_delta_u)
                                  + ((col + offset.y()) * pixel_delta_v);

        const auto ray_origin = camera_center;
        const auto ray_direction = pixel_sample - ray_origin;

        return {ray_origin, ray_direction};
    }

    static auto sample_square() -> Vec3
    {
        // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
        return {random_double() - 0.5, random_double() - 0.5, 0};
    }

    static auto ray_color(const Ray &r, int depth, const Hittable &world) -> Color
    {
        if (depth <= 0)
        {
            return {0, 0, 0};
        }

        if (HitRecord hit_rec; world.hit(r, Interval(0.001, infinity), hit_rec))
        {
            Vec3 direction = hit_rec.normal + random_unit_vector();
            return 0.3 * ray_color(Ray(hit_rec.point, direction), depth - 1, world);
        }

        const Vec3 unit_direction = unit_vector(r.direction());
        // Linear interpolation
        // blendedValue=(1−a)⋅startValue+a⋅endValue
        // or
        // blendedValue=startValue + a * (endValue - startValue)
        const auto a = 0.5 * (unit_direction.y() + 1.0);
        const auto startValue = Color(1.0, 1.0, 1.0); // white
        const auto endValue = Color(0.5, 0.7, 1.0); // blue

        return (1.0 - a) * startValue + a * endValue;
    }
};
