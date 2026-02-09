cmake_minimum_required(VERSION 3.20)
project(AmbiDB)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# --- Backend Selection ---
set(AMBIDB_BACKEND "GUI" CACHE STRING "Backend to use (GUI or TUI)")
message(STATUS "Building with backend: ${AMBIDB_BACKEND}")

# --- GoogleTest ---
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.2
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# --- Application Shared Sources ---
set(APP_SOURCES
    src/app.cpp
    src/app.h
)

# --- GUI Backend Configuration ---
if(AMBIDB_BACKEND STREQUAL "GUI")
    find_package(OpenGL REQUIRED)
    find_package(glfw3 REQUIRED)

    # ImGui for GUI
    FetchContent_Declare(
      imgui
      GIT_REPOSITORY https://github.com/ocornut/imgui.git
      GIT_TAG v1.91.1
    )
    FetchContent_GetProperties(imgui)
    if(NOT imgui_POPULATED)
      FetchContent_Populate(imgui)
    endif()

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

    set(BACKEND_SOURCES
        src/backends/gui_backend.cpp
        src/backends/gui_backend.h
    )

    add_executable(ambidb src/main.cpp ${APP_SOURCES} ${BACKEND_SOURCES})
    target_compile_definitions(ambidb PRIVATE AMBIDB_GUI)
    target_link_libraries(ambidb PRIVATE imgui_gui)

    # Tests for GUI mode
    add_executable(app_tests tests/test_app.cpp ${APP_SOURCES})
    target_link_libraries(app_tests PRIVATE GTest::gtest_main imgui_gui)

# --- TUI Backend Configuration ---
elseif(AMBIDB_BACKEND STREQUAL "TUI")
    find_package(Curses REQUIRED)

    FetchContent_Declare(
      imtui
      GIT_REPOSITORY https://github.com/ggerganov/imtui.git
      GIT_TAG master
    )
    # imtui's CMake handles its own dependencies including imgui
    FetchContent_MakeAvailable(imtui)

    set(BACKEND_SOURCES
        src/backends/tui_backend.cpp
        src/backends/tui_backend.h
    )

    add_executable(ambidb src/main.cpp ${APP_SOURCES} ${BACKEND_SOURCES})
    target_compile_definitions(ambidb PRIVATE AMBIDB_TUI)
    target_link_libraries(ambidb PRIVATE imtui-ncurses)
    # We need to find where imgui.h is in imtui
    target_include_directories(ambidb PRIVATE ${imtui_SOURCE_DIR}/third-party/imgui/imgui)

    # Tests for TUI mode
    add_executable(app_tests tests/test_app.cpp ${APP_SOURCES})
    target_link_libraries(app_tests PRIVATE GTest::gtest_main imtui-ncurses)
    target_include_directories(app_tests PRIVATE ${imtui_SOURCE_DIR}/third-party/imgui/imgui)
endif()

enable_testing()
add_test(NAME AppTests COMMAND app_tests)
