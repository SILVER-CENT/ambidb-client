# Backend Architecture Documentation

## Overview

This document describes the interface between the TUI and GUI backends in the AmbiDB client, and explains how the shared application logic interacts with both rendering backends.

## Architecture Principles

The AmbiDB client follows a **Backend Abstraction Pattern** where:

1. **Application Logic is Backend-Agnostic**: The `App` class contains all business logic and UI definitions using ImGui API primitives
2. **Backends Handle Rendering Only**: GUI and TUI backends are responsible solely for:
   - Initializing their respective rendering systems
   - Managing the render loop
   - Translating ImGui draw commands to their rendering target (OpenGL or ncurses)
3. **Compile-Time Backend Selection**: The backend is selected at CMake configuration time, not runtime

## Component Diagram

```
┌─────────────────────────────────────────────────────────────┐
│                          main.cpp                           │
│                  (Backend selection via #ifdef)             │
└──────────────────────────┬──────────────────────────────────┘
                           │
            ┌──────────────┴──────────────┐
            │                             │
┌───────────▼──────────┐      ┌──────────▼──────────┐
│    GuiBackend        │      │    TuiBackend       │
│  (GLFW + OpenGL)     │      │  (ncurses + ImTui)  │
└───────────┬──────────┘      └──────────┬──────────┘
            │                             │
            └──────────────┬──────────────┘
                           │
                   ┌───────▼────────┐
                   │  BackendBase   │
                   │  (Template     │
                   │   Method)      │
                   └───────┬────────┘
                           │
                   ┌───────▼────────┐
                   │   IBackend     │
                   │  (Interface)   │
                   └───────┬────────┘
                           │
                   ┌───────▼────────┐
                   │      App       │
                   │  (ImGui API)   │
                   └────────────────┘
```

## Class Hierarchy

### IBackend (Interface)

**File**: `backend_interface.h`

Defines the contract that all backends must implement:

```cpp
class IBackend {
public:
    virtual bool Initialize() = 0;  // Setup backend and ImGui
    virtual void Run() = 0;          // Main render loop
    virtual void Shutdown() = 0;     // Cleanup resources
    virtual const char* GetName() const = 0;  // Backend identification
};
```

### BackendBase (Abstract Base Class)

**File**: `backend_base.h`

Provides common functionality using the Template Method pattern:

- **Manages App lifecycle**: Creates and owns the `App` instance
- **Orchestrates initialization**: Calls backend-specific hooks in the correct order
- **Provides logging**: Adds consistent logging for initialization and shutdown
- **Error handling**: Ensures proper error propagation

Derived classes implement these protected methods:
- `InitializeBackend()`: Backend-specific setup (window, screen)
- `InitializeImGui()`: ImGui context and rendering backend setup
- `ShutdownImGui()`: ImGui cleanup
- `ShutdownBackend()`: Backend-specific cleanup

### GuiBackend

**File**: `gui_backend.h`, `gui_backend.cpp`

Implements the GUI backend using:
- **GLFW**: Window management and input handling
- **OpenGL 3.0**: Hardware-accelerated rendering
- **ImGui OpenGL3 backend**: Translates ImGui draw commands to OpenGL calls

**Initialization Flow**:
1. Initialize GLFW and create window
2. Create ImGui context
3. Initialize ImGui GLFW and OpenGL3 backends

**Render Loop**:
```
while (window open && !app.ShouldClose()):
    Poll events (GLFW)
    Begin frame (ImGui)
    app.Update()  // Application logic
    Render (OpenGL)
    Swap buffers
```

### TuiBackend

**File**: `tui_backend.h`, `tui_backend.cpp`

Implements the TUI backend using:
- **ncurses**: Terminal manipulation
- **ImTui**: Text-mode rendering of ImGui draw commands

**Initialization Flow**:
1. Create ImGui context
2. Initialize ImTui ncurses backend
3. Initialize ImTui text renderer

**Render Loop**:
```
while (!app.ShouldClose()):
    Wait for input (poll)
    Begin frame (ImGui)
    app.Update()  // Application logic
    Render to text buffer (ImTui)
    Draw to screen (ncurses)
```

