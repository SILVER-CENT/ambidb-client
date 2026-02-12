#pragma once

#include "imgui.h"

namespace ambidb::ui {

#if defined(IMGUI_HAS_TABLE)
	using TableFlags = ImGuiTableFlags;
	using TableColumnFlags = ImGuiTableColumnFlags;
	inline constexpr TableFlags kDefaultTableFlags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
	inline constexpr TableColumnFlags kNoTableColumnFlags = ImGuiTableColumnFlags_None;
#else
	using TableFlags = int;
	using TableColumnFlags = int;
	inline constexpr TableFlags kDefaultTableFlags = 0;
	inline constexpr TableColumnFlags kNoTableColumnFlags = 0;
#endif

	struct TableConfig {
		TableFlags flags{kDefaultTableFlags};
		ImVec2 outerSize{0.0f, 0.0f};
		float innerWidth{0.0f};
	};

	bool
	BeginDataTable (const char* id, int columns, const TableConfig& config = {});

	void
	EndDataTable ();

	void
	SetupColumn (const char* label,
				 TableColumnFlags flags = kNoTableColumnFlags,
				 float initWidthOrWeight = 0.0f,
				 ImU32 userId = 0);

	void
	HeadersRow ();

	void
	NextRow ();

	void
	NextColumn ();

	void
	CellText (const char* text);

}  // namespace ambidb::ui
