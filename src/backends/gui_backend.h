#pragma once
#include "../app.h"
#include <memory>

struct GLFWwindow;

namespace ambidb {

class GuiBackend {
public:
    GuiBackend();
    ~GuiBackend();

    bool Initialize();
    void Run();
    void Shutdown();

private:
    std::unique_ptr<App> m_app;
    GLFWwindow* m_window = nullptr;
};

} // namespace ambidb
