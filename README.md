# AmbiDB Client (C++)

> **One Codebase. Two Interfaces.**
> A lightweight, high-performance database client that runs as a modern GUI on your desktop and a mouse-driven TUI on your servers.

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![C++](https://img.shields.io/badge/std-c%2B%2B23-blue)
![Build Status](https://img.shields.io/badge/build-passing-brightgreen)

## 📖 Overview

**AmbiDB** is a database management tool built on the **Dear ImGui** ecosystem. It solves the "GUI vs. CLI" dilemma by separating the application logic from the rendering layer.

* **Local Machine?** Build with GUI support to get a hardware-accelerated OpenGL window with smooth fonts and high-res plotting.
* **Headless Server (SSH)?** Build with TUI support to get the exact same interface rendered in ASCII via `ncurses`.

## ✨ Features

* **Dual Rendering:** Seamless switching between GLFW/OpenGL (Desktop) and Ncurses (Terminal) at compile-time.
* **Mouse Support:** Full mouse interaction in Terminal mode (Click, Drag, Resize windows).
* **Unified Logic:** Filters, sorting, and query execution code is shared 100% between modes.
* **Lightweight:** Written in C++23 for minimal memory footprint (ideal for micro-instances).
* **Universal Shortcuts:** `Ctrl+S` saves your query, whether you are in VS Code or Putty.

## 🛠️ Architecture

The project uses a "Backend Swap" architecture:

| Component | GUI Mode | TUI Mode |
| :--- | :--- | :--- |
| **Logic** | C++ Standard Library | C++ Standard Library |
| **UI Definition** | Dear ImGui | Dear ImGui |
| **Windowing** | GLFW | Ncurses |
| **Rendering** | OpenGL 3 | ImTui (Text Rasterizer) |

## 📦 Prerequisites

Ensure you have the following installed on your system:

* **C++ Compiler** (GCC 11+ / Clang 13+ / MSVC 2022+)
* **CMake** (3.14+)
* **Git**
* **Libraries:**
    * `libglfw3-dev` (For GUI mode)
    * `libncurses-dev` (For TUI mode)

## 🚀 Build Instructions

```bash
# 1. Clone the repository
git clone https://github.com/your-username/ambidb-client.git
cd ambidb-client

# 2. Create build directory
mkdir build && cd build

# 3. Configure via CMake
# For GUI mode:
cmake -DAMBIDB_BACKEND=GUI ..
# For TUI mode:
cmake -DAMBIDB_BACKEND=TUI ..

# 4. Compile
make -j4
```

## 🖥️ Usage

The application produces a single binary: `ambidb`.

### Running in GUI Mode
Ideal for local development on Windows/Mac/Linux desktops.
```bash
./ambidb
```

### Running in TUI Mode
Ideal for SSH sessions or servers without a display server (X11/Wayland).
```bash
./ambidb
```

## 📂 Project Structure
├── src/
│   ├── main.cpp          # Entry point & Backend selection
│   ├── app.cpp           # The Core DB Client Logic (Shared ImGui code)
│   ├── app.h             # Shared state definitions
│   └── backends/         # Backend implementations (GUI/TUI)
├── tests/                # Unit tests (GTest)
├── CMakeLists.txt        # Build configuration
└── README.md

## 🤝 Contributing
1. Fork the repository.
2. Create your feature branch (`git checkout -b feature/amazing-feature`).
3. Commit your changes (`git commit -m 'Add some amazing feature'`).
4. Push to the branch (`git push origin feature/amazing-feature`).
5. Open a Pull Request.

## 📄 License
Distributed under the MIT License. See LICENSE for more information.
