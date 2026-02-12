#pragma once

#include "imgui.h"

namespace ambidb::ui {

	enum class ModalResult {
		None,
		Confirmed,
		Cancelled,
	};

	void
	OpenPopup (const char* popupId);

	bool
	BeginPopup (const char* popupId, ImGuiWindowFlags flags = 0);

	bool
	BeginContextPopup (const char* popupId = nullptr);

	void
	EndPopup ();

	bool
	BeginCenteredModal (const char* title,
						bool* open = nullptr,
						ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize);

	void
	EndModal ();

	ModalResult
	ModalButtonRow (const char* confirmLabel = "OK",
					const char* cancelLabel = "Cancel",
					float buttonWidth = 120.0f,
					bool closeOnClick = true);

}  // namespace ambidb::ui
