#include "app.h"

namespace ambidb {

App::App() {}
App::~App() {}

void App::Update() {
    ImGui::Begin("AmbiDB Client");

    ImGui::Text("Welcome to AmbiDB!");
    ImGui::Text("One Codebase. Two Interfaces.");

    ImGui::Separator();

    if (ImGui::Button("Click me!")) {
        m_counter++;
    }
    ImGui::SameLine();
    ImGui::Text("Counter: %d", m_counter);

    if (ImGui::Button("Quit")) {
        m_shouldClose = true;
    }

    ImGui::End();
}

} // namespace ambidb
