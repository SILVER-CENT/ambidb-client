#include "hints.h"

#include "imgui.h"

namespace ambidb::ui {

	namespace {

		void
		BulletLine (const char* text) {
			ImGui::Bullet ();
			ImGui::SameLine ();
			ImGui::TextUnformatted (text);
		}

	}  // namespace

	void
	HelpMarker (const char* description, float wrapAtChars) {
		ImGui::PushStyleColor (ImGuiCol_Text, ImGui::GetStyleColorVec4 (ImGuiCol_TextDisabled));
		ImGui::TextUnformatted ("(?)");
		ImGui::PopStyleColor ();
		if (!ImGui::IsItemHovered ()) return;

		ImGui::BeginTooltip ();
		ImGui::PushTextWrapPos (ImGui::GetFontSize () * wrapAtChars);
		ImGui::TextUnformatted (description);
		ImGui::PopTextWrapPos ();
		ImGui::EndTooltip ();
	}

	void
	ShowUserGuide () {
		ImGuiIO& io = ImGui::GetIO ();

		BulletLine ("Double-click title bar to collapse a window.");
		BulletLine ("CTRL+click sliders/drag fields to type exact values.");
		BulletLine ("TAB/SHIFT+TAB cycles keyboard-editable fields.");
		if (io.FontAllowUserScaling) {
			BulletLine ("CTRL+mouse wheel zooms window contents.");
		}

		BulletLine ("While entering text:");
		ImGui::Indent ();
		BulletLine ("CTRL+Left/Right jumps by word.");
		BulletLine ("CTRL+A or double-click selects all.");
		BulletLine ("CTRL+X/C/V does cut/copy/paste.");
		BulletLine ("CTRL+Z/CTRL+Y does undo/redo.");
		BulletLine ("ESC reverts.");
		ImGui::Unindent ();

		BulletLine ("With keyboard navigation enabled:");
		ImGui::Indent ();
		BulletLine ("Arrow keys navigate.");
		BulletLine ("Space activates widgets.");
		BulletLine ("Return starts text entry on focused widgets.");
		BulletLine ("Escape deactivates widgets and closes popups.");
		BulletLine ("CTRL+Tab switches windows.");
		ImGui::Unindent ();
	}

}  // namespace ambidb::ui
