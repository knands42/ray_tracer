cmake_minimum_required(VERSION 3.16)
project(simple_ray_tracer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(simple_ray_tracer
        src/main.cpp
        src/Vec3.h
        src/Hittable.h
        src/Sphere.h
        src/Color.h
)

add_custom_target(run_simple_ray_tracer
        COMMAND $<TARGET_FILE:simple_ray_tracer>
        DEPENDS simple_ray_tracer
)
