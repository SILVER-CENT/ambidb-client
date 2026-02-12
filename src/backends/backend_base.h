#pragma once

#include "backend_concept.h"
#include <app.h>
#include <functional>
#include <memory>
#include <print>

namespace ambidb {

	/**
	 * @brief CRTP base class providing common functionality for all backends.
	 *
	 * This class uses the Curiously Recurring Template Pattern (CRTP) to achieve
	 * compile-time polymorphism, eliminating the runtime overhead of virtual functions.
	 * The Backend concept can be used to validate derived classes satisfy the interface.
	 *
	 * Usage:
	 *   class MyBackend : public BackendBase<MyBackend> {
	 *   public:
	 *       bool InitializeBackend() { ... }
	 *       bool InitializeImGui() { ... }
	 *       void Run() { ... }
	 *       void ShutdownImGui() { ... }
	 *       void ShutdownBackend() { ... }
	 *       const char* GetName() const { return "MyBackend"; }
	 *   };
	 *
	 * The derived class must be a friend of BackendBase if methods are private, or make them public.
	 */
	template <typename Derived>
	class BackendBase {
	public:
		BackendBase () : m_app (std::make_unique<App> ()) {}
		~BackendBase () = default;

		// Non-copyable, non-movable
		MAKE_NONCOPYABLE (BackendBase);
		MAKE_NONMOVABLE (BackendBase);

		/**
		 * @brief Initialize the backend using the Template Method pattern.
		 * Orchestrates the initialization sequence, delegating to derived class.
		 */
		bool
		Initialize () {
			// Compile-time validation that Derived satisfies Backend concept
			static_assert (Backend<Derived>, "Derived class must satisfy Backend concept");

			std::println ("[{}] Initializing backend...", derived ().GetName ());

			if (!derived ().InitializeBackend ()) {
				std::println (stderr, "[{}] Backend initialization failed!", derived ().GetName ());
				return false;
			}

			if (!derived ().InitializeImGui ()) {
				std::println (stderr, "[{}] ImGui initialization failed!", derived ().GetName ());
				// Clean up backend resources since InitializeBackend() succeeded
				derived ().ShutdownBackend ();
				return false;
			}

			std::println ("[{}] Initialization complete.", derived ().GetName ());
			return true;
		}

		/**
		 * @brief Shutdown the backend in the correct order.
		 * Delegates to derived class for backend-specific cleanup.
		 */
		void
		Shutdown () {
			std::println ("[{}] Shutting down...", derived ().GetName ());
			derived ().ShutdownImGui ();
			derived ().ShutdownBackend ();
			std::println ("[{}] Shutdown complete.", derived ().GetName ());
		}

		/**
		 * @brief Run the main event loop.
		 * Delegates to derived class implementation.
		 */
		void
		Run () {
			derived ().Run ();
		}

		/**
		 * @brief Set an optional per-frame callback. If set, Run() uses it instead of App.
		 * Callback returns true when the app should close.
		 */
		void
		SetFrameCallback (std::function<bool ()> f) {
			m_frameCallback = std::move (f);
		}

	protected:
		/**
		 * @brief Run one frame: either the custom callback or App::Update().
		 * @return true if the main loop should exit.
		 */
		bool
		RunFrame () {
			if (m_frameCallback) {
				return m_frameCallback ();
			}
			m_app->Update ();
			return m_app->ShouldClose ();
		}

		std::unique_ptr<App> m_app;
		std::function<bool ()> m_frameCallback;

	private:
		/**
		 * @brief CRTP helper to get a reference to the derived class.
		 */
		Derived&
		derived () {
			return static_cast<Derived&> (*this);
		}

		/**
		 * @brief CRTP helper to get a const reference to the derived class.
		 */
		const Derived&
		derived () const {
			return static_cast<const Derived&> (*this);
		}
	};

}  // namespace ambidb
