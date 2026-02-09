#pragma once

namespace ambidb {
namespace config {

// Window configuration for GUI backend
constexpr int DEFAULT_WINDOW_WIDTH = 1280;
constexpr int DEFAULT_WINDOW_HEIGHT = 720;
constexpr const char* WINDOW_TITLE = "AmbiDB Client";

// OpenGL configuration
constexpr const char* GLSL_VERSION = "#version 130";
constexpr int OPENGL_MAJOR_VERSION = 3;
constexpr int OPENGL_MINOR_VERSION = 0;

// Visual configuration
constexpr float CLEAR_COLOR_R = 0.45f;
constexpr float CLEAR_COLOR_G = 0.55f;
constexpr float CLEAR_COLOR_B = 0.60f;
constexpr float CLEAR_COLOR_A = 1.00f;

} // namespace config
} // namespace ambidb
