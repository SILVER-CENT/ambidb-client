#include "filter.h"

namespace ambidb::ui {

	TextFilter::TextFilter (const char* defaultFilter) : m_filter (defaultFilter) {}

	bool
	TextFilter::Draw (const char* label, float width) {
		if (width > 0.0f) {
			return m_filter.Draw (label, width);
		}
		return m_filter.Draw (label);
	}

	bool
	TextFilter::PassFilter (const char* text) const {
		return m_filter.PassFilter (text);
	}

	bool
	TextFilter::IsActive () const {
		return m_filter.IsActive ();
	}

	void
	TextFilter::Clear () {
		m_filter.Clear ();
	}

}  // namespace ambidb::ui
