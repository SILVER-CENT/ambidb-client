# Backend Architecture Documentation

## Overview

This document describes the interface between the TUI and GUI backends in the AmbiDB client, and explains how the shared application logic interacts with both rendering backends.

## Architecture Principles

The AmbiDB client follows a **Backend Abstraction Pattern** with **C++23 compile-time polymorphism** where:

1. **Application Logic is Backend-Agnostic**: The `App` class contains all business logic and UI definitions using ImGui API primitives
2. **Backends Handle Rendering Only**: GUI and TUI backends are responsible solely for:
   - Initializing their respective rendering systems
   - Managing the render loop
   - Translating ImGui draw commands to their rendering target (OpenGL or ncurses)
3. **Compile-Time Backend Selection**: The backend is selected at CMake configuration time, not runtime
4. **Zero Runtime Overhead**: Uses CRTP (Curiously Recurring Template Pattern) and C++23 concepts instead of virtual functions

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
│                      │      │                     │
│ BackendBase<         │      │ BackendBase<        │
│   GuiBackend>        │      │   TuiBackend>       │
└───────────┬──────────┘      └──────────┬──────────┘
            │                             │
            └──────────────┬──────────────┘
                           │
                   ┌───────▼────────┐
                   │ Backend Concept│
                   │ (Compile-time) │
                   │  Validation    │
                   └───────┬────────┘
                           │
                   ┌───────▼────────┐
                   │      App       │
                   │  (ImGui API)   │
                   └────────────────┘
```

## Class Hierarchy

### Backend Concept (C++23)

**File**: `backend_concept.h`

Defines the compile-time requirements for backends using C++23 concepts:

```cpp
template<typename T>
concept Backend = requires(T& backend, const T& const_backend) {
    { backend.InitializeBackend() } -> std::same_as<bool>;
    { backend.InitializeImGui() } -> std::same_as<bool>;
    { backend.Run() } -> std::same_as<void>;
    { backend.ShutdownImGui() } -> std::same_as<void>;
    { backend.ShutdownBackend() } -> std::same_as<void>;
    { const_backend.GetName() } -> std::convertible_to<const char*>;
};
```

**Benefits**:
- Compile-time validation of backend interface
- Clear compiler error messages when interface is not satisfied
- No runtime overhead (no vtables, no virtual function calls)
- Better optimization opportunities for compilers

### BackendBase (CRTP Template Base Class)

**File**: `backend_base.h`

Provides common functionality using CRTP (Curiously Recurring Template Pattern):

```cpp
template<typename Derived>
class BackendBase {
public:
    bool Initialize() {
        static_assert(Backend<Derived>, "Must satisfy Backend concept");
        // Calls derived().InitializeBackend() and derived().InitializeImGui()
    }
    // ...
};
```

**Key Features**:
- **Manages App lifecycle**: Creates and owns the `App` instance
- **Orchestrates initialization**: Calls backend-specific hooks in the correct order
- **Provides logging**: Adds consistent logging for initialization and shutdown
- **Error handling**: Ensures proper error propagation
- **Zero overhead**: All calls resolved at compile-time via CRTP
- **Type safety**: Static assertions verify derived class satisfies Backend concept

Derived classes must implement these public methods:
- `InitializeBackend()`: Backend-specific setup (window, screen)
- `InitializeImGui()`: ImGui context and rendering backend setup
- `Run()`: Main event loop
- `ShutdownImGui()`: ImGui cleanup
- `ShutdownBackend()`: Backend-specific cleanup
- `GetName() const`: Return backend name

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

1. **Eliminated Code Duplication**: Common patterns extracted to `BackendBase` CRTP template
2. **Zero Runtime Overhead**: Replaced virtual functions with compile-time CRTP pattern
3. **C++23 Concepts**: Type-safe compile-time validation of backend interface
4. **Improved Error Handling**: All initialization failures are logged with context
5. **Configuration Management**: Magic numbers moved to `backend_config.h`
6. **Better Resource Management**: Added null checks and proper cleanup order
7. **Documentation**: Inline comments explain non-obvious logic (e.g., TUI polling)
8. **Interface Clarity**: Backend concept makes the contract explicit

## Adding a New Backend

To add a new backend (e.g., for WebAssembly):

1. Create `new_backend.h` and `new_backend.cpp`
2. Inherit from `BackendBase<NewBackend>` using CRTP:
   ```cpp
   class NewBackend : public BackendBase<NewBackend> {
   public:
       // Implement required methods
       bool InitializeBackend();
       bool InitializeImGui();
       void Run();
       void ShutdownImGui();
       void ShutdownBackend();
       const char* GetName() const { return "WebAssembly"; }
   };
   ```
3. The compiler will validate your implementation satisfies the Backend concept via `static_assert`
4. Implement the required methods with backend-specific logic
5. Add CMake configuration for your backend
6. Update `main.cpp` to include your backend header with appropriate `#ifdef`

The `App` class requires no changes - it remains fully backend-agnostic. The Backend concept ensures compile-time type safety without any runtime overhead.

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
