#pragma once

#include "backend_base.h"

struct GLFWwindow;

namespace ambidb {

/**
 * @brief GUI backend implementation using GLFW and OpenGL.
 *
 * This backend provides a hardware-accelerated graphical interface
 * suitable for desktop environments with display servers.
 */
class GuiBackend : public BackendBase {
public:
    GuiBackend() = default;
    ~GuiBackend() override = default;

    void Run() override;
    const char* GetName() const override { return "GUI"; }

protected:
    bool InitializeBackend() override;
    bool InitializeImGui() override;
    void ShutdownImGui() override;
    void ShutdownBackend() override;

private:
    GLFWwindow* m_window = nullptr;
};

} // namespace ambidb
