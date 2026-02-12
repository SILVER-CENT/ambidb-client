#include "theme.h"

#include "color_utils.h"
#include "metrics.h"

namespace ambidb::ui {

	namespace {

		bool
		SameColor (const ImVec4& lhs, const ImVec4& rhs) {
			return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
		}

		bool
		SameTheme (const ThemeConfig& lhs, const ThemeConfig& rhs) {
			return SameColor (lhs.windowBg, rhs.windowBg) &&
				   SameColor (lhs.childBg, rhs.childBg) && SameColor (lhs.button, rhs.button) &&
				   SameColor (lhs.buttonHovered, rhs.buttonHovered) &&
				   SameColor (lhs.buttonActive, rhs.buttonActive) &&
				   SameColor (lhs.header, rhs.header) &&
				   SameColor (lhs.headerHovered, rhs.headerHovered) &&
				   SameColor (lhs.headerActive, rhs.headerActive) &&
				   SameColor (lhs.frameBg, rhs.frameBg) &&
				   SameColor (lhs.frameBgHovered, rhs.frameBgHovered) &&
				   SameColor (lhs.frameBgActive, rhs.frameBgActive) &&
				   SameColor (lhs.separator, rhs.separator) &&
				   SameColor (lhs.scrollbarBg, rhs.scrollbarBg) &&
				   SameColor (lhs.scrollbarGrab, rhs.scrollbarGrab) &&
				   SameColor (lhs.scrollbarGrabHovered, rhs.scrollbarGrabHovered) &&
				   SameColor (lhs.scrollbarGrabActive, rhs.scrollbarGrabActive) &&
				   SameColor (lhs.text, rhs.text) &&
				   SameColor (lhs.textDisabled, rhs.textDisabled) &&
				   SameColor (lhs.sidebarBg, rhs.sidebarBg) &&
				   SameColor (lhs.navActiveHeader, rhs.navActiveHeader) &&
				   SameColor (lhs.navActiveHovered, rhs.navActiveHovered) &&
				   SameColor (lhs.navActivePressed, rhs.navActivePressed) &&
				   SameColor (lhs.navInactiveHeader, rhs.navInactiveHeader) &&
				   SameColor (lhs.navInactiveHovered, rhs.navInactiveHovered) &&
				   SameColor (lhs.navInactivePressed, rhs.navInactivePressed);
		}

		ImGuiContext*&
		StyledContext () {
			static ImGuiContext* context = nullptr;
			return context;
		}

		ThemeConfig&
		ActiveThemeStorage () {
			static ThemeConfig theme = DarkTheme ();
			return theme;
		}

		ImVec4
		SnapOrKeep (const ImVec4& color) {
			if (color.w == 0.0f) return color;
			return SnapToAnsi256 (color);
		}

	}  // namespace

	ThemeConfig
	DarkTheme () {
		return {
			ImVec4 (0.10f, 0.10f, 0.12f, 1.0f), ImVec4 (0.0f, 0.0f, 0.0f, 0.0f),

			ImVec4 (0.20f, 0.20f, 0.24f, 1.0f), ImVec4 (0.28f, 0.28f, 0.34f, 1.0f),
			ImVec4 (0.34f, 0.34f, 0.40f, 1.0f),

			ImVec4 (0.22f, 0.35f, 0.55f, 1.0f), ImVec4 (0.26f, 0.40f, 0.60f, 1.0f),
			ImVec4 (0.30f, 0.45f, 0.65f, 1.0f),

			ImVec4 (0.16f, 0.16f, 0.20f, 1.0f), ImVec4 (0.22f, 0.22f, 0.28f, 1.0f),
			ImVec4 (0.28f, 0.28f, 0.34f, 1.0f),

			ImVec4 (0.22f, 0.22f, 0.26f, 1.0f),

			ImVec4 (0.10f, 0.10f, 0.12f, 1.0f), ImVec4 (0.28f, 0.28f, 0.32f, 1.0f),
			ImVec4 (0.36f, 0.36f, 0.40f, 1.0f), ImVec4 (0.44f, 0.44f, 0.48f, 1.0f),

			ImVec4 (0.90f, 0.90f, 0.92f, 1.0f), ImVec4 (0.50f, 0.50f, 0.55f, 1.0f),

			ImVec4 (0.08f, 0.08f, 0.10f, 1.0f),

			ImVec4 (0.22f, 0.35f, 0.55f, 1.0f), ImVec4 (0.26f, 0.40f, 0.60f, 1.0f),
			ImVec4 (0.30f, 0.45f, 0.65f, 1.0f),

			ImVec4 (0.0f, 0.0f, 0.0f, 0.0f),	ImVec4 (0.18f, 0.18f, 0.22f, 1.0f),
			ImVec4 (0.24f, 0.24f, 0.30f, 1.0f),
		};
	}

