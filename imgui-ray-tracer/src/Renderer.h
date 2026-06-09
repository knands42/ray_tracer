#pragma once

#include <memory>
#include <vector>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Camera.h"

class Renderer {
public:
    Renderer(uint32_t viewPortWidth, uint32_t viewPortHeight);

    [[nodiscard]] auto GetFinalImage() const -> std::shared_ptr<GLFWimage> { return m_FinalImage; }

    [[nodiscard]] auto GetData() -> uint32_t* { return m_ImageData.data(); }
    [[nodiscard]] auto GetSize() const -> uint32_t { return m_ImageData.size(); }

    [[nodiscard]] auto GetWidth() const -> uint32_t { return m_FinalImage->width; }
    [[nodiscard]] auto GetHeight() const -> uint32_t { return m_FinalImage->height; }

    [[nodiscard]] auto GetLastRenderTime() const -> float { return m_LastRenderTime; }

    void OnResize(uint32_t width, uint32_t height);
    void Render(const Camera& camera);
    void SetLightDir(float x, float y, float z) { m_LightDir = glm::normalize(glm::vec3(x, y, z)); }

private:
    std::shared_ptr<GLFWimage> m_FinalImage;
    float m_LastRenderTime = 0.0f;
    std::vector<uint32_t> m_ImageData;
    glm::vec3 m_LightDir = glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f));

    auto PerPixel(glm::vec2 coord) const -> glm::vec4;
};
