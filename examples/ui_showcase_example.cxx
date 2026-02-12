#include "ui_showcase_example.h"

#include "ui/ui.h"

#include <array>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>

#include "imgui.h"

namespace showcase {

	namespace {

		struct ConnectionRow {
			const char* name;
			const char* dbType;
			bool connected;
			int latencyMs;
		};

		constexpr std::array<const char*, 5> kDbTypes{
			"postgresql",
			"mysql",
			"sqlite",
			"mariadb",
			"duckdb",
		};

		constexpr std::array<const char*, 5> kFishNames{
			"Bream",
			"Haddock",
			"Mackerel",
			"Pollock",
			"Tilefish",
		};

		constexpr std::array<ConnectionRow, 8> kConnectionRows{{
			{"prod-primary", "postgresql", true, 12},
			{"analytics", "mysql", true, 31},
			{"edge-cache", "sqlite", false, 0},
			{"warehouse", "duckdb", true, 8},
			{"staging", "postgresql", false, 0},
			{"legacy", "mariadb", true, 55},
			{"reports", "mysql", true, 19},
			{"playground", "sqlite", true, 2},
		}};

		enum class Page {
			Layout,
			Widgets,
			Hints,
			Selection,
			Dialogs,
			FilterAndTables,
			Forms,
			Theme,
			Metrics,
		};

		struct ShowcaseState {
			Page activePage{Page::Layout};
			bool quitRequested{false};
			int themePreset{0};
			bool databaseConnected{true};
			int widgetNavSelection{0};
			int comboSelection{0};
			int advancedComboSelection{1};
			int listSelection{0};
			std::array<bool, 5> selectedItems{true, false, true, false, false};
			int popupFishSelection{-1};
			std::array<bool, 5> popupToggles{true, false, false, false, false};
			bool dontAskAgain{false};
			std::string modalLastResult;
			ambidb::ui::TextFilter connectionFilter{};
			char nameInput [64] = "ambidb-user";
			char passwordInput [64] = "password123";
			int intInputValue{42};
			float dragValue{0.5f};
			float sliderValue{25.0f};
			float floatInputValue{3.14f};
			float progress{0.35f};
			bool animateProgress{false};
		};

		ShowcaseState&
		State () {
			static ShowcaseState state;
			return state;
		}

		const char*
		PageTitle (Page page) {
			switch (page) {
				case Page::Layout: return "Layout";
				case Page::Widgets: return "Widgets";
				case Page::Hints: return "Hints";
				case Page::Selection: return "Selection";
				case Page::Dialogs: return "Dialogs";
				case Page::FilterAndTables: return "Filter + Tables";
				case Page::Forms: return "Forms";
				case Page::Theme: return "Theme";
				case Page::Metrics: return "Metrics";
			}
			return "Layout";
		}

		void
		MetricLine (const char* label, float value) {
			ambidb::ui::AlignContentStart ();
			std::ostringstream stream;
			stream << label << ": " << std::fixed << std::setprecision (1) << value;
			const std::string line = stream.str ();
			ImGui::TextUnformatted (line.c_str ());
		}

