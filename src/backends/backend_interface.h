#pragma once

#include <string>

namespace ambidb {

/**
 * @brief Abstract interface for backend implementations.
 *
 * This interface defines the contract that both GUI and TUI backends must implement.
 * It provides a clear separation between the application logic and the rendering layer.
 *
 * Lifecycle:
 * 1. Construction (backend-specific resources)
 * 2. Initialize() - Set up ImGui context and backend-specific initialization
 * 3. Run() - Main event loop
 * 4. Shutdown() - Clean up resources
 */
class IBackend {
public:
    virtual ~IBackend() = default;

    /**
     * @brief Initialize the backend and ImGui context.
     * @return true if initialization succeeded, false otherwise.
     */
    virtual bool Initialize() = 0;

    /**
     * @brief Run the main event loop.
     * This method blocks until the application should exit.
     */
    virtual void Run() = 0;

    /**
     * @brief Shutdown the backend and clean up resources.
     */
    virtual void Shutdown() = 0;

    /**
     * @brief Get the name of the backend (e.g., "GUI", "TUI").
     * @return Backend name for logging and diagnostics.
     */
    virtual const char* GetName() const = 0;
};

} // namespace ambidb
