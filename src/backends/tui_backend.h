#pragma once
#include "../app.h"
#include <memory>

namespace ambidb {

class TuiBackend {
public:
    TuiBackend();
    ~TuiBackend();

    bool Initialize();
    void Run();
    void Shutdown();

private:
    std::unique_ptr<App> m_app;
    void* m_screen = nullptr;
};

} // namespace ambidb
