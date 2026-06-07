#include "Renderer.h"
#include <chrono>

Renderer::Renderer(uint32_t viewPortWidth, uint32_t viewPortHeight)
{
    m_ImageData.resize(static_cast<size_t>(viewPortWidth) * static_cast<size_t>(viewPortHeight));
}

void Renderer::Render()
{
    auto start = std::chrono::high_resolution_clock::now();

    for (uint32_t y = 0; y < m_FinalImage->height; y++)
    {
        for (uint32_t x = 0; x < m_FinalImage->width; x++)
        {
            glm::vec2 coord = {
                static_cast<float>(x) / static_cast<float>(m_FinalImage->width),
                static_cast<float>(y) / static_cast<float>(m_FinalImage->height)
            };
            coord = coord * 2.0f - 1.0f; // [-1; 1]
            m_ImageData[x + y * m_FinalImage->width] = PerPixel(coord);
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    m_LastRenderTime = std::chrono::duration<float, std::milli>(end - start).count();
}

void Renderer::OnResize(uint32_t width, uint32_t height)
{
    if (m_FinalImage)
    {
        if (m_FinalImage->width == static_cast<int>(width) && m_FinalImage->height == static_cast<int>(height))
        {
            return;
        }
    }

    m_FinalImage = std::make_shared<GLFWimage>();
    m_FinalImage->width = width;
    m_FinalImage->height = height;
    m_ImageData.resize(width * height);
}

uint32_t Renderer::PerPixel(glm::vec2 coord)
{
    glm::vec3 rayOrigin(0.0f, 0.0f, -2.0f);
    glm::vec3 projectionPlane(coord.x, coord.y, 0.0f);
    glm::vec3 rayDirection = glm::normalize(projectionPlane - rayOrigin);

    float radius = 0.5f;
    rayDirection = glm::normalize(rayDirection);

    float a = glm::dot(rayDirection, rayDirection);
    float b = 2.0f * glm::dot(rayOrigin, rayDirection);
    float c = glm::dot(rayOrigin, rayOrigin) - radius * radius;

    float discriminant = b * b - 4.0f * a * c;
    if (discriminant >= 0.0f)
    {
        float sqrtDiscriminant = glm::sqrt(discriminant);
        float t0 = (-b - sqrtDiscriminant) / (2.0f * a);
        float t1 = (-b + sqrtDiscriminant) / (2.0f * a);
        if (t0 >= 0.0f || t1 >= 0.0f) return 0xffff00ff;
    }

    return 0xff000000;
}
