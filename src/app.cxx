#include "app.h"

#include "ui/ui.h"

#include <string>

namespace ambidb {

	namespace {

		const char*
		PageTitle (Page page) {
			switch (page) {
				case Page::Dashboard: return "Dashboard";
				case Page::Connections: return "Connections";
				case Page::QueryEditor: return "Query Editor";
				case Page::SchemaBrowser: return "Schema Browser";
				case Page::DataGrid: return "Data Grid";
				case Page::QueryHistory: return "Query History";
				case Page::Settings: return "Settings";
			}
			UNREACHABLE ();
		}

	}  // namespace

	App::App () {
		m_connections = {
			{"Production DB", "postgresql", true},
			{"Local MySQL", "mysql", true},
			{"Test SQLite", "sqlite", false},
		};
	}

	void
	App::Update () {
		ui::ApplyTheme ();

		ui::BeginAppShell ();

		ui::BeginSidebar ();
		RenderSidebar ();
		ui::EndSidebar ();

		ImGui::SameLine ();
		ui::VerticalDivider ();

		ui::BeginContent ();
		RenderContent ();
		ui::EndContent ();

		ui::EndAppShell ();
	}

	void
	App::ConnectionEntry (const ConnectionInfo& conn) {
		ImGui::PushID (conn.name.c_str ());

		ui::StatusDot (conn.connected);
		ImGui::SameLine (0.0f, ui::kMetrics.styleItemSpacing.x);
		ImGui::TextUnformatted (conn.name.c_str ());
		ImGui::SameLine ();
		ui::TypeBadge (conn.type);

		ImGui::PopID ();
	}

	void
	App::RenderSidebar () {
		ImGui::PushStyleVar (ImGuiStyleVar_WindowPadding, ui::kMetrics.sidebarPadding);

		ui::Gap (ui::kMetrics.topGapY);
		ui::AppTitle ("DBClient");

		ui::Gap (ui::kMetrics.rowGapY);

		const ui::ThemeConfig& theme = ui::ActiveTheme ();
		const float buttonWidth = ImGui::GetContentRegionAvail ().x;
		ImGui::PushStyleColor (ImGuiCol_Button, theme.button);
		ImGui::PushStyleColor (ImGuiCol_ButtonHovered, theme.buttonHovered);
		ImGui::PushStyleColor (ImGuiCol_ButtonActive, theme.buttonActive);
		ImGui::PushStyleVar (ImGuiStyleVar_FrameRounding, ui::kMetrics.frameRounding);
		ImGui::PushStyleVar (ImGuiStyleVar_FramePadding, ImVec2 (0.0f, ui::kMetrics.primaryButtonPadY));
		if (ImGui::Button ("+ New Connection", ImVec2 (buttonWidth, 0.0f))) {
			// Placeholder action for a future connection dialog.
		}
		ImGui::PopStyleVar (2);
		ImGui::PopStyleColor (3);

		ui::Gap (ui::kMetrics.sectionGapY);
		ImGui::Separator ();
		ui::Gap (ui::kMetrics.rowGapY);

		if (ui::NavItem ("[D]", "Dashboard", m_activePage == Page::Dashboard)) {
			m_activePage = Page::Dashboard;
		}
		if (ui::NavItem ("[C]", "Connections", m_activePage == Page::Connections)) {
			m_activePage = Page::Connections;
		}
		if (ui::NavItem ("[Q]", "Query Editor", m_activePage == Page::QueryEditor)) {
			m_activePage = Page::QueryEditor;
		}
		if (ui::NavItem ("[S]", "Schema Browser", m_activePage == Page::SchemaBrowser)) {
			m_activePage = Page::SchemaBrowser;
		}
		if (ui::NavItem ("[G]", "Data Grid", m_activePage == Page::DataGrid)) {
			m_activePage = Page::DataGrid;
		}
		if (ui::NavItem ("[H]", "Query History", m_activePage == Page::QueryHistory)) {
			m_activePage = Page::QueryHistory;
		}
		if (ui::NavItem ("[*]", "Settings", m_activePage == Page::Settings)) {
			m_activePage = Page::Settings;
		}

		ui::Gap (ui::kMetrics.sectionGapY);
		ImGui::Separator ();
		ui::Gap (ui::kMetrics.rowGapY);

		ImGui::PushStyleColor (ImGuiCol_Text, ImGui::GetStyleColorVec4 (ImGuiCol_TextDisabled));
		m_connectionsExpanded = ImGui::TreeNodeEx ("CONNECTIONS",
												   ImGuiTreeNodeFlags_DefaultOpen |
													   ImGuiTreeNodeFlags_SpanAvailWidth);
		ImGui::PopStyleColor ();

		if (m_connectionsExpanded) {
			for (const ConnectionInfo& conn: m_connections) {
				ConnectionEntry (conn);
			}
			ImGui::TreePop ();
		}

		const float statusReserve = ImGui::GetTextLineHeightWithSpacing () + ui::kMetrics.statusReserveExtraY;
		ui::PinToBottom (statusReserve);

		ImGui::Separator ();
		ui::Gap (ui::kMetrics.rowGapY);

		ui::StatusDot (true);
		ImGui::SameLine (0.0f, ui::kMetrics.styleItemSpacing.x);
		ui::TextMuted ("System Ready");

		ImGui::PopStyleVar ();
	}

	void
	App::RenderContent () {
		ImGui::PushStyleVar (ImGuiStyleVar_WindowPadding, ui::kMetrics.contentPadding);

		const char* title = PageTitle (m_activePage);

		ui::Gap (ui::kMetrics.topGapY);
		ui::ContentHeading (title);

		ui::AlignContentStart ();
		ui::TextMuted ("Welcome back! Here's an overview of your database activity.");

		ui::Gap (ui::kMetrics.sectionGapY);
		ui::AlignContentStart ();
		ImGui::Separator ();
		ui::Gap (ui::kMetrics.sectionGapY);

		std::string contentHint = "(Content for \"";
		contentHint += title;
		contentHint += "\" goes here)";
		ui::AlignContentStart ();
		ui::TextMuted (contentHint.c_str ());

		ui::PinToBottom (ui::kMetrics.quitReserveY);
		ui::AlignContentStart ();
		if (ImGui::Button ("Quit")) {
			m_shouldClose = true;
		}

		ImGui::PopStyleVar ();
	}

}  // namespace ambidb
