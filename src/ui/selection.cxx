#include "selection.h"

namespace ambidb::ui {

	bool
	ComboCString (const char* label, int* currentItem, const char* const* items, int itemCount) {
		return ImGui::Combo (label, currentItem, items, itemCount);
	}

	bool
	BeginCombo (const char* label, const char* previewValue, ImGuiComboFlags flags) {
		return ImGui::BeginCombo (label, previewValue, flags);
	}

	void
	EndCombo () {
		ImGui::EndCombo ();
	}

	bool
	Selectable (const char* label, bool selected, ImGuiSelectableFlags flags, const ImVec2& size) {
		return ImGui::Selectable (label, selected, flags, size);
	}

	bool
	SelectableToggle (const char* label, bool* selected, ImGuiSelectableFlags flags, const ImVec2& size) {
		return ImGui::Selectable (label, selected, flags, size);
	}

	bool
	SelectableList (const char* id, const char* const* items, int itemCount, int* selectedIndex, float heightInItems) {
		if (!selectedIndex || itemCount <= 0) return false;

		bool changed = false;
		if (ImGui::BeginListBox (id, ImVec2 (0.0f, heightInItems))) {
			for (int i = 0; i < itemCount; ++i) {
				const bool isSelected = (*selectedIndex == i);
				if (ImGui::Selectable (items [i], isSelected)) {
					*selectedIndex = i;
					changed = true;
				}
				if (isSelected) {
					ImGui::SetItemDefaultFocus ();
				}
			}
			ImGui::EndListBox ();
		}

		return changed;
	}

}  // namespace ambidb::ui
