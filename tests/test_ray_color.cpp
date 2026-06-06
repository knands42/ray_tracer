// test_ray_color.cpp
//
// Two focused tests for the recursive ray_color function:
//
//   1. Self-hit: a ray that hits a sphere and scatters should NOT
//      infinitely recurse. With depth=0, ray_color returns black.
//      With depth=1, it bounces once then terminates.
//
//   2. Inside-hit: a ray starting slightly inside a sphere should
//      find the exit hit, scatter from there, and eventually
//      terminate — not loop forever.
//
// Build & run:
//   cd build
//   cmake --build . --target ray_color_tests
//   ./ray_color_tests

#include "test_harness.h"
#include "../src/camera.h"
#include "../src/sphere.h"
#include "../src/hittable_list.h"
#include "../src/interval.h"
#include "../src/ray.h"
#include "../src/vec3.h"
#include "../src/rtweekend.h"

#include <cmath>
#include <iostream>

using namespace std;

// ---------------------------------------------------------------------------
// Helper: trace one bounce of ray_color and print what happened.
// Returns the color and writes debug info to stdout.
// ---------------------------------------------------------------------------
static color trace_bounce(const ray& r, int depth, const hittable& world, int bounce_num) {
    if (depth <= 0) {
        cout << "  [bounce " << bounce_num << "] depth=0 → BLACK (terminated)\n";
        return color(0, 0, 0);
    }

    hit_record rec;
    bool hit = world.hit(r, interval(0.001, infinity), rec);

    cout << "  [bounce " << bounce_num << "] origin=" << r.origin()
         << "  dir=" << r.direction()
         << "  hit=" << (hit ? "true" : "false");

    if (hit) {
        cout << "  t=" << rec.t
             << "  p=" << rec.p
             << "  normal=" << rec.normal;
    }
    cout << "\n";

    if (hit) {
        vec3 direction = random_on_hemisphere(rec.normal);
        cout << "  [bounce " << bounce_num << "] scattered dir=" << direction
             << "  dot(dir,normal)=" << dot(direction, rec.normal) << "\n";
        return 0.5 * camera::ray_color(ray(rec.p, direction), depth - 1, world);
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5 * (unit_direction.y() + 1.0);
    color sky = (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
    cout << "  [bounce " << bounce_num << "] missed → sky color=" << sky << "\n";
    return sky;
}

// ---------------------------------------------------------------------------
// TEST 1: Self-hit — recursion terminates with depth limit
//
//   Scene: single sphere at (0,0,-1) with radius 0.5
//   Ray: from (0,0,0) toward (0,0,-1) — hits the sphere
//
//   With depth=0:
//     ray_color returns black immediately. No hit check at all.
//
//   With depth=1:
//     Hits sphere → scatters → next call has depth=0 → black.
//     Result: 0.5 * black = black.
//
//   With depth=2:
//     Hits → scatters → hits or misses → scatters → depth=0 → black.
//
//   The point: depth limit guarantees termination. No infinite loop.
// ---------------------------------------------------------------------------
TEST(self_hit_recursion_terminates) {
    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));

    // Ray from origin toward the sphere
    ray r(point3(0, 0, 0), vec3(0, 0, -1));

    cout << "\n  --- depth=0 ---\n";
    color c0 = camera::ray_color(r, 0, world);
    cout << "  color=" << c0 << "\n";
    ASSERT_NEAR(c0.x(), 0.0, 0.001);
    ASSERT_NEAR(c0.y(), 0.0, 0.001);
    ASSERT_NEAR(c0.z(), 0.0, 0.001);

    cout << "\n  --- depth=1 ---\n";
    color c1 = camera::ray_color(r, 1, world);
    cout << "  color=" << c1 << "\n";
    // depth=1: hits, scatters, next call depth=0 returns black → 0.5*black = black
    ASSERT_NEAR(c1.x(), 0.0, 0.001);
    ASSERT_NEAR(c1.y(), 0.0, 0.001);
    ASSERT_NEAR(c1.z(), 0.0, 0.001);

    cout << "\n  --- depth=5 (should still terminate) ---\n";
    color c5 = camera::ray_color(r, 5, world);
    cout << "  color=" << c5 << "\n";
    // Should be non-negative and finite — the key test is that it doesn't crash
    ASSERT_GT(c5.x(), -0.001);
    ASSERT_GT(c5.y(), -0.001);
    ASSERT_GT(c5.z(), -0.001);

    return {"self_hit_recursion_terminates", true, ""};
}

// ---------------------------------------------------------------------------
// TEST 2: Inside-hit — ray starting inside sphere finds exit, then terminates
//
//   Scene: single sphere at (0,0,-1) with radius 0.5
//   Ray origin: (0,0,-0.9) — inside the sphere
//   Ray direction: (0,0,1) — pointing toward the inner wall
//
//   What should happen:
//     1. The ray hits the sphere at the exit point (t > 0, not tiny)
//     2. It scatters from there using the outward normal
//     3. The scattered ray leaves the sphere
//     4. Eventually it misses everything → sky color
//     5. Recursion terminates via depth limit
//
//   This proves: starting inside does NOT cause infinite bouncing.
// ---------------------------------------------------------------------------
TEST(inside_hit_recursion_terminates) {
    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));

    // Ray starts inside the sphere, pointing toward the exit wall
    ray r(point3(0, 0, -0.9), vec3(0, 0, 1));

    cout << "\n  --- Inside-hit: manual trace with depth=5 ---\n";
    color c = camera::ray_color(r, 5, world);
    cout << "  final color=" << c << "\n";

    // The color should be non-negative (valid) — the key test is termination
    ASSERT_GT(c.x(), -0.001);
    ASSERT_GT(c.y(), -0.001);
    ASSERT_GT(c.z(), -0.001);

    // With enough depth, the ray should eventually escape and see sky.
    // Sky color has y >= 0.5 and z >= 0.7, attenuated by 0.5 each bounce.
    // After a few bounces it may be dim, but it should not be negative.
    cout << "\n  --- Inside-hit: depth=50 (default) ---\n";
    color c50 = camera::ray_color(r, 50, world);
    cout << "  final color=" << c50 << "\n";
    ASSERT_GT(c50.x(), -0.001);
    ASSERT_GT(c50.y(), -0.001);
    ASSERT_GT(c50.z(), -0.001);

    return {"inside_hit_recursion_terminates", true, ""};
}

// ---------------------------------------------------------------------------
// Main
// ---------------------------------------------------------------------------
int main() {
    cout << "Running ray_color recursion tests...\n";
    return run_all_tests();
}
