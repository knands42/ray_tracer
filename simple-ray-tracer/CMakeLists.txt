cmake_minimum_required(VERSION 3.16)
project(simple_ray_tracer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(simple_ray_tracer
        src/main.cpp
        src/vec3.h
        src/hittable.h
        src/sphere.h
        src/color.h
)

add_custom_target(run
        COMMAND $<TARGET_FILE:simple_ray_tracer>
        DEPENDS simple_ray_tracer
)