**Note**: TUI uses `poll()` to wait for input instead of continuously polling, which reduces CPU usage in terminal mode.

## App Class

**File**: `app.h`, `app.cpp`

The `App` class is completely backend-agnostic and uses only the Dear ImGui API:

```cpp
void App::Update() {
    ImGui::Begin("AmbiDB Client");

    ImGui::Text("Welcome to AmbiDB!");
    // ... UI code using ImGui primitives ...

    if (ImGui::Button("Quit")) {
        m_shouldClose = true;
    }

    ImGui::End();
}
```

**Key Points**:
- Uses only Dear ImGui API calls (`ImGui::*`)
- Never references backend-specific code (no GLFW, OpenGL, ncurses)
- Maintains application state independent of rendering
- Signals when the application should close via `ShouldClose()`

## Configuration

**File**: `backend_config.h`

Centralizes configuration constants:
- Window dimensions (GUI only)
- OpenGL version requirements
- Clear colors and visual settings

Benefits:
- Single source of truth for configuration
- Easy to adjust without touching implementation code
- Type-safe compile-time constants

## Data Flow

### Initialization Phase

```
main()
  → Backend::Initialize()
    → BackendBase::Initialize()
      → InitializeBackend()    // Backend-specific
      → InitializeImGui()      // Backend-specific
        → Creates App instance
```

### Runtime Phase

```
Backend::Run()
  → Backend event polling
  → ImGui::NewFrame()
  → App::Update()             // Backend-agnostic
    → ImGui::Begin/End/Button/Text...
  → ImGui::Render()
  → Backend renders draw data
```

### Shutdown Phase

```
Backend::Shutdown()
  → BackendBase::Shutdown()
    → ShutdownImGui()         // Backend-specific
    → ShutdownBackend()       // Backend-specific
      → Destroys App instance
```

## Optimization Strategies Applied

1. **Eliminated Code Duplication**: Common patterns extracted to `BackendBase`
2. **Improved Error Handling**: All initialization failures are logged with context
3. **Configuration Management**: Magic numbers moved to `backend_config.h`
4. **Better Resource Management**: Added null checks and proper cleanup order
5. **Documentation**: Inline comments explain non-obvious logic (e.g., TUI polling)
6. **Interface Clarity**: `IBackend` makes the contract explicit

## Adding a New Backend

To add a new backend (e.g., for WebAssembly):

1. Create `new_backend.h` and `new_backend.cpp`
2. Inherit from `BackendBase`
3. Implement the four protected methods:
   - `InitializeBackend()`
   - `InitializeImGui()`
   - `ShutdownImGui()`
   - `ShutdownBackend()`
4. Implement `Run()` with your render loop
5. Implement `GetName()` to return a descriptive name
6. Add CMake configuration for your backend
7. Update `main.cpp` to include your backend header

The `App` class requires no changes - it remains fully backend-agnostic.

## Testing Strategy

Each backend can be tested independently:

1. **Unit Tests**: Test `App` class in isolation (already in `test_app.cpp`)
2. **Integration Tests**: Test each backend's initialization/shutdown cycle
3. **Manual Tests**: Run the application in both GUI and TUI modes

Current test setup uses GoogleTest and can instantiate `App` objects without a full backend.

## Performance Considerations

### GUI Backend
- **VSync enabled**: Limits frame rate to monitor refresh rate (60 Hz typically)
- **Hardware accelerated**: Uses GPU for rendering
- **Continuous polling**: Always responsive to input

### TUI Backend
- **Event-driven**: Uses `poll()` to wait for input, reducing CPU usage
- **Software rendering**: ImTui rasterizes to ASCII characters
- **First frame optimization**: Skips poll on first frame for immediate display

## Future Enhancements

Possible future optimizations:

1. **Dynamic Backend Switching**: Allow runtime selection (requires linking both backends)
2. **Backend Plugins**: Load backends as shared libraries
3. **Configuration Files**: Replace compile-time config with runtime JSON/TOML
4. **Remote Backend**: Network-based rendering for web interfaces
5. **Performance Metrics**: Add instrumentation to track frame times
6. **Resource Pooling**: Reuse ImGui contexts across backend switches
