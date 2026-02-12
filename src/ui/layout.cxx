#include "layout.h"

#include "metrics.h"
#include "theme.h"

#include "imgui.h"

namespace ambidb::ui {

	namespace {

		constexpr ImGuiWindowFlags MAIN_WINDOW_FLAGS = ImGuiWindowFlags_NoDecoration |
													   ImGuiWindowFlags_NoMove |
													   ImGuiWindowFlags_NoResize |
													   ImGuiWindowFlags_NoBringToFrontOnFocus |
													   ImGuiWindowFlags_NoSavedSettings;

	}  // namespace

	void
	BeginAppShell () {
		const ImGuiViewport* viewport = ImGui::GetMainViewport ();
		ImGui::SetNextWindowPos (viewport->WorkPos);
		ImGui::SetNextWindowSize (viewport->WorkSize);

		ImGui::PushStyleVar (ImGuiStyleVar_WindowPadding, ImVec2 (0.0f, 0.0f));
		ImGui::Begin ("##MainWindow", nullptr, MAIN_WINDOW_FLAGS);
		ImGui::PopStyleVar ();
	}

	void
	EndAppShell () {
		ImGui::End ();
	}

	void
	BeginSidebar (float width) {
		const float sidebarWidth = (width > 0.0f) ? width : kMetrics.sidebarWidth;
		ImGui::PushStyleColor (ImGuiCol_ChildBg, ActiveTheme ().sidebarBg);
		ImGui::BeginChild ("##Sidebar", ImVec2 (sidebarWidth, 0.0f), false, ImGuiWindowFlags_None);
	}

	void
	EndSidebar () {
		ImGui::EndChild ();
		ImGui::PopStyleColor ();
	}

	void
	BeginContent () {
		ImGui::BeginChild ("##Content", ImVec2 (0.0f, 0.0f), false, ImGuiWindowFlags_None);
	}

	void
	EndContent () {
		ImGui::EndChild ();
	}

	void
	VerticalDivider () {
		if (!kCaps.drawVerticalDivider) return;

		const ImGuiViewport* viewport = ImGui::GetMainViewport ();
		const ImVec2 dividerPos = ImGui::GetCursorScreenPos ();
		ImGui::GetWindowDrawList ()->AddLine (
			ImVec2 (dividerPos.x, viewport->WorkPos.y),
			ImVec2 (dividerPos.x, viewport->WorkPos.y + viewport->WorkSize.y),
			ImGui::ColorConvertFloat4ToU32 (ActiveTheme ().separator),
			1.0f);
	}

	void
	PinToBottom (float reserveHeight) {
		const float remaining = ImGui::GetContentRegionAvail ().y;
		if (remaining > reserveHeight) {
			ImGui::SetCursorPosY (ImGui::GetCursorPosY () + remaining - reserveHeight);
		}
	}

}  // namespace ambidb::ui
