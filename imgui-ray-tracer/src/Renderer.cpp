#include "Renderer.h"
#include <chrono>

namespace Utils
{
    static uint32_t ConvertToRGBA(glm::vec4 &color)
    {
        auto r = static_cast<uint8_t>(color.r * 255.0f);
        auto g = static_cast<uint8_t>(color.g * 255.0f);
        auto b = static_cast<uint8_t>(color.b * 255.0f);
        auto a = static_cast<uint8_t>(color.a * 255.0f);

        return (a << 24) | (b << 16) | (g << 8) | r;
    }
}

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

            glm::vec4 color = PerPixel(coord);
            color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
            m_ImageData[x + (y * m_FinalImage->width)] = Utils::ConvertToRGBA(color);
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

glm::vec4 Renderer::PerPixel(glm::vec2 coord)
{
    glm::vec3 rayOrigin(0.0f, 0.0f, 2.0f);
    glm::vec3 rayDirection(coord.x, coord.y, -1.0f);
    glm::vec3 sphereCenter = {0.0f, 0.0f, 0.0f};
    glm::vec3 oc = sphereCenter - rayOrigin;

    float radius = 0.5f;

    float a = glm::dot(rayDirection, rayDirection);
    float b = -2.0f * glm::dot(oc, rayDirection);
    float c = glm::dot(oc, oc) - (radius * radius);

    float discriminant = b * b - 4.0f * a * c;
    if (discriminant < 0.0f)
    {
        return {0, 0, 0, 1};
    }

    const float sqrtDiscriminant = glm::sqrt(discriminant);
    const float t0 = (-b + sqrtDiscriminant) / (2.0f * a);
    const float closestT = (-b - sqrtDiscriminant) / (2.0f * a);

    glm::vec3 h0 = rayOrigin + (t0 * rayDirection);
    glm::vec3 closestHit = rayOrigin + (closestT * rayDirection);

    glm::vec3 sphereColor(1, 0, 1);
    sphereColor = closestHit;
    return {sphereColor, 1.0f};
}
