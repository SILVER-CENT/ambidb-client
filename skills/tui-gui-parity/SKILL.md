---
name: tui-gui-parity
description: Keeps one shared ImGui UI path for GUI (GLFW/OpenGL) and TUI (ImTui/ncurses) in ambidb-client. Use when editing src/app.h, src/app.cxx, or UI code to avoid duplicated render flows and limit backend differences to metrics, style, and small presentation wrappers.
---

# TUI–GUI Parity

Use this skill whenever updating app-level UI in ambidb-client.

## Workflow

1. Model backend differences as data (metrics and capabilities), not separate render trees.
2. Keep one `RenderSidebar`, one `RenderContent`, and one navigation/state path.
3. Prefer portable ImGui widgets first; wrap non-portable visuals behind helper functions.
4. Keep backend checks local to helpers and style setup only.
5. Build both GUI and TUI after each UI change.

## Reference

For architecture rules, portable widget set, and fallback patterns, see [references/rulebook.md](references/rulebook.md). Read it before implementing non-trivial UI changes.

## Verification

Run both builds after UI changes:

```bash
cmake -S . -B build -DAMBIDB_BACKEND=GUI && cmake --build build -j10
cmake -S . -B build -DAMBIDB_BACKEND=TUI && cmake --build build -j10
```

If backend-specific branching remains, keep it minimal and add a one-line comment at the branch explaining why.