	ThemeConfig
	GruvboxDarkTheme () {
		const ImVec4 bg0 = RGBA (40, 40, 40);
		const ImVec4 bg0_h = RGBA (29, 32, 33);
		const ImVec4 bg1 = RGBA (60, 56, 54);
		const ImVec4 bg2 = RGBA (80, 73, 69);
		const ImVec4 bg3 = RGBA (102, 92, 84);
		const ImVec4 fg = RGBA (235, 219, 178);
		const ImVec4 fgDim = RGBA (168, 153, 132);
		const ImVec4 blue = RGBA (69, 133, 136);
		const ImVec4 blueBr = RGBA (131, 165, 152);

		ThemeConfig config{};
		config.windowBg = bg0;
		config.childBg = Transparent ();
		config.button = bg2;
		config.buttonHovered = bg3;
		config.buttonActive = ScaleRGB (bg3, 1.15f);
		config.header = blue;
		config.headerHovered = blueBr;
		config.headerActive = ScaleRGB (blueBr, 1.12f);
		config.frameBg = bg1;
		config.frameBgHovered = bg2;
		config.frameBgActive = bg3;
		config.separator = bg2;
		config.scrollbarBg = bg0_h;
		config.scrollbarGrab = bg2;
		config.scrollbarGrabHovered = bg3;
		config.scrollbarGrabActive = ScaleRGB (bg3, 1.15f);
		config.text = fg;
		config.textDisabled = fgDim;
		config.sidebarBg = bg0_h;
		config.navActiveHeader = blue;
		config.navActiveHovered = blueBr;
		config.navActivePressed = ScaleRGB (blueBr, 1.12f);
		config.navInactiveHeader = Transparent ();
		config.navInactiveHovered = bg1;
		config.navInactivePressed = bg2;
		return config;
	}

	ThemeConfig
	TokyonightTheme () {
		const ImVec4 bg = RGBA (36, 40, 59);
		const ImVec4 bgDark = RGBA (30, 30, 46);
		const ImVec4 bg1 = RGBA (52, 56, 77);
		const ImVec4 bg2 = RGBA (65, 72, 104);
		const ImVec4 bg3 = RGBA (86, 95, 137);
		const ImVec4 fg = RGBA (192, 202, 245);
		const ImVec4 fgDim = RGBA (86, 95, 137);
		const ImVec4 blue = RGBA (122, 162, 247);
		const ImVec4 blueBr = RGBA (125, 207, 255);
		const ImVec4 magenta = RGBA (187, 154, 247);

		ThemeConfig config{};
		config.windowBg = bg;
		config.childBg = Transparent ();
		config.button = bg2;
		config.buttonHovered = bg3;
		config.buttonActive = ScaleRGB (bg3, 1.15f);
		config.header = blue;
		config.headerHovered = blueBr;
		config.headerActive = ScaleRGB (blueBr, 1.10f);
		config.frameBg = bg1;
		config.frameBgHovered = bg2;
		config.frameBgActive = bg3;
		config.separator = bg2;
		config.scrollbarBg = bgDark;
		config.scrollbarGrab = bg2;
		config.scrollbarGrabHovered = bg3;
		config.scrollbarGrabActive = ScaleRGB (bg3, 1.15f);
		config.text = fg;
		config.textDisabled = fgDim;
		config.sidebarBg = bgDark;
		config.navActiveHeader = blue;
		config.navActiveHovered = magenta;
		config.navActivePressed = ScaleRGB (magenta, 1.10f);
		config.navInactiveHeader = Transparent ();
		config.navInactiveHovered = bg1;
		config.navInactivePressed = bg2;
		return config;
	}

