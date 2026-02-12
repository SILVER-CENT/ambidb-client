#pragma once

namespace ambidb::ui {

	void
	BeginAppShell ();

	void
	EndAppShell ();

	void
	BeginSidebar (float width = 0.0f);

	void
	EndSidebar ();

	void
	BeginContent ();

	void
	EndContent ();

	void
	VerticalDivider ();

	void
	PinToBottom (float reserveHeight);

}  // namespace ambidb::ui
