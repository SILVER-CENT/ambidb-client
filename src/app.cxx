#include "app.h"

namespace ambidb {

	void
	App::Update () {
		if (!m_hasCenteredWindow) {
			const ImGuiViewport* viewport = ImGui::GetMainViewport ();
			ImGui::SetNextWindowPos (viewport->GetCenter (), ImGuiCond_Always, ImVec2 (0.5f, 0.5f));
			m_hasCenteredWindow = true;
		}

		ImGui::Begin ("AmbiDB Client", nullptr, ImGuiWindowFlags_NoSavedSettings);

		ImGui::Text ("Welcome to AmbiDB!");
		ImGui::Text ("One Codebase. Two Interfaces.");

		ImGui::Separator ();

		if (ImGui::Button ("Click me!")) {
			m_counter++;
		}
		ImGui::SameLine ();
		ImGui::Text ("Counter: %d", m_counter);

		if (ImGui::Button ("Quit")) {
			m_shouldClose = true;
		}

		ImGui::End ();
	}

}  // namespace ambidb
