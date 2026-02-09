#include "tui_backend.h"
#include "imtui/imtui.h"
#include "imtui/imtui-impl-ncurses.h"
#include <ncurses.h>
#include <poll.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <iostream>

namespace ambidb {

bool TuiBackend::InitializeBackend() {
    // For TUI, backend initialization is minimal
    // Most work is done in InitializeImGui
    return true;
}

bool TuiBackend::InitializeImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    m_screen = ImTui_ImplNcurses_Init(true);
    if (!m_screen) {
        std::cerr << "Failed to initialize ImTui ncurses implementation" << std::endl;
        ImGui::DestroyContext();
        m_screen = nullptr;
        return false;
    }

    ImTui_ImplText_Init();
    return true;
}

void TuiBackend::Run() {
    bool firstFrame = true;
    while (!m_app->ShouldClose()) {
        // Skip polling on the first frame to avoid delay
        if (!firstFrame) {
            struct pollfd fds[1];
            fds[0].fd = STDIN_FILENO;
            fds[0].events = POLLIN;

            // Wait for input with blocking poll
            if (poll(fds, 1, -1) == -1) {
                if (errno != EINTR) {
                    std::cerr << "Poll error while waiting for stdin: " << std::strerror(errno)
                              << " (errno=" << errno << ")" << std::endl;
                    break;
                }
            }
        }
        firstFrame = false;

        ImTui_ImplNcurses_NewFrame();
        ImTui_ImplText_NewFrame();
        ImGui::NewFrame();

        m_app->Update();

        ImGui::Render();
        ImTui_ImplText_RenderDrawData(ImGui::GetDrawData(), (ImTui::TScreen*)m_screen);
        ImTui_ImplNcurses_DrawScreen(true);
    }
}

void TuiBackend::ShutdownImGui() {
    ImTui_ImplText_Shutdown();
    ImTui_ImplNcurses_Shutdown();
    ImGui::DestroyContext();
}

void TuiBackend::ShutdownBackend() {
    // No additional backend-specific cleanup needed
    m_screen = nullptr;
}

} // namespace ambidb
