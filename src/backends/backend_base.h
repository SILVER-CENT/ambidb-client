#pragma once

#include "backend_interface.h"
#include "../app.h"
#include <memory>
#include <iostream>

namespace ambidb {

/**
 * @brief Base class providing common functionality for all backends.
 *
 * This class implements the Template Method pattern, where the overall
 * lifecycle is defined here, but specific initialization and rendering
 * steps are delegated to derived classes.
 */
class BackendBase : public IBackend {
public:
    BackendBase() : m_app(std::make_unique<App>()) {}
    virtual ~BackendBase() = default;

    bool Initialize() final {
        std::cout << "[" << GetName() << "] Initializing backend..." << std::endl;

        if (!InitializeBackend()) {
            std::cerr << "[" << GetName() << "] Backend initialization failed!" << std::endl;
            return false;
        }

        if (!InitializeImGui()) {
            std::cerr << "[" << GetName() << "] ImGui initialization failed!" << std::endl;
            return false;
        }

        std::cout << "[" << GetName() << "] Initialization complete." << std::endl;
        return true;
    }

    void Shutdown() final {
        std::cout << "[" << GetName() << "] Shutting down..." << std::endl;
        ShutdownImGui();
        ShutdownBackend();
        std::cout << "[" << GetName() << "] Shutdown complete." << std::endl;
    }

protected:
    std::unique_ptr<App> m_app;

    /**
     * @brief Initialize backend-specific resources (window, screen, etc.).
     * @return true if successful, false otherwise.
     */
    virtual bool InitializeBackend() = 0;

    /**
     * @brief Initialize ImGui context and backend-specific ImGui setup.
     * @return true if successful, false otherwise.
     */
    virtual bool InitializeImGui() = 0;

    /**
     * @brief Shutdown ImGui context and backend-specific ImGui cleanup.
     */
    virtual void ShutdownImGui() = 0;

    /**
     * @brief Shutdown backend-specific resources.
     */
    virtual void ShutdownBackend() = 0;
};

} // namespace ambidb
