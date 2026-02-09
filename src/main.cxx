#ifdef AMBIDB_GUI
#  include "backends/gui/backend.h"
using Backend = ambidb::GuiBackend;
#elif defined(AMBIDB_TUI)
#  include "backends/tui/backend.h"
using Backend = ambidb::TuiBackend;
#else
#  error "No backend defined! Set AMBIDB_GUI or AMBIDB_TUI"
#endif

int
main () {
	Backend backend;

	if (!backend.Initialize ()) {
		return 1;
	}

	backend.Run ();
	backend.Shutdown ();

	return 0;
}