	ThemeConfig
	DraculaTheme () {
		const ImVec4 bg = RGBA (40, 42, 54);
		const ImVec4 bgDark = RGBA (33, 34, 44);
		const ImVec4 bg1 = RGBA (55, 58, 77);
		const ImVec4 bg2 = RGBA (68, 71, 90);
		const ImVec4 bg3 = RGBA (98, 114, 164);
		const ImVec4 fg = RGBA (248, 248, 242);
		const ImVec4 fgDim = RGBA (98, 114, 164);
		const ImVec4 purple = RGBA (189, 147, 249);
		const ImVec4 pink = RGBA (255, 121, 198);

		ThemeConfig config{};
		config.windowBg = bg;
		config.childBg = Transparent ();
		config.button = bg2;
		config.buttonHovered = bg3;
		config.buttonActive = ScaleRGB (bg3, 1.15f);
		config.header = purple;
		config.headerHovered = pink;
		config.headerActive = ScaleRGB (pink, 1.10f);
		config.frameBg = bg1;
		config.frameBgHovered = bg2;
		config.frameBgActive = bg3;
		config.separator = bg2;
		config.scrollbarBg = bgDark;
		config.scrollbarGrab = bg2;
		config.scrollbarGrabHovered = bg3;
		config.scrollbarGrabActive = ScaleRGB (bg3, 1.15f);
		config.text = fg;
		config.textDisabled = fgDim;
		config.sidebarBg = bgDark;
		config.navActiveHeader = purple;
		config.navActiveHovered = pink;
		config.navActivePressed = ScaleRGB (pink, 1.10f);
		config.navInactiveHeader = Transparent ();
		config.navInactiveHovered = bg1;
		config.navInactivePressed = bg2;
		return config;
	}

	ThemeConfig
	PresetTheme (ThemePreset preset) {
		switch (preset) {
			case ThemePreset::Default: return DarkTheme ();
			case ThemePreset::GruvboxDark: return GruvboxDarkTheme ();
			case ThemePreset::Tokyonight: return TokyonightTheme ();
			case ThemePreset::Dracula: return DraculaTheme ();
		}
		return DarkTheme ();
	}

	ThemeConfig
	SnapThemeForTUI (const ThemeConfig& src) {
		ThemeConfig out{};
		out.windowBg = SnapOrKeep (src.windowBg);
		out.childBg = SnapOrKeep (src.childBg);
		out.button = SnapOrKeep (src.button);
		out.buttonHovered = SnapOrKeep (src.buttonHovered);
		out.buttonActive = SnapOrKeep (src.buttonActive);
		out.header = SnapOrKeep (src.header);
		out.headerHovered = SnapOrKeep (src.headerHovered);
		out.headerActive = SnapOrKeep (src.headerActive);
		out.frameBg = SnapOrKeep (src.frameBg);
		out.frameBgHovered = SnapOrKeep (src.frameBgHovered);
		out.frameBgActive = SnapOrKeep (src.frameBgActive);
		out.separator = SnapOrKeep (src.separator);
		out.scrollbarBg = SnapOrKeep (src.scrollbarBg);
		out.scrollbarGrab = SnapOrKeep (src.scrollbarGrab);
		out.scrollbarGrabHovered = SnapOrKeep (src.scrollbarGrabHovered);
		out.scrollbarGrabActive = SnapOrKeep (src.scrollbarGrabActive);
		out.text = SnapOrKeep (src.text);
		out.textDisabled = SnapOrKeep (src.textDisabled);
		out.sidebarBg = SnapOrKeep (src.sidebarBg);
		out.navActiveHeader = SnapOrKeep (src.navActiveHeader);
		out.navActiveHovered = SnapOrKeep (src.navActiveHovered);
		out.navActivePressed = SnapOrKeep (src.navActivePressed);
		out.navInactiveHeader = SnapOrKeep (src.navInactiveHeader);
		out.navInactiveHovered = SnapOrKeep (src.navInactiveHovered);
		out.navInactivePressed = SnapOrKeep (src.navInactivePressed);
		return out;
	}

