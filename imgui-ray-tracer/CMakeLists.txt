cmake_minimum_required(VERSION 3.16)
project(imgui_ray_tracer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

find_package(OpenGL REQUIRED)

# Glm
FetchContent_Declare(
        glm
        GIT_REPOSITORY	https://github.com/g-truc/glm.git
)

FetchContent_MakeAvailable(glm)

# ImGui (docking branch)
FetchContent_Declare(
        imgui
        GIT_REPOSITORY	https://github.com/ocornut/imgui.git
        GIT_TAG		5a76f2adf1b0403b86a45010121fb32a6bff8680
)
FetchContent_MakeAvailable(imgui)

# ImGui core sources
set(IMGUI_SOURCES
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
)

# ImGui backends (GLFW + OpenGL3)
set(IMGUI_BACKEND_SOURCES
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

add_executable(imgui_ray_tracer
        src/main.cpp
        src/Application.cpp
        ${IMGUI_SOURCES}
        ${IMGUI_BACKEND_SOURCES}
        src/Renderer.h
        src/Renderer.cpp
        src/Camera.h
        src/Camera.cpp
        ../simple-ray-tracer/src/Material.h
)

target_include_directories(imgui_ray_tracer PRIVATE
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(imgui_ray_tracer PRIVATE
        glm::glm
        OpenGL::GL
        glfw
)

add_custom_target(run_imgui_ray_tracer
        COMMAND $<TARGET_FILE:imgui_ray_tracer>
        DEPENDS imgui_ray_tracer
)