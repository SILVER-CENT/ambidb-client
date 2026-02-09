#include "gui_backend.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

namespace ambidb {

GuiBackend::GuiBackend() : m_app(std::make_unique<App>()) {}
GuiBackend::~GuiBackend() {}

bool GuiBackend::Initialize() {
    if (!glfwInit()) return false;

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    m_window = glfwCreateWindow(1280, 720, "AmbiDB Client (GUI)", nullptr, nullptr);
    if (!m_window) {
        glfwTerminate();
        m_window = nullptr;
        return false;
    }

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1); // Enable vsync

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    bool glfw_imgui_ok = ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    bool gl3_imgui_ok = ImGui_ImplOpenGL3_Init(glsl_version);
    if (!glfw_imgui_ok || !gl3_imgui_ok) {
        if (gl3_imgui_ok) {
            ImGui_ImplOpenGL3_Shutdown();
        }
        if (glfw_imgui_ok) {
            ImGui_ImplGlfw_Shutdown();
        }
        ImGui::DestroyContext();
        glfwDestroyWindow(m_window);
        glfwTerminate();
        return false;
    }

    return true;
}

void GuiBackend::Run() {
    while (!glfwWindowShouldClose(m_window) && !m_app->ShouldClose()) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        m_app->Update();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(m_window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_window);
    }
}

void GuiBackend::Shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_window);
    glfwTerminate();
}

} // namespace ambidb