		void
		RenderSidebar () {
			ShowcaseState& state = State ();

			ImGui::PushStyleVar (ImGuiStyleVar_WindowPadding, ambidb::ui::kMetrics.sidebarPadding);

			ambidb::ui::Gap (ambidb::ui::kMetrics.topGapY);
			ambidb::ui::AppTitle ("Parity Showcase");

			ambidb::ui::Gap (ambidb::ui::kMetrics.rowGapY);

			const ambidb::ui::ThemeConfig& theme = ambidb::ui::ActiveTheme ();
			const float buttonWidth = ImGui::GetContentRegionAvail ().x;
			ImGui::PushStyleColor (ImGuiCol_Button, theme.button);
			ImGui::PushStyleColor (ImGuiCol_ButtonHovered, theme.buttonHovered);
			ImGui::PushStyleColor (ImGuiCol_ButtonActive, theme.buttonActive);
			ImGui::PushStyleVar (ImGuiStyleVar_FrameRounding, ambidb::ui::kMetrics.frameRounding);
			ImGui::PushStyleVar (ImGuiStyleVar_FramePadding,
								 ImVec2 (0.0f, ambidb::ui::kMetrics.primaryButtonPadY));
			(void) ImGui::Button ("+ New Connection", ImVec2 (buttonWidth, 0.0f));
			ImGui::PopStyleVar (2);
			ImGui::PopStyleColor (3);

			ambidb::ui::Gap (ambidb::ui::kMetrics.sectionGapY);
			ImGui::Separator ();
			ambidb::ui::Gap (ambidb::ui::kMetrics.rowGapY);

			if (ambidb::ui::NavItem ("[L]", "Layout", state.activePage == Page::Layout)) {
				state.activePage = Page::Layout;
			}
			if (ambidb::ui::NavItem ("[W]", "Widgets", state.activePage == Page::Widgets)) {
				state.activePage = Page::Widgets;
			}
			if (ambidb::ui::NavItem ("[?]", "Hints", state.activePage == Page::Hints)) {
				state.activePage = Page::Hints;
			}
			if (ambidb::ui::NavItem ("[S]", "Selection", state.activePage == Page::Selection)) {
				state.activePage = Page::Selection;
			}
			if (ambidb::ui::NavItem ("[P]", "Dialogs", state.activePage == Page::Dialogs)) {
				state.activePage = Page::Dialogs;
			}
			if (ambidb::ui::NavItem ("[F]", "Filter + Tables", state.activePage == Page::FilterAndTables)) {
				state.activePage = Page::FilterAndTables;
			}
			if (ambidb::ui::NavItem ("[I]", "Forms", state.activePage == Page::Forms)) {
				state.activePage = Page::Forms;
			}
			if (ambidb::ui::NavItem ("[T]", "Theme", state.activePage == Page::Theme)) {
				state.activePage = Page::Theme;
			}
			if (ambidb::ui::NavItem ("[M]", "Metrics", state.activePage == Page::Metrics)) {
				state.activePage = Page::Metrics;
			}

			ambidb::ui::Gap (ambidb::ui::kMetrics.sectionGapY);
			ImGui::Separator ();
			ambidb::ui::Gap (ambidb::ui::kMetrics.rowGapY);

			ImGui::PushStyleColor (ImGuiCol_Text, ImGui::GetStyleColorVec4 (ImGuiCol_TextDisabled));
			const bool componentsOpen = ImGui::TreeNodeEx ("COMPONENTS",
														   ImGuiTreeNodeFlags_DefaultOpen |
															   ImGuiTreeNodeFlags_SpanAvailWidth);
			ImGui::PopStyleColor ();

			if (componentsOpen) {
				ambidb::ui::StatusDot (true);
				ImGui::SameLine (0.0f, ambidb::ui::kMetrics.styleItemSpacing.x);
				ImGui::TextUnformatted ("StatusDot(true)");
				ambidb::ui::StatusDot (false);
				ImGui::SameLine (0.0f, ambidb::ui::kMetrics.styleItemSpacing.x);
				ImGui::TextUnformatted ("StatusDot(false)");
				ambidb::ui::TypeBadge ("postgresql");
				ImGui::SameLine ();
				ambidb::ui::TypeBadge ("mysql");
				ImGui::SameLine ();
				ambidb::ui::TypeBadge ("sqlite");
				ImGui::TreePop ();
			}

			const float footerReserve = ImGui::GetTextLineHeightWithSpacing () +
										ambidb::ui::kMetrics.statusReserveExtraY;
			ambidb::ui::PinToBottom (footerReserve);

			ImGui::Separator ();
			ambidb::ui::Gap (ambidb::ui::kMetrics.rowGapY);
			ambidb::ui::StatusDot (state.databaseConnected);
			ImGui::SameLine (0.0f, ambidb::ui::kMetrics.styleItemSpacing.x);
			ambidb::ui::TextMuted (state.databaseConnected ? "System Ready" : "System Offline");

			ImGui::PopStyleVar ();
		}

