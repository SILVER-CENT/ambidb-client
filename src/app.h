#pragma once
#include <macro.h>
#include "imgui.h"

namespace ambidb {

	class App {
	public:
		MAKE_NONCOPYABLE (App);
		MAKE_NONMOVABLE (App);
		App () = default;
		~App () = default;

		void
		Update ();
		bool
		ShouldClose () const {
			return m_shouldClose;
		}

	private:
		bool m_shouldClose{false};
		int m_counter{0};
		bool m_hasCenteredWindow{true};
	};

}  // namespace ambidb
