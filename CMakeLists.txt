cmake_minimum_required(VERSION 3.16)
project(ray_tracer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ray_tracer
        src/main.cpp
        src/vec3.h
        src/hittable.h
        src/sphere.h
        src/color.h
)

add_executable(sphere_tests
        tests/test_sphere_intersection.cpp
        tests/test_harness.h
        tests/test_ray_color.cpp
        src/vec3.h
        src/hittable.h
        src/sphere.h
        src/color.h
        src/interval.h
        src/ray.h
        src/rtweekend.h
)

add_custom_target(run_tests
        COMMAND sphere_tests
        DEPENDS sphere_tests
)

add_custom_target(run
        COMMAND $<TARGET_FILE:ray_tracer>
        DEPENDS ray_tracer
)