		void
		RenderLayoutPage () {
			ambidb::ui::AlignContentStart ();
			ambidb::ui::ContentHeading ("Layout Helpers");
			ambidb::ui::AlignContentStart ();
			ambidb::ui::TextMuted ("BeginAppShell/EndAppShell + BeginSidebar/EndSidebar "
								   "+ BeginContent/EndContent");
			ambidb::ui::AlignContentStart ();
			ambidb::ui::TextMuted (
				"VerticalDivider and PinToBottom are shared across GUI and TUI.");
			ambidb::ui::Gap (ambidb::ui::kMetrics.rowGapY);
			ambidb::ui::AlignContentStart ();
			ambidb::ui::TextMuted ("This page uses one render path with backend "
								   "differences from ui::kMetrics/ui::kCaps.");
		}

		void
		RenderWidgetsPage () {
			ShowcaseState& state = State ();

			ambidb::ui::AlignContentStart ();
			ambidb::ui::ContentHeading ("Widget Helpers");
			ambidb::ui::Gap (ambidb::ui::kMetrics.rowGapY);

			ambidb::ui::AlignContentStart ();
			ambidb::ui::StatusDot (state.databaseConnected);
			ImGui::SameLine (0.0f, ambidb::ui::kMetrics.styleItemSpacing.x);
			ImGui::Checkbox ("Connected##widgets", &state.databaseConnected);

			ambidb::ui::AlignContentStart ();
			ambidb::ui::TypeBadge ("postgresql");
			ImGui::SameLine ();
			ambidb::ui::TypeBadge ("mysql");
			ImGui::SameLine ();
			ambidb::ui::TypeBadge ("sqlite");

			ambidb::ui::Gap (ambidb::ui::kMetrics.rowGapY);
			ambidb::ui::AlignContentStart ();
			ambidb::ui::TextMuted ("NavItem is stateless: caller owns active index.");

			if (ambidb::ui::NavItem ("[1]", "Sample Item 1", state.widgetNavSelection == 0)) {
				state.widgetNavSelection = 0;
			}
			if (ambidb::ui::NavItem ("[2]", "Sample Item 2", state.widgetNavSelection == 1)) {
				state.widgetNavSelection = 1;
			}
		}

		void
		RenderHintsPage () {
			ambidb::ui::AlignContentStart ();
			ambidb::ui::ContentHeading ("Help Markers + User Guide");
			ambidb::ui::Gap (ambidb::ui::kMetrics.rowGapY);

			ambidb::ui::AlignContentStart ();
			ImGui::TextUnformatted ("Hover the marker:");
			ImGui::SameLine ();
			ambidb::ui::HelpMarker (
				"Reusable helper inspired by imtui-demo HelpMarker().\n"
				"Use it next to labels where advanced behavior needs an inline tip.");

			ambidb::ui::Gap (ambidb::ui::kMetrics.rowGapY);
			ambidb::ui::AlignContentStart ();
			ImGui::Separator ();
			ambidb::ui::Gap (ambidb::ui::kMetrics.rowGapY);

			ambidb::ui::AlignContentStart ();
			ambidb::ui::ShowUserGuide ();
		}

