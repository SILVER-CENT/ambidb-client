# Rulebook: Minimal Rewrite Between TUI and GUI

## Core Rule

Keep business state and UI flow shared. Express backend differences through constants, style tokens, and tiny visual adapters.

## Non-Negotiable Rules

1. Keep one render path per screen.
2. Avoid full function forks like `if (is_tui) { ... return; }` in primary render functions.
3. Keep backend checks in only three places:
   - metrics selection
   - style setup
   - tiny visual fallback helpers
4. Keep navigation state shared across backends.
5. Keep data models shared across backends.
6. Treat TUI dimensions as character-cell units, not pixel units.

## Architecture Pattern

Use one shared config object selected at compile-time:

```cpp
struct UiMetrics {
    float sidebar_width;
    ImVec2 window_padding;
    ImVec2 frame_padding;
    ImVec2 item_spacing;
    float frame_rounding;
};

struct UiCaps {
    bool draw_lines;
    bool draw_filled_badges;
    bool draw_circle_status;
};
```

Select values once:

```cpp
#if defined(AMBIDB_TUI)
constexpr UiMetrics kUiMetrics{30.0f, {1.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 0.0f}, 0.0f};
constexpr UiCaps kUiCaps{false, false, false};
#else
constexpr UiMetrics kUiMetrics{190.0f, {8.0f, 8.0f}, {8.0f, 4.0f}, {8.0f, 6.0f}, 4.0f};
constexpr UiCaps kUiCaps{true, true, true};
#endif
```

Consume in shared rendering code. Do not branch entire layout functions.

## Portable-First Widget Set

Prefer these APIs for shared code:

- `ImGui::TextUnformatted`
- `ImGui::Button`
- `ImGui::Selectable`
- `ImGui::TreeNodeEx`
- `ImGui::Separator`
- `ImGui::BeginChild` and `ImGui::EndChild`
- `ImGui::SameLine`
- `ImGui::PushStyleColor` and `ImGui::PopStyleColor`

## Visual Fallback Mapping

Use wrappers for visuals that degrade in TUI:

- Status dot
  - GUI: draw circle or colored bullet
  - TUI: render `*` with color
- Badge background
  - GUI: filled rounded rect + text
  - TUI: plain `[type]` text
- Thin divider line
  - GUI: draw line
  - TUI: rely on `ImGui::Separator`

Implement wrappers once:

```cpp
void RenderStatusDot(bool ok);
void RenderTypeBadge(const std::string& type);
void RenderVerticalDivider();
```

Branch only inside these wrappers.

## Common Failure Modes

- Setting large pixel-tuned values in TUI (`190`, `8`, `24`) and causing oversized terminal layout.
- Using draw-list primitives directly in feature code instead of wrappers.
- Duplicating navigation logic inside backend-specific paths.
- Styling for one backend globally and breaking the other backend.

## PR Checklist

1. Keep render/state logic shared.
2. Keep backend branching limited to helpers and style selection.
3. Keep TUI values conservative in character cells.
4. Build GUI backend.
5. Build TUI backend.
6. Confirm both backends keep the same navigation flow and page state behavior.
