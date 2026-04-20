# EP-ENG-04-S01 — HUD Framework: Done

## Files Created

| File | Description |
|---|---|
| `engine/include/engine/hud/HudSystem.h` | HudSystem class declaration (non-copyable, non-movable; GLFWwindow forward-declared) |
| `engine/src/hud/HudSystem.cpp` | Full implementation: ImGui context lifecycle, docking, panel registry, frame loop |
| `tests/hud/test_imgui_init.cpp` | HudInit.CreatesContext, HudInit.DestructorCleans |
| `tests/hud/test_panel_lifecycle.cpp` | RegisterPanel, UnregisterPanel, RenderPanelCalled, DuplicateRegisterOverwrites |

## Tests Written

- `HudInit.CreatesContext` — construct HudSystem, assert isReady() == true
- `HudInit.DestructorCleans` — HudSystem goes out of scope without crash
- `PanelLifecycle.RegisterPanel` — hasPanel/panelCount after register
- `PanelLifecycle.UnregisterPanel` — hasPanel/panelCount after unregister
- `PanelLifecycle.RenderPanelCalled` — beginFrame/renderPanels/endFrame invokes the callback
- `PanelLifecycle.DuplicateRegisterOverwrites` — second registration of same name keeps panelCount == 1

All tests call GTEST_SKIP() when window.isContextValid() == false (CI-safe).

## Compilation Status

All three `.cpp` files compiled successfully with no warnings under:
```
x86_64-w64-mingw32-g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror
```

## Issues / Deviations

- `ImGui::DockSpaceOverViewport` signature in the installed vcpkg version is
  `DockSpaceOverViewport(ImGuiID, const ImGuiViewport*, ...)` — the sprint spec
  showed an older one-argument form. Fixed by passing `0` (auto-ID) as first
  argument and `ImGui::GetMainViewport()` as second.