		void
		RenderSelectionPage () {
			ShowcaseState& state = State ();

			ambidb::ui::AlignContentStart ();
			ambidb::ui::ContentHeading ("Combo + Selectable Helpers");
			ambidb::ui::Gap (ambidb::ui::kMetrics.rowGapY);

			ambidb::ui::AlignContentStart ();
			(void) ambidb::ui::ComboCString ("DB engine##combo",
											 &state.comboSelection,
											 kDbTypes.data (),
											 static_cast<int> (kDbTypes.size ()));

			ambidb::ui::AlignContentStart ();
			if (ambidb::ui::BeginCombo ("Advanced combo", kDbTypes [state.advancedComboSelection])) {
				for (int i = 0; i < static_cast<int> (kDbTypes.size ()); ++i) {
					const bool selected = (state.advancedComboSelection == i);
					if (ambidb::ui::Selectable (kDbTypes [i], selected)) {
						state.advancedComboSelection = i;
					}
					if (selected) {
						ImGui::SetItemDefaultFocus ();
					}
				}
				ambidb::ui::EndCombo ();
			}

			ambidb::ui::Gap (ambidb::ui::kMetrics.rowGapY);
			ambidb::ui::AlignContentStart ();
			ImGui::TextUnformatted ("Selectable toggles:");
			for (int i = 0; i < static_cast<int> (state.selectedItems.size ()); ++i) {
				ambidb::ui::AlignContentStart ();
				std::string label = "Item ";
				label += std::to_string (i + 1);
				(void) ambidb::ui::SelectableToggle (label.c_str (), &state.selectedItems [i]);
			}

			ambidb::ui::Gap (ambidb::ui::kMetrics.rowGapY);
			ambidb::ui::AlignContentStart ();
			(void) ambidb::ui::SelectableList ("Fish list",
											   kFishNames.data (),
											   static_cast<int> (kFishNames.size ()),
											   &state.listSelection,
											   6.0f);
		}

		void
		RenderDialogsPage () {
			ShowcaseState& state = State ();

			ambidb::ui::AlignContentStart ();
			ambidb::ui::ContentHeading ("Popup + Modal Helpers");
			ambidb::ui::Gap (ambidb::ui::kMetrics.rowGapY);

			ambidb::ui::AlignContentStart ();
			if (ImGui::Button ("Select fish..")) {
				ambidb::ui::OpenPopup ("showcase_select_popup");
			}
			ImGui::SameLine ();
			const char* selectedFish = (state.popupFishSelection >= 0)
										   ? kFishNames [state.popupFishSelection]
										   : "[none]";
			ImGui::TextUnformatted (selectedFish);

			if (ambidb::ui::BeginPopup ("showcase_select_popup")) {
				for (int i = 0; i < static_cast<int> (kFishNames.size ()); ++i) {
					if (ambidb::ui::Selectable (kFishNames [i], state.popupFishSelection == i)) {
						state.popupFishSelection = i;
						ImGui::CloseCurrentPopup ();
					}
				}
				ambidb::ui::EndPopup ();
			}

			ambidb::ui::Gap (ambidb::ui::kMetrics.rowGapY);
			ambidb::ui::AlignContentStart ();
			ImGui::Button ("Right-click for context menu");
			if (ambidb::ui::BeginContextPopup ()) {
				for (int i = 0; i < static_cast<int> (kFishNames.size ()); ++i) {
					(void) ImGui::MenuItem (kFishNames [i], nullptr, &state.popupToggles [i]);
				}
				ambidb::ui::EndPopup ();
			}

			ambidb::ui::Gap (ambidb::ui::kMetrics.rowGapY);
			ambidb::ui::AlignContentStart ();
			if (ImGui::Button ("Delete...")) {
				ambidb::ui::OpenPopup ("Delete?##modal");
			}

			if (ambidb::ui::BeginCenteredModal ("Delete?##modal")) {
				ImGui::TextUnformatted ("Delete selected connection profile?");
				ImGui::Separator ();
				ImGui::Checkbox ("Don't ask next time", &state.dontAskAgain);
				const ambidb::ui::ModalResult result = ambidb::ui::ModalButtonRow ();
				if (result == ambidb::ui::ModalResult::Confirmed) {
					state.modalLastResult = "Last result: confirmed";
				}
				else if (result == ambidb::ui::ModalResult::Cancelled) {
					state.modalLastResult = "Last result: cancelled";
				}
				ambidb::ui::EndModal ();
			}

			if (!state.modalLastResult.empty ()) {
				ambidb::ui::AlignContentStart ();
				ImGui::TextUnformatted (state.modalLastResult.c_str ());
			}
		}

