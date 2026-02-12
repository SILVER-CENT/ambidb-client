#pragma once
#include <macro.h>
#include <string>
#include <vector>

namespace ambidb {

	enum class Page {
		Dashboard,
		Connections,
		QueryEditor,
		SchemaBrowser,
		DataGrid,
		QueryHistory,
		Settings,
	};

	struct ConnectionInfo {
		std::string name;
		std::string type;  // "postgresql", "mysql", "sqlite"
		bool connected{false};
	};

	class App {
	public:
		MAKE_NONCOPYABLE (App);
		MAKE_NONMOVABLE (App);
		App ();
		~App () = default;

		void
		Update ();
		bool
		ShouldClose () const {
			return m_shouldClose;
		}

	private:
		void
		RenderSidebar ();
		void
		RenderContent ();
		void
		ConnectionEntry (const ConnectionInfo& conn);

		bool m_shouldClose{false};

		Page m_activePage{Page::Dashboard};
		bool m_connectionsExpanded{true};

		std::vector<ConnectionInfo> m_connections;
	};

}  // namespace ambidb
