#pragma once

#include <string>

namespace ambidb::ui {

	void
	TextMuted (const char* text);

	void
	StatusDot (bool connected);

	void
	TypeBadge (const std::string& type);

	bool
	NavItem (const char* icon, const char* label, bool isActive);

	void
	Gap (float height);

	void
	AppTitle (const char* text);

	void
	ContentHeading (const char* text);

	void
	AlignContentStart ();

}  // namespace ambidb::ui
