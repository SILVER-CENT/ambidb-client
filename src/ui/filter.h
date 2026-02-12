#pragma once

#include "imgui.h"

namespace ambidb::ui {

	class TextFilter {
	public:
		explicit TextFilter (const char* defaultFilter = "");

		bool
		Draw (const char* label = "Filter (inc,-exc)", float width = 0.0f);

		bool
		PassFilter (const char* text) const;

		bool
		IsActive () const;

		void
		Clear ();

	private:
		ImGuiTextFilter m_filter;
	};

}  // namespace ambidb::ui
