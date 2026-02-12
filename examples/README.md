# UI Parity Showcase

`example_ui_showcase` is a single comprehensive example that uses the same UI code path for both backends.

- One render flow in `examples/ui_showcase_example.cxx`
- Backend-specific runtime/bootstrap only in `examples/main_ui_showcase.cxx`
- Backend differences modeled through `ambidb::ui::kMetrics`, `ambidb::ui::kCaps`, and `ambidb::ui` helpers
- Full reusable component pack demoed: hints, selection, dialogs, filtering, tables, and form wrappers

## Build and run

GUI:

```bash
cmake -S . -B build-gui -DAMBIDB_BACKEND=GUI
cmake --build build-gui --target example_ui_showcase
./build-gui/examples/example_ui_showcase
```

TUI:

```bash
cmake -S . -B build-tui -DAMBIDB_BACKEND=TUI
cmake --build build-tui --target example_ui_showcase
./build-tui/examples/example_ui_showcase
```
