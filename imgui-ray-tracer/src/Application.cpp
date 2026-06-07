#include "Application.h"
#include "Renderer.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <cstdio>

Application::Application()
{
    SetupGlfw();
    SetupImGui();

    m_Renderer = new Renderer(m_ViewPortWidth, m_ViewPortHeight);

    CreateTexture();
    UpdateTexture();
}

Application::~Application()
{
    Shutdown();
}

void Application::GlfwErrorCallback(int error, const char *description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

void Application::SetupGlfw()
{
    glfwSetErrorCallback(GlfwErrorCallback);
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    m_Window = glfwCreateWindow(1280, 720, "Ray Tracer", nullptr, nullptr);
    if (!m_Window)
    {
        fprintf(stderr, "Failed to create GLFW window\n");
        exit(1);
    }

    glfwMakeContextCurrent(m_Window);
    glfwSwapInterval(1);
}

void Application::SetupImGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
    ImGui_ImplOpenGL3_Init(m_GlslVersion);
}

void Application::CreateTexture()
{
    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Application::UpdateTexture()
{
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 m_ViewPortWidth, m_ViewPortHeight, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, m_Renderer->GetData());
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Application::RenderFrame()
{
    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::DockSpaceOverViewport();

    // Code Here
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    DrawOnUI();
    ImGui::PopStyleVar();

    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(m_Window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(m_ClearColor.x * m_ClearColor.w,
                 m_ClearColor.y * m_ClearColor.w,
                 m_ClearColor.z * m_ClearColor.w,
                 m_ClearColor.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(m_Window);
}

void Application::DrawOnUI()
{
    ImGui::Begin("Settings");
    ImGui::Text("Last render: %.3fms", m_Renderer->GetLastRenderTime());
    if (ImGui::Button("Render"))
    {
        m_Renderer->Render();
    }
    ImGui::End();

    ImGui::Begin("Viewport");
    Render();

    auto finalImage = m_Renderer->GetFinalImage();
    ImGui::Image(m_TextureID, ImVec2(finalImage->width, finalImage->height),
                 ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();
}

void Application::Render()
{
    m_ViewPortWidth = ImGui::GetContentRegionAvail().x;
    m_ViewPortHeight = ImGui::GetContentRegionAvail().y;
    m_Renderer->OnResize(m_ViewPortWidth, m_ViewPortHeight);
    m_Renderer->Render();
    UpdateTexture();
}

void Application::Run()
{
    while (!glfwWindowShouldClose(m_Window))
        RenderFrame();
}

void Application::Shutdown()
{
    glDeleteTextures(1, &m_TextureID);

    delete m_Renderer;
    m_Renderer = nullptr;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_Window);
    glfwTerminate();
}
