#pragma once

#include "imgui.h"

namespace ambidb::ui {

	bool
	ComboCString (const char* label, int* currentItem, const char* const* items, int itemCount);

	bool
	BeginCombo (const char* label, const char* previewValue, ImGuiComboFlags flags = 0);

	void
	EndCombo ();

	bool
	Selectable (const char* label,
				bool selected = false,
				ImGuiSelectableFlags flags = 0,
				const ImVec2& size = ImVec2 (0.0f, 0.0f));

	bool
	SelectableToggle (const char* label,
					  bool* selected,
					  ImGuiSelectableFlags flags = 0,
					  const ImVec2& size = ImVec2 (0.0f, 0.0f));

	bool
	SelectableList (const char* id,
					const char* const* items,
					int itemCount,
					int* selectedIndex,
					float heightInItems = -1.0f);

}  // namespace ambidb::ui
