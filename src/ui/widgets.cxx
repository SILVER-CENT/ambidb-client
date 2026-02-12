#include "widgets.h"

#include "metrics.h"
#include "theme.h"

#include "imgui.h"

namespace ambidb::ui {

	namespace {

		void
		ApplyOffsetX (float offsetX) {
			if (offsetX > 0.0f) {
				ImGui::SetCursorPosX (ImGui::GetCursorPosX () + offsetX);
			}
		}

	}  // namespace

	void
	TextMuted (const char* text) {
		ImGui::PushStyleColor (ImGuiCol_Text, ImGui::GetStyleColorVec4 (ImGuiCol_TextDisabled));
		ImGui::TextUnformatted (text);
		ImGui::PopStyleColor ();
	}

	void
	StatusDot (bool connected) {
		ImGui::PushStyleColor (ImGuiCol_Text, StatusColor (connected));
		ImGui::TextUnformatted ("*");
		ImGui::PopStyleColor ();
	}

	void
	TypeBadge (const std::string& type) {
		std::string badge = "[";
		badge += type;
		badge += "]";

		ImGui::PushStyleColor (ImGuiCol_Text, DbTypeColor (type));
		ImGui::TextUnformatted (badge.c_str ());
		ImGui::PopStyleColor ();
	}

	bool
	NavItem (const char* icon, const char* label, bool isActive) {
		const ThemeConfig& theme = ActiveTheme ();

		std::string itemLabel = "  ";
		itemLabel += icon;
		itemLabel += "  ";
		itemLabel += label;

		ImGui::PushID (label);
		if (isActive) {
			ImGui::PushStyleColor (ImGuiCol_Header, theme.navActiveHeader);
			ImGui::PushStyleColor (ImGuiCol_HeaderHovered, theme.navActiveHovered);
			ImGui::PushStyleColor (ImGuiCol_HeaderActive, theme.navActivePressed);
		}
		else {
			ImGui::PushStyleColor (ImGuiCol_Header, theme.navInactiveHeader);
			ImGui::PushStyleColor (ImGuiCol_HeaderHovered, theme.navInactiveHovered);
			ImGui::PushStyleColor (ImGuiCol_HeaderActive, theme.navInactivePressed);
		}

		const float width = ImGui::GetContentRegionAvail ().x;
		const bool clicked = ImGui::Selectable (itemLabel.c_str (),
												isActive,
												ImGuiSelectableFlags_None,
												ImVec2 (width, 0.0f));

		ImGui::PopStyleColor (3);
		ImGui::PopID ();
		return clicked;
	}

	void
	Gap (float height) {
		if (height <= 0.0f) return;
		ImGui::Dummy (ImVec2 (0.0f, height));
	}

	void
	AppTitle (const char* text) {
		ApplyOffsetX (kMetrics.titleOffsetX);
		ImGui::TextUnformatted (text);
	}

	void
	ContentHeading (const char* text) {
		ApplyOffsetX (kMetrics.contentOffsetX);
		ImGui::TextUnformatted (text);
	}

	void
	AlignContentStart () {
		ApplyOffsetX (kMetrics.contentOffsetX);
	}

}  // namespace ambidb::ui
