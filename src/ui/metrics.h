#pragma once

#include "imgui.h"

namespace ambidb::ui {

	struct UiMetrics {
		float sidebarWidth;
		ImVec2 styleFramePadding;
		ImVec2 styleItemSpacing;
		ImVec2 styleWindowPadding;
		ImVec2 sidebarPadding;
		ImVec2 contentPadding;
		float frameRounding;
		float scrollbarSize;
		float primaryButtonPadY;
		float titleOffsetX;
		float contentOffsetX;
		float topGapY;
		float rowGapY;
		float sectionGapY;
		float statusReserveExtraY;
		float quitReserveY;
	};

	struct UiCaps {
		bool drawVerticalDivider;
	};

#if defined(AMBIDB_TUI)
	inline const UiMetrics kMetrics{
		30.0f,
		ImVec2 (1.0f, 0.0f),
		ImVec2 (1.0f, 0.0f),
		ImVec2 (1.0f, 0.0f),
		ImVec2 (1.0f, 0.0f),
		ImVec2 (1.0f, 0.0f),
		0.0f,
		1.0f,
		0.0f,
		0.0f,
		0.0f,
		0.0f,
		1.0f,
		1.0f,
		2.0f,
		2.0f,
	};

	inline const UiCaps kCaps{false};
#else
	inline const UiMetrics kMetrics{
		190.0f,
		ImVec2 (8.0f, 4.0f),
		ImVec2 (8.0f, 6.0f),
		ImVec2 (8.0f, 8.0f),
		ImVec2 (8.0f, 8.0f),
		ImVec2 (24.0f, 20.0f),
		4.0f,
		10.0f,
		6.0f,
		10.0f,
		12.0f,
		2.0f,
		4.0f,
		8.0f,
		12.0f,
		40.0f,
	};

	inline const UiCaps kCaps{true};
#endif

}  // namespace ambidb::ui
