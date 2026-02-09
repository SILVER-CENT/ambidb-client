#pragma once

#include "imgui.h"

namespace ambidb {

class App {
public:
    App();
    ~App();

    void Update();
    bool ShouldClose() const { return m_shouldClose; }

private:
    bool m_shouldClose = false;
    int m_counter = 0;
};

} // namespace ambidb
