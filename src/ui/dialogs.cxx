#include "dialogs.h"

namespace ambidb::ui {

	void
	OpenPopup (const char* popupId) {
		ImGui::OpenPopup (popupId);
	}

	bool
	BeginPopup (const char* popupId, ImGuiWindowFlags flags) {
		return ImGui::BeginPopup (popupId, flags);
	}

	bool
	BeginContextPopup (const char* popupId) {
		if (popupId && popupId [0] != '\0') {
			return ImGui::BeginPopupContextItem (popupId);
		}
		return ImGui::BeginPopupContextItem ();
	}

	void
	EndPopup () {
		ImGui::EndPopup ();
	}

	bool
	BeginCenteredModal (const char* title, bool* open, ImGuiWindowFlags flags) {
		const ImVec2 center = ImGui::GetMainViewport ()->GetCenter ();
		ImGui::SetNextWindowPos (center, ImGuiCond_Appearing, ImVec2 (0.5f, 0.5f));
		return ImGui::BeginPopupModal (title, open, flags);
	}

	void
	EndModal () {
		ImGui::EndPopup ();
	}

	ModalResult
	ModalButtonRow (const char* confirmLabel, const char* cancelLabel, float buttonWidth, bool closeOnClick) {
		if (ImGui::Button (confirmLabel, ImVec2 (buttonWidth, 0.0f))) {
			if (closeOnClick) {
				ImGui::CloseCurrentPopup ();
			}
			return ModalResult::Confirmed;
		}

		ImGui::SetItemDefaultFocus ();
		ImGui::SameLine ();

		if (ImGui::Button (cancelLabel, ImVec2 (buttonWidth, 0.0f))) {
			if (closeOnClick) {
				ImGui::CloseCurrentPopup ();
			}
			return ModalResult::Cancelled;
		}

		return ModalResult::None;
	}

}  // namespace ambidb::ui