		void
		RenderFilterAndTablesPage () {
			ShowcaseState& state = State ();

			ambidb::ui::AlignContentStart ();
			ambidb::ui::ContentHeading ("Text Filter + Data Table");
			ambidb::ui::Gap (ambidb::ui::kMetrics.rowGapY);

			ambidb::ui::AlignContentStart ();
			(void) state.connectionFilter.Draw ("Filter (name/type)", 260.0f);

			ambidb::ui::Gap (ambidb::ui::kMetrics.rowGapY);

			ambidb::ui::TableConfig tableConfig;
			tableConfig.flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg |
								ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingStretchProp;

			if (ambidb::ui::BeginDataTable ("connections_table", static_cast<int> (4), tableConfig)) {
				ambidb::ui::SetupColumn ("Name");
				ambidb::ui::SetupColumn ("Type");
				ambidb::ui::SetupColumn ("Status");
				ambidb::ui::SetupColumn ("Latency");
				ambidb::ui::HeadersRow ();

				for (const ConnectionRow& row: kConnectionRows) {
					if (!state.connectionFilter.PassFilter (row.name) &&
						!state.connectionFilter.PassFilter (row.dbType)) {
						continue;
					}

					ambidb::ui::NextRow ();

					ambidb::ui::NextColumn ();
					ambidb::ui::CellText (row.name);

					ambidb::ui::NextColumn ();
					ambidb::ui::CellText (row.dbType);

					ambidb::ui::NextColumn ();
					ambidb::ui::CellText (row.connected ? "online" : "offline");

					ambidb::ui::NextColumn ();
					std::string latency = row.connected ? std::to_string (row.latencyMs) + " ms" : "--";
					ambidb::ui::CellText (latency.c_str ());
				}

				ambidb::ui::EndDataTable ();
			}
		}

		void
		RenderFormsPage () {
			ShowcaseState& state = State ();

			ambidb::ui::AlignContentStart ();
			ambidb::ui::ContentHeading ("Form/Input Helpers");
			ambidb::ui::Gap (ambidb::ui::kMetrics.rowGapY);

			const float fieldWidth = 260.0f;

			ambidb::ui::AlignContentStart ();
			(void) ambidb::ui::InputTextField ("User", state.nameInput, sizeof (state.nameInput), 0, fieldWidth);

			ambidb::ui::AlignContentStart ();
			(void) ambidb::ui::InputTextWithHintField ("Password",
													   "type secret",
													   state.passwordInput,
													   sizeof (state.passwordInput),
													   ImGuiInputTextFlags_Password,
													   fieldWidth);

			ambidb::ui::AlignContentStart ();
			(void) ambidb::ui::InputIntField ("Retries", &state.intInputValue, 1, 10, 0, fieldWidth);

			ambidb::ui::AlignContentStart ();
			(void) ambidb::ui::InputFloatField ("Timeout", &state.floatInputValue, 0.1f, 1.0f, "%.2f", 0, fieldWidth);

			ambidb::ui::AlignContentStart ();
			(void) ambidb::ui::DragFloatField (
				"Drag threshold", &state.dragValue, 0.01f, 0.0f, 1.0f, "%.2f", 0, fieldWidth);

			ambidb::ui::AlignContentStart ();
			(void) ambidb::ui::SliderFloatField ("Throttle", &state.sliderValue, 0.0f, 100.0f, "%.0f%%", 0, fieldWidth);

			if (state.animateProgress) {
				state.progress += ImGui::GetIO ().DeltaTime * 0.2f;
				if (state.progress > 1.0f) {
					state.progress = 0.0f;
				}
			}

			ambidb::ui::Gap (ambidb::ui::kMetrics.rowGapY);
			ambidb::ui::AlignContentStart ();
			ImGui::Checkbox ("Animate progress", &state.animateProgress);

			std::ostringstream overlayStream;
			overlayStream << std::fixed << std::setprecision (0) << (state.progress * 100.0f) << "%";
			const std::string overlay = overlayStream.str ();

			ambidb::ui::AlignContentStart ();
			ambidb::ui::ProgressBarLabeled (state.progress, overlay.c_str (), fieldWidth);
		}

