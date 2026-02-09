#pragma once

#include "backend_base.h"

namespace ambidb {

/**
 * @brief TUI backend implementation using ncurses and ImTui.
 *
 * This backend provides a terminal-based interface suitable for
 * SSH sessions and headless servers without display servers.
 *
 * Uses CRTP pattern via BackendBase<TuiBackend> for compile-time polymorphism.
 */
class TuiBackend : public BackendBase<TuiBackend> {
public:
    TuiBackend() = default;
    ~TuiBackend() = default;

    // Backend interface implementation
    void Run();
    const char* GetName() const { return "TUI"; }
    bool InitializeBackend();
    bool InitializeImGui();
    void ShutdownImGui();
    void ShutdownBackend();

private:
    void* m_screen = nullptr;
};

} // namespace ambidb
