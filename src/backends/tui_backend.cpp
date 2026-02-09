#include "tui_backend.h"
#include "imtui/imtui.h"
#include "imtui/imtui-impl-ncurses.h"
#include <ncurses.h>
#include <thread>
#include <chrono>

namespace ambidb {

TuiBackend::TuiBackend() : m_app(std::make_unique<App>()) {}
TuiBackend::~TuiBackend() {}

bool TuiBackend::Initialize() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // ImTui doesn't use standard ImGui backends
    m_screen = ImTui_ImplNcurses_Init(true);
    ImTui_ImplText_Init();

    return m_screen != nullptr;
}

void TuiBackend::Run() {
    while (!m_app->ShouldClose()) {
        ImTui_ImplNcurses_NewFrame();
        ImTui_ImplText_NewFrame();
        ImGui::NewFrame();

        m_app->Update();

        ImGui::Render();
        ImTui_ImplText_RenderDrawData(ImGui::GetDrawData(), (ImTui::TScreen*)m_screen);
        ImTui_ImplNcurses_DrawScreen(true);

        // Prevent busy-spinning and high CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void TuiBackend::Shutdown() {
    ImTui_ImplText_Shutdown();
    ImTui_ImplNcurses_Shutdown();
    ImGui::DestroyContext();
}

} // namespace ambidb