		void
		ColorSwatch (const char* label, const ImVec4& color) {
			ambidb::ui::AlignContentStart ();
			ImGui::PushStyleColor (ImGuiCol_Button, color);
			ImGui::PushStyleColor (ImGuiCol_ButtonHovered, color);
			ImGui::PushStyleColor (ImGuiCol_ButtonActive, color);
			ImGui::Button ("##swatch",
						   ImVec2 (ImGui::GetTextLineHeight (), ImGui::GetTextLineHeight ()));
			ImGui::PopStyleColor (3);
			ImGui::SameLine ();

			std::ostringstream stream;
			stream << label << "  [" << std::fixed << std::setprecision (2) << color.x
				   << " " << color.y << " " << color.z << "]";
			const std::string line = stream.str ();
			ImGui::TextUnformatted (line.c_str ());
		}

		void
		RenderThemePage () {
			ShowcaseState& state = State ();

			ambidb::ui::AlignContentStart ();
			ambidb::ui::ContentHeading ("Theme Preset Selector");

			ambidb::ui::AlignContentStart ();
			const char* presetNames [] = {"Default", "Gruvbox Dark", "Tokyonight", "Dracula"};
			(void) ambidb::ui::ComboCString ("Preset##theme_preset", &state.themePreset, presetNames, 4);

			ambidb::ui::Gap (ambidb::ui::kMetrics.sectionGapY);

			const ambidb::ui::ThemeConfig& theme = ambidb::ui::ActiveTheme ();

			ambidb::ui::AlignContentStart ();
			ambidb::ui::ContentHeading ("Palette Swatches");
			ambidb::ui::Gap (ambidb::ui::kMetrics.rowGapY);

			ColorSwatch ("windowBg", theme.windowBg);
			ColorSwatch ("sidebarBg", theme.sidebarBg);
			ColorSwatch ("button", theme.button);
			ColorSwatch ("buttonHovered", theme.buttonHovered);
			ColorSwatch ("header", theme.header);
			ColorSwatch ("headerHovered", theme.headerHovered);
			ColorSwatch ("frameBg", theme.frameBg);
			ColorSwatch ("separator", theme.separator);
			ColorSwatch ("text", theme.text);
			ColorSwatch ("textDisabled", theme.textDisabled);
			ColorSwatch ("scrollbarGrab", theme.scrollbarGrab);
			ColorSwatch ("navActiveHeader", theme.navActiveHeader);

			ambidb::ui::Gap (ambidb::ui::kMetrics.sectionGapY);

			ambidb::ui::AlignContentStart ();
			ambidb::ui::ContentHeading ("Semantic Colors");
			ambidb::ui::Gap (ambidb::ui::kMetrics.rowGapY);

			ColorSwatch ("StatusColor(true)", ambidb::ui::StatusColor (true));
			ColorSwatch ("StatusColor(false)", ambidb::ui::StatusColor (false));
			ColorSwatch ("DbTypeColor(pg)", ambidb::ui::DbTypeColor ("postgresql"));
			ColorSwatch ("DbTypeColor(mysql)", ambidb::ui::DbTypeColor ("mysql"));
			ColorSwatch ("DbTypeColor(sqlite)", ambidb::ui::DbTypeColor ("sqlite"));

			ambidb::ui::Gap (ambidb::ui::kMetrics.sectionGapY);

			ambidb::ui::AlignContentStart ();
			ambidb::ui::ContentHeading ("ANSI-256 Snap Info");
			ambidb::ui::Gap (ambidb::ui::kMetrics.rowGapY);

			ambidb::ui::AlignContentStart ();
			const ImVec4 snapped = ambidb::ui::SnapToAnsi256 (theme.header);
			const uint8_t ansiIdx = ambidb::ui::ColorToAnsi256 (theme.header);
			std::ostringstream ansiStream;
			ansiStream << "header -> ANSI-256 index " << static_cast<int> (ansiIdx)
					   << "  snapped=[" << std::fixed << std::setprecision (2)
					   << snapped.x << " " << snapped.y << " " << snapped.z << "]";
			const std::string ansiLine = ansiStream.str ();
			ImGui::TextUnformatted (ansiLine.c_str ());

			ambidb::ui::AlignContentStart ();
			const float cr = ambidb::ui::ContrastRatio (theme.text, theme.windowBg);
			std::ostringstream crStream;
			crStream << "text/windowBg contrast ratio: " << std::fixed
					 << std::setprecision (1) << cr
					 << (ambidb::ui::HasSufficientContrast (theme.text, theme.windowBg)
							 ? "  (AA pass)"
							 : "  (AA fail)");
			const std::string crLine = crStream.str ();
			ImGui::TextUnformatted (crLine.c_str ());
		}

