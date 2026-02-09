#include "gui_backend.h"
#include "backend_config.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <iostream>

namespace ambidb {

bool GuiBackend::InitializeBackend() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, config::OPENGL_MAJOR_VERSION);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, config::OPENGL_MINOR_VERSION);

    std::string title = std::string(config::WINDOW_TITLE) + " (GUI)";
    m_window = glfwCreateWindow(
        config::DEFAULT_WINDOW_WIDTH,
        config::DEFAULT_WINDOW_HEIGHT,
        title.c_str(),
        nullptr,
        nullptr
    );

    if (!m_window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1); // Enable vsync

    return true;
}

bool GuiBackend::InitializeImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    if (!ImGui_ImplGlfw_InitForOpenGL(m_window, true)) {
        std::cerr << "Failed to initialize ImGui GLFW implementation" << std::endl;
        return false;
    }

    if (!ImGui_ImplOpenGL3_Init(config::GLSL_VERSION)) {
        std::cerr << "Failed to initialize ImGui OpenGL3 implementation" << std::endl;
        ImGui_ImplGlfw_Shutdown();
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
        glClearColor(
            config::CLEAR_COLOR_R,
            config::CLEAR_COLOR_G,
            config::CLEAR_COLOR_B,
            config::CLEAR_COLOR_A
        );
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_window);
    }
}

void GuiBackend::ShutdownImGui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GuiBackend::ShutdownBackend() {
    if (m_window) {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
    glfwTerminate();
}

} // namespace ambidb
