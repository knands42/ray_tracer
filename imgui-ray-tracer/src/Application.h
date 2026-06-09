#ifndef APPLICATION_H
#define APPLICATION_H

#include <GLFW/glfw3.h>
#include "imgui.h"
#include "Camera.h"

class Renderer;

class Application {
public:
    Application();
    ~Application();
    void Run();
    void SetCamera(const Camera& camera);

private:
    static void GlfwErrorCallback(int error, const char* description);
    void SetupGlfw();
    void SetupImGui();
    void CreateTexture();
    void UpdateTexture();
    void RenderFrame();
    void DrawOnUI();
    void Render();
    void Shutdown();

    GLFWwindow* m_Window = nullptr;
    Renderer* m_Renderer = nullptr;
    GLuint m_TextureID = 0;
    ImVec4 m_ClearColor{0.0f, 0.0f, 0.0f, 1.0f};
    const char* m_GlslVersion = "#version 130";
    float m_ViewPortWidth = 800.0f;
    float m_ViewPortHeight = 600.0f;
    Camera m_Camera;
};

#endif
