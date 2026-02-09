#pragma once
#include <backends/backend_base.h>
#include <backends/backend_config.h>

struct GLFWwindow;

namespace ambidb {

	/**
	 * @brief GUI backend implementation using GLFW and OpenGL.
	 *
	 * This backend provides a hardware-accelerated graphical interface
	 * suitable for desktop environments with display servers.
	 *
	 * Uses CRTP pattern via BackendBase<GuiBackend> for compile-time polymorphism.
	 */
	class GuiBackend : public BackendBase<GuiBackend> {
	public:
		MAKE_NONCOPYABLE (GuiBackend);
		MAKE_NONMOVABLE (GuiBackend);
		GuiBackend () = default;
		~GuiBackend () = default;

		// Backend interface implementation
		void
		Run ();
		const char*
		GetName () const {
			return "GUI";
		}
		bool
		InitializeBackend ();
		bool
		InitializeImGui ();
		void
		ShutdownImGui ();
		void
		ShutdownBackend ();

	private:
		GLFWwindow* m_window = nullptr;
	};

}  // namespace ambidb