		void
		RenderMetricsPage () {
			ambidb::ui::AlignContentStart ();
			ambidb::ui::ContentHeading ("Metrics + Capabilities");
			ambidb::ui::Gap (ambidb::ui::kMetrics.rowGapY);

			MetricLine ("sidebarWidth", ambidb::ui::kMetrics.sidebarWidth);
			MetricLine ("frameRounding", ambidb::ui::kMetrics.frameRounding);
			MetricLine ("scrollbarSize", ambidb::ui::kMetrics.scrollbarSize);
			MetricLine ("topGapY", ambidb::ui::kMetrics.topGapY);
			MetricLine ("rowGapY", ambidb::ui::kMetrics.rowGapY);
			MetricLine ("sectionGapY", ambidb::ui::kMetrics.sectionGapY);

			ambidb::ui::AlignContentStart ();
			ImGui::TextUnformatted (ambidb::ui::kCaps.drawVerticalDivider
										? "drawVerticalDivider: true"
										: "drawVerticalDivider: false");
		}

		void
		RenderContent () {
			ShowcaseState& state = State ();

			ImGui::PushStyleVar (ImGuiStyleVar_WindowPadding, ambidb::ui::kMetrics.contentPadding);

			ambidb::ui::Gap (ambidb::ui::kMetrics.topGapY);
			ambidb::ui::ContentHeading (PageTitle (state.activePage));

			ambidb::ui::AlignContentStart ();
			ambidb::ui::TextMuted ("One shared render path for both backends. Only "
								   "metrics, style, and wrappers vary by backend.");

			ambidb::ui::Gap (ambidb::ui::kMetrics.sectionGapY);
			ambidb::ui::AlignContentStart ();
			ImGui::Separator ();
			ambidb::ui::Gap (ambidb::ui::kMetrics.sectionGapY);

			switch (state.activePage) {
				case Page::Layout: RenderLayoutPage (); break;
				case Page::Widgets: RenderWidgetsPage (); break;
				case Page::Hints: RenderHintsPage (); break;
				case Page::Selection: RenderSelectionPage (); break;
				case Page::Dialogs: RenderDialogsPage (); break;
				case Page::FilterAndTables: RenderFilterAndTablesPage (); break;
				case Page::Forms: RenderFormsPage (); break;
				case Page::Theme: RenderThemePage (); break;
				case Page::Metrics: RenderMetricsPage (); break;
			}

			ambidb::ui::PinToBottom (ambidb::ui::kMetrics.quitReserveY);
			ambidb::ui::AlignContentStart ();
			if (ImGui::Button ("Quit Showcase")) {
				state.quitRequested = true;
			}

			ImGui::PopStyleVar ();
		}

	}  // namespace

	bool
	RunShowcaseFrame () {
		ShowcaseState& state = State ();

		const auto preset = static_cast<ambidb::ui::ThemePreset> (state.themePreset);
		ambidb::ui::ThemeConfig theme = ambidb::ui::PresetTheme (preset);
#if defined(AMBIDB_TUI)
		theme = ambidb::ui::SnapThemeForTUI (theme);
#endif
		ambidb::ui::ApplyTheme (theme);

		ambidb::ui::BeginAppShell ();

		ambidb::ui::BeginSidebar ();
		RenderSidebar ();
		ambidb::ui::EndSidebar ();

		ImGui::SameLine ();
		ambidb::ui::VerticalDivider ();

		ambidb::ui::BeginContent ();
		RenderContent ();
		ambidb::ui::EndContent ();

		ambidb::ui::EndAppShell ();

		return state.quitRequested;
	}

}  // namespace showcase
