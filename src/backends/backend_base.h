#pragma once

#include "backend_concept.h"
#include "../app.h"
#include <memory>
#include <iostream>

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
template<typename Derived>
class BackendBase {
public:
    BackendBase() : m_app(std::make_unique<App>()) {}
    ~BackendBase() = default;

    // Non-copyable, non-movable
    BackendBase(const BackendBase&) = delete;
    BackendBase& operator=(const BackendBase&) = delete;
    BackendBase(BackendBase&&) = delete;
    BackendBase& operator=(BackendBase&&) = delete;

    /**
     * @brief Initialize the backend using the Template Method pattern.
     * Orchestrates the initialization sequence, delegating to derived class.
     */
    bool Initialize() {
        // Compile-time validation that Derived satisfies Backend concept
        static_assert(Backend<Derived>, "Derived class must satisfy Backend concept");

        std::cout << "[" << derived().GetName() << "] Initializing backend..." << std::endl;

        if (!derived().InitializeBackend()) {
            std::cerr << "[" << derived().GetName() << "] Backend initialization failed!" << std::endl;
            return false;
        }

        if (!derived().InitializeImGui()) {
            std::cerr << "[" << derived().GetName() << "] ImGui initialization failed!" << std::endl;
            // Clean up backend resources since InitializeBackend() succeeded
            derived().ShutdownBackend();
            return false;
        }

        std::cout << "[" << derived().GetName() << "] Initialization complete." << std::endl;
        return true;
    }

    /**
     * @brief Shutdown the backend in the correct order.
     * Delegates to derived class for backend-specific cleanup.
     */
    void Shutdown() {
        std::cout << "[" << derived().GetName() << "] Shutting down..." << std::endl;
        derived().ShutdownImGui();
        derived().ShutdownBackend();
        std::cout << "[" << derived().GetName() << "] Shutdown complete." << std::endl;
    }

    /**
     * @brief Run the main event loop.
     * Delegates to derived class implementation.
     */
    void Run() {
        derived().Run();
    }

protected:
    std::unique_ptr<App> m_app;

private:
    /**
     * @brief CRTP helper to get a reference to the derived class.
     */
    Derived& derived() {
        return static_cast<Derived&>(*this);
    }

    /**
     * @brief CRTP helper to get a const reference to the derived class.
     */
    const Derived& derived() const {
        return static_cast<const Derived&>(*this);
    }
};

} // namespace ambidb