	void
	ApplyTheme (const ThemeConfig& config) {
		ImGuiContext* currentContext = ImGui::GetCurrentContext ();
		if (!currentContext) return;

		ThemeConfig& activeTheme = ActiveThemeStorage ();
		if (StyledContext () == currentContext && SameTheme (activeTheme, config)) return;

		activeTheme = config;

		ImGui::StyleColorsDark ();

		ImGuiStyle& style = ImGui::GetStyle ();
		style.WindowRounding = 0.0f;
		style.FrameRounding = kMetrics.frameRounding;
		style.GrabRounding = kMetrics.frameRounding;
		style.ChildRounding = 0.0f;
		style.ScrollbarSize = kMetrics.scrollbarSize;
		style.FramePadding = kMetrics.styleFramePadding;
		style.ItemSpacing = kMetrics.styleItemSpacing;
		style.WindowPadding = kMetrics.styleWindowPadding;

		ImVec4* colors = style.Colors;
		const ThemeConfig& theme = ActiveThemeStorage ();
		colors [ImGuiCol_WindowBg] = theme.windowBg;
		colors [ImGuiCol_ChildBg] = theme.childBg;
		colors [ImGuiCol_Button] = theme.button;
		colors [ImGuiCol_ButtonHovered] = theme.buttonHovered;
		colors [ImGuiCol_ButtonActive] = theme.buttonActive;
		colors [ImGuiCol_Header] = theme.header;
		colors [ImGuiCol_HeaderHovered] = theme.headerHovered;
		colors [ImGuiCol_HeaderActive] = theme.headerActive;
		colors [ImGuiCol_FrameBg] = theme.frameBg;
		colors [ImGuiCol_FrameBgHovered] = theme.frameBgHovered;
		colors [ImGuiCol_FrameBgActive] = theme.frameBgActive;
		colors [ImGuiCol_Separator] = theme.separator;
		colors [ImGuiCol_ScrollbarBg] = theme.scrollbarBg;
		colors [ImGuiCol_ScrollbarGrab] = theme.scrollbarGrab;
		colors [ImGuiCol_ScrollbarGrabHovered] = theme.scrollbarGrabHovered;
		colors [ImGuiCol_ScrollbarGrabActive] = theme.scrollbarGrabActive;
		colors [ImGuiCol_Text] = theme.text;
		colors [ImGuiCol_TextDisabled] = theme.textDisabled;

		StyledContext () = currentContext;
	}

	const ThemeConfig&
	ActiveTheme () {
		return ActiveThemeStorage ();
	}

	ImVec4
	StatusColor (bool connected) {
		if (connected) return {80.0f / 255.0f, 200.0f / 255.0f, 120.0f / 255.0f, 1.0f};
		return {180.0f / 255.0f, 180.0f / 255.0f, 190.0f / 255.0f, 1.0f};
	}

	ImVec4
	DbTypeColor (const std::string& type) {
		if (type == "postgresql")
			return {59.0f / 255.0f, 89.0f / 255.0f, 152.0f / 255.0f, 1.0f};
		if (type == "mysql") return {0.0f, 117.0f / 255.0f, 143.0f / 255.0f, 1.0f};
		if (type == "sqlite") return {68.0f / 255.0f, 138.0f / 255.0f, 1.0f, 1.0f};
		return {120.0f / 255.0f, 120.0f / 255.0f, 120.0f / 255.0f, 1.0f};
	}

}  // namespace ambidb::ui
