#pragma once

#include "backend_base.h"

namespace ambidb {

/**
 * @brief TUI backend implementation using ncurses and ImTui.
 *
 * This backend provides a terminal-based interface suitable for
 * SSH sessions and headless servers without display servers.
 */
class TuiBackend : public BackendBase {
public:
    TuiBackend() = default;
    ~TuiBackend() override = default;

    void Run() override;
    const char* GetName() const override { return "TUI"; }

protected:
    bool InitializeBackend() override;
    bool InitializeImGui() override;
    void ShutdownImGui() override;
    void ShutdownBackend() override;

private:
    void* m_screen = nullptr;
};

} // namespace ambidb
