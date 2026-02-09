cmake_minimum_required(VERSION 3.20)

# Check if ccache is installed on the system
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    message(STATUS "Using ccache: ${CCACHE_PROGRAM}")
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
    set(CMAKE_C_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
else()
    message(STATUS "ccache not found - compilation will be slower")
endif()

project(AmbiDB-client
    VERSION 1.0.0
    DESCRIPTION "AmbiDB client"
    LANGUAGES CXX
)

include(FetchContent)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build (Debug/Release)" FORCE)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
    -Wformat=2
    -Wformat-security
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- Backend selection ---
set(AMBIDB_BACKEND "GUI" CACHE STRING "Backend to use (GUI or TUI)")
message(STATUS "Building with backend: ${AMBIDB_BACKEND}")

# --- GoogleTest (for tests subdirectory) ---
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# --- Application library (used by executable and tests) ---
add_library(ambidb_app STATIC
    src/app.cxx
    src/app.h
)
target_include_directories(ambidb_app PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# --- GUI backend ---
if(AMBIDB_BACKEND STREQUAL "GUI")
    find_package(OpenGL REQUIRED)

    # GLFW (FetchContent). Prefer Wayland on Linux to avoid X11 dev packages (e.g. libXrandr-devel).
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set(GLFW_BUILD_X11 OFF CACHE BOOL "" FORCE)
        set(GLFW_BUILD_WAYLAND ON CACHE BOOL "" FORCE)
    endif()
    FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
    )
    FetchContent_MakeAvailable(glfw)

    FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.91.1
    )
    FetchContent_MakeAvailable(imgui)

    set(IMGUI_SOURCES
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    )
    add_library(imgui_gui STATIC ${IMGUI_SOURCES})
    target_include_directories(imgui_gui PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)
    target_link_libraries(imgui_gui PUBLIC glfw OpenGL::GL)

    target_compile_definitions(ambidb_app PUBLIC AMBIDB_GUI)
    target_include_directories(ambidb_app PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)
    target_link_libraries(ambidb_app PUBLIC imgui_gui)

    add_executable(ambidb
        src/main.cxx
        src/backends/gui/backend.cxx
        src/backends/gui/backend.h
    )
    target_include_directories(ambidb PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src> $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
    target_compile_definitions(ambidb PRIVATE AMBIDB_GUI)
    target_link_libraries(ambidb PRIVATE ambidb_app)

    # --- TUI backend ---
elseif(AMBIDB_BACKEND STREQUAL "TUI")
    find_package(Curses REQUIRED)

    FetchContent_Declare(
        imtui
        GIT_REPOSITORY https://github.com/ggerganov/imtui.git
        GIT_TAG master
    )
    FetchContent_MakeAvailable(imtui)

    target_compile_definitions(ambidb_app PUBLIC AMBIDB_TUI)
    target_include_directories(ambidb_app PUBLIC ${imtui_SOURCE_DIR}/third-party/imgui/imgui)
    target_link_libraries(ambidb_app PUBLIC imtui-ncurses)

    add_executable(ambidb
        src/main.cxx
        src/backends/tui/backend.cxx
        src/backends/tui/backend.h
    )
    target_include_directories(ambidb PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${imtui_SOURCE_DIR}/third-party/imgui/imgui>
    )
    target_compile_definitions(ambidb PRIVATE AMBIDB_TUI)
    target_link_libraries(ambidb PRIVATE ambidb_app)
else()
    message(FATAL_ERROR "AMBIDB_BACKEND must be GUI or TUI, got: ${AMBIDB_BACKEND}")
endif()

# --- Tests (link to library defined above) ---
add_subdirectory(tests)

message(STATUS "Configuration complete.")
