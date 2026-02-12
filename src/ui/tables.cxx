#include "tables.h"

#include <vector>

namespace ambidb::ui {

	namespace {

#if !defined(IMGUI_HAS_TABLE)
		std::vector<int> g_columnStack;
#endif

	}  // namespace

	bool
	BeginDataTable (const char* id, int columns, const TableConfig& config) {
#if defined(IMGUI_HAS_TABLE)
		return ImGui::BeginTable (id,
								  columns,
								  static_cast<ImGuiTableFlags> (config.flags),
								  config.outerSize,
								  config.innerWidth);
#else
		if (columns <= 0) return false;
		ImGui::Columns (columns, id, true);
		g_columnStack.push_back (columns);
		return true;
#endif
	}

	void
	EndDataTable () {
#if defined(IMGUI_HAS_TABLE)
		ImGui::EndTable ();
#else
		if (g_columnStack.empty ()) return;
		ImGui::Columns (1);
		g_columnStack.pop_back ();
#endif
	}

	void
	SetupColumn (const char* label, TableColumnFlags flags, float initWidthOrWeight, ImU32 userId) {
#if defined(IMGUI_HAS_TABLE)
		ImGui::TableSetupColumn (label, static_cast<ImGuiTableColumnFlags> (flags), initWidthOrWeight, userId);
#else
		(void) label;
		(void) flags;
		(void) initWidthOrWeight;
		(void) userId;
#endif
	}

	void
	HeadersRow () {
#if defined(IMGUI_HAS_TABLE)
		ImGui::TableHeadersRow ();
#endif
	}

	void
	NextRow () {
#if defined(IMGUI_HAS_TABLE)
		ImGui::TableNextRow ();
#endif
	}

	void
	NextColumn () {
#if defined(IMGUI_HAS_TABLE)
		ImGui::TableNextColumn ();
#else
		ImGui::NextColumn ();
#endif
	}

	void
	CellText (const char* text) {
		ImGui::TextUnformatted (text);
	}

}  // namespace ambidb::ui
