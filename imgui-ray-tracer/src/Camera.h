#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>

class Camera
{
public:
    Camera() = default;
    Camera(float verticalFOV, float nearClip, float farClip);

    void OnUpdate(float ts);
    void OnResize(uint32_t width, uint32_t height);

    [[nodiscard]] auto GetProjection() const -> const glm::mat4& { return m_Projection; }
    [[nodiscard]] auto GetInverseProjection() const -> const glm::mat4& { return m_InverseProjection; }
    [[nodiscard]] auto GetView() const -> const glm::mat4& { return m_View; }
    [[nodiscard]] auto GetInverseView() const -> const glm::mat4& { return m_InverseView; }

    [[nodiscard]] auto GetPosition() const -> const glm::vec3& { return m_Position; }
    [[nodiscard]] auto GetDirection() const -> const glm::vec3& { return m_ForwardDirection; }

    [[nodiscard]] const std::vector<glm::vec3>& GetRayDirections() const { return m_RayDirections; }

    static auto GetRotationSpeed() -> float;
private:
    void RecalculateProjection();
    void RecalculateView();
    void RecalculateRayDirections();

    glm::mat4 m_Projection{ 1.0f };
    glm::mat4 m_View{ 1.0f };
    glm::mat4 m_InverseProjection{ 1.0f };
    glm::mat4 m_InverseView{ 1.0f };

    float m_VerticalFOV = 45.0f;
    float m_NearClip = 0.1f;
    float m_FarClip = 100.0f;

    glm::vec3 m_Position{0.0f, 0.0f, 0.0f};
    glm::vec3 m_ForwardDirection{0.0f, 0.0f, 0.0f};

    // Cached ray directions
    std::vector<glm::vec3> m_RayDirections;

    glm::vec2 m_LastMousePosition{0.0f, 0.0f};

    uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
};
