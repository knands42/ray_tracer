// test_sphere_intersection.cpp
//
// Two focused tests for understanding recursive ray bouncing:
//
//   1. Self-hit: a ray starting exactly on the sphere surface should NOT
//      register an immediate hit when using an epsilon min_t (0.001).
//      Without epsilon, floating-point can cause a false self-hit at t~=0.
//
//   2. Inside-hit: a ray starting slightly INSIDE a sphere should find
//      a valid EXIT hit at a positive t, not an immediate self-hit.
//
// Build & run:
//   cd build
//   cmake .. -DCMAKE_CXX_STANDARD=26
//   cmake --build . --target sphere_tests
//   ./sphere_tests

#include "test_harness.h"
#include "../src/sphere.h"
#include "../src/interval.h"
#include "../src/ray.h"
#include "../src/vec3.h"
#include "../src/rtweekend.h"

#include <cmath>
#include <iostream>

using namespace std;

// ---------------------------------------------------------------------------
// Helper: check whether a hit occurs and print details (for debugger use)
// ---------------------------------------------------------------------------
static void debug_hit(const char* label, const ray& r, interval ray_t, const sphere& s) {
    hit_record rec;
    bool hit = s.hit(r, ray_t, rec);
    cout << "  [" << label << "] hit=" << (hit ? "true" : "false");
    if (hit) {
        cout << "  t=" << rec.t
             << "  p=" << rec.p
             << "  normal=" << rec.normal
             << "  front_face=" << rec.front_face;
    }
    cout << "\n";
}

// ---------------------------------------------------------------------------
// TEST 1: Self-hit on the same surface
//
//   Sphere: center=(0,0,-1), radius=0.5
//   Ray origin: (0,0,-0.5) — exactly on the sphere surface (top point)
//   Ray direction: (0,1,0) — pointing straight up, outward
//
//   With interval(0, infinity):
//     Depending on floating-point, the solver may or may not find a hit.
//     If it does, t will be extremely tiny (acne).
//
//   With interval(0.001, infinity):
//     The tiny near-zero hit is rejected → no self-hit.
// ---------------------------------------------------------------------------
TEST(self_hit_ignored_with_epsilon) {
    sphere s(point3(0, 0, -1), 0.5);
    hit_record rec;

    // Ray starts exactly on the sphere surface, pointing outward (up)
    ray r(point3(0, 0, -0.5), vec3(0, 1, 0));

    // With min_t = 0, floating-point may produce a spurious hit at t~=0.
    // We just observe what happens — this is the "before fix" state.
    cout << "\n  --- Self-hit test (min_t = 0) ---\n";
    debug_hit("min_t=0", r, interval(0, infinity), s);

    // With epsilon, the same ray must NOT self-hit.
    cout << "  --- Self-hit test (min_t = 0.001) ---\n";
    debug_hit("min_t=0.001", r, interval(0.001, infinity), s);

    bool hit_epsilon = s.hit(r, interval(0.001, infinity), rec);
    ASSERT_FALSE(hit_epsilon);

    return {"self_hit_ignored_with_epsilon", true, ""};
}

// ---------------------------------------------------------------------------
// TEST 2: Ray starting slightly INSIDE the sphere
//
//   Sphere: center=(0,0,-1), radius=0.5
//   Ray origin: (0,0,-0.9) — inside the sphere (0.4 units from center,
//                  radius is 0.5, so 0.1 units from the inner wall)
//   Ray direction: (0,0,1) — pointing toward the inner wall
//
//   Expected:
//     - The ray should hit the sphere at a positive t (the exit point)
//     - t should NOT be near-zero (it is a real geometric intersection)
//     - The hit point should be approximately on the sphere surface
// ---------------------------------------------------------------------------
TEST(inside_hit_finds_exit) {
    sphere s(point3(0, 0, -1), 0.5);
    hit_record rec;

    // Start inside: center is (0,0,-1), radius=0.5
    // (0,0,-0.9) is 0.4 units from center along z, so inside (0.4 < 0.5)
    ray r(point3(0, 0, -0.9), vec3(0, 0, 1));

    cout << "\n  --- Inside-hit test ---\n";
    debug_hit("inside", r, interval(0.001, infinity), s);

    bool hit = s.hit(r, interval(0.001, infinity), rec);

    // Must find a hit
    ASSERT_TRUE(hit);

    // t must be positive and not tiny (this is a real exit, not acne)
    ASSERT_GT(rec.t, 0.01);

    // The hit point should be approximately on the sphere surface.
    // Distance from center to hit point should be ~radius.
    vec3 outward_normal = (rec.p - point3(0, 0, -1)) / 0.5;
    double dist_from_center = outward_normal.length();
    ASSERT_NEAR(dist_from_center, 1.0, 0.001);  // unit vector, so length ~1

    // More directly: distance from center to hit point should be ~radius=0.5
    double dist_len = (rec.p - point3(0, 0, -1)).length();
    ASSERT_NEAR(dist_len, 0.5, 0.001);

    return {"inside_hit_finds_exit", true, ""};
}

// ---------------------------------------------------------------------------
// Main: run all registered tests
// ---------------------------------------------------------------------------
int main() {
    cout << "Running sphere intersection tests...\n";
    return run_all_tests();
}
