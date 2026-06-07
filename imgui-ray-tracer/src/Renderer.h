#ifndef RAY_TRACER_SIMPLE_RENDERING_H
#define RAY_TRACER_SIMPLE_RENDERING_H

#include <memory>
#include <vector>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Renderer {
public:
    Renderer(uint32_t viewPortWidth, uint32_t viewPortHeight);

    [[nodiscard]] std::shared_ptr<GLFWimage> GetFinalImage() const { return m_FinalImage; }

    [[nodiscard]] uint32_t* GetData() { return m_ImageData.data(); }
    [[nodiscard]] uint32_t GetSize() const { return m_ImageData.size(); }

    [[nodiscard]] uint32_t GetWidth() const { return m_FinalImage->width; }
    [[nodiscard]] uint32_t GetHeight() const { return m_FinalImage->height; }

    [[nodiscard]] float GetLastRenderTime() const { return m_LastRenderTime; }

    void OnResize(uint32_t width, uint32_t height);
    void Render();

private:
    glm::vec4 PerPixel(glm::vec2 coord);

private:
    std::shared_ptr<GLFWimage> m_FinalImage;
    float m_LastRenderTime = 0.0f;
    std::vector<uint32_t> m_ImageData;
};

#endif
