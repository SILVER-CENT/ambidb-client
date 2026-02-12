cmake_minimum_required(VERSION 3.20)

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

# Third-party dependencies (download via scripts/fetch-deps.sh)
set(THIRD_PARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party")

if(NOT EXISTS "${THIRD_PARTY_DIR}/googletest")
    message(FATAL_ERROR
        "Missing third_party/googletest. Run: ./scripts/fetch-deps.sh")
endif()

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

option(AMBIDB_BUILD_EXAMPLES "Build UI component examples" ON)

set(AMBIDB_BACKEND "GUI" CACHE STRING "Backend to use (GUI or TUI)")
message(STATUS "Building with backend: ${AMBIDB_BACKEND}")

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(${THIRD_PARTY_DIR}/googletest)

add_library(ambidb_app STATIC
    src/app.cxx
    src/app.h
    src/ui/dialogs.cxx
    src/ui/filter.cxx
    src/ui/forms.cxx
    src/ui/hints.cxx
    src/ui/layout.cxx
    src/ui/selection.cxx
    src/ui/tables.cxx
    src/ui/color_utils.cxx
    src/ui/theme.cxx
    src/ui/widgets.cxx
)
target_include_directories(ambidb_app PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

if(AMBIDB_BACKEND STREQUAL "GUI")
    find_package(OpenGL REQUIRED)

    if(NOT EXISTS "${THIRD_PARTY_DIR}/glfw" OR NOT EXISTS "${THIRD_PARTY_DIR}/imgui")
        message(FATAL_ERROR
            "Missing third_party/glfw or third_party/imgui for GUI backend. Run: ./scripts/fetch-deps.sh")
    endif()
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set(GLFW_BUILD_X11 OFF CACHE BOOL "" FORCE)
        set(GLFW_BUILD_WAYLAND ON CACHE BOOL "" FORCE)
    endif()
    add_subdirectory(${THIRD_PARTY_DIR}/glfw)

    set(IMGUI_DIR "${THIRD_PARTY_DIR}/imgui")
    set(IMGUI_SOURCES
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
    )
    add_library(imgui_gui STATIC ${IMGUI_SOURCES})
    target_include_directories(imgui_gui PUBLIC ${IMGUI_DIR} ${IMGUI_DIR}/backends)
    target_link_libraries(imgui_gui PUBLIC glfw OpenGL::GL)

    target_compile_definitions(ambidb_app PUBLIC AMBIDB_GUI)
    target_include_directories(ambidb_app PUBLIC ${IMGUI_DIR} ${IMGUI_DIR}/backends)
    target_link_libraries(ambidb_app PUBLIC imgui_gui)

    add_executable(ambidb
        src/main.cxx
        src/backends/gui/backend.cxx
        src/backends/gui/backend.h
    )
    target_include_directories(ambidb PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src> $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
    target_compile_definitions(ambidb PRIVATE AMBIDB_GUI)
    target_link_libraries(ambidb PRIVATE ambidb_app)

elseif(AMBIDB_BACKEND STREQUAL "TUI")
    find_package(Curses REQUIRED)

    if(NOT EXISTS "${THIRD_PARTY_DIR}/imtui")
        message(FATAL_ERROR
            "Missing third_party/imtui for TUI backend. Run: ./scripts/fetch-deps.sh")
    endif()
    add_subdirectory(${THIRD_PARTY_DIR}/imtui)

    set(IMTUI_DIR "${THIRD_PARTY_DIR}/imtui")
    target_compile_definitions(ambidb_app PUBLIC AMBIDB_TUI)
    target_include_directories(ambidb_app PUBLIC ${IMTUI_DIR}/third-party/imgui/imgui)
    target_link_libraries(ambidb_app PUBLIC imtui-ncurses)

    add_executable(ambidb
        src/main.cxx
        src/backends/tui/backend.cxx
        src/backends/tui/backend.h
    )
    target_include_directories(ambidb PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${IMTUI_DIR}/third-party/imgui/imgui>
    )
    target_compile_definitions(ambidb PRIVATE AMBIDB_TUI)
    target_link_libraries(ambidb PRIVATE ambidb_app)
else()
    message(FATAL_ERROR "AMBIDB_BACKEND must be GUI or TUI, got: ${AMBIDB_BACKEND}")
endif()

add_subdirectory(tests)

if(AMBIDB_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

message(STATUS "Configuration complete.")
