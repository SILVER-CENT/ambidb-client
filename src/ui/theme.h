#pragma once

#include <string>

#include "imgui.h"

namespace ambidb::ui {

	struct ThemeConfig {
		ImVec4 windowBg;
		ImVec4 childBg;

		ImVec4 button;
		ImVec4 buttonHovered;
		ImVec4 buttonActive;

		ImVec4 header;
		ImVec4 headerHovered;
		ImVec4 headerActive;

		ImVec4 frameBg;
		ImVec4 frameBgHovered;
		ImVec4 frameBgActive;

		ImVec4 separator;

		ImVec4 scrollbarBg;
		ImVec4 scrollbarGrab;
		ImVec4 scrollbarGrabHovered;
		ImVec4 scrollbarGrabActive;

		ImVec4 text;
		ImVec4 textDisabled;

		ImVec4 sidebarBg;

		ImVec4 navActiveHeader;
		ImVec4 navActiveHovered;
		ImVec4 navActivePressed;

		ImVec4 navInactiveHeader;
		ImVec4 navInactiveHovered;
		ImVec4 navInactivePressed;
	};

	enum class ThemePreset {
		Default,
		GruvboxDark,
		Tokyonight,
		Dracula,
	};


	/// Original dark theme (the same palette we shipped earlier).
	ThemeConfig
	DarkTheme ();

	/// Gruvbox Dark — warm retro palette.
	/// Uses ANSI-256-safe RGB values so imtui terminals render the intended colors.
	ThemeConfig
	GruvboxDarkTheme ();

	/// Tokyonight Storm — cool blue-purple palette.
	ThemeConfig
	TokyonightTheme ();

	/// Dracula — vivid purple-pink palette.
	ThemeConfig
	DraculaTheme ();

	/// Return the ThemeConfig for a given preset enum.
	ThemeConfig
	PresetTheme (ThemePreset preset);

	/// Run every color in a ThemeConfig through SnapToAnsi256().
	/// Call this before ApplyTheme() when targeting TUI to guarantee each
	/// ImGui color lands on the exact ANSI-256 index you intend.
	ThemeConfig
	SnapThemeForTUI (const ThemeConfig& config);

	void
	ApplyTheme (const ThemeConfig& config = DarkTheme ());

	const ThemeConfig&
	ActiveTheme ();

	ImVec4
	StatusColor (bool connected);

	ImVec4
	DbTypeColor (const std::string& type);

}  // namespace ambidb::ui
