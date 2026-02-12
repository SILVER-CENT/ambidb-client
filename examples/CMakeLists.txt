add_executable(example_ui_showcase
    main_ui_showcase.cxx
    ui_showcase_example.cxx
)

target_include_directories(example_ui_showcase PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
)

if(AMBIDB_BACKEND STREQUAL "GUI")
    target_sources(example_ui_showcase PRIVATE ${CMAKE_SOURCE_DIR}/src/backends/gui/backend.cxx)
elseif(AMBIDB_BACKEND STREQUAL "TUI")
    target_sources(example_ui_showcase PRIVATE ${CMAKE_SOURCE_DIR}/src/backends/tui/backend.cxx)
endif()

target_link_libraries(example_ui_showcase PRIVATE ambidb_app)
