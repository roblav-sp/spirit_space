# Phase 1 Engine — Sprint Integration Report
Generated: 2026-04-19

## `make` result: ✅ PASSES — all 9 binaries produced

---

## Sprint Status

| Sprint | Name | Status | Test Binary |
|---|---|---|---|
| EP-ENG-01-S01 | Build System | ✅ DONE (QA signed off) | `spirit_space_tests.exe` |
| EP-ENG-01-S02 | Window & GL Context | ✅ DONE (QA signed off) | `engine_tests.exe` |
| EP-ENG-02-S01 | ECS Layer | ✅ BUILT — awaiting QA | `ecs_tests.exe` |
| EP-ENG-02-S02 | Config System | ✅ BUILT — awaiting QA | `config_tests.exe` |
| EP-ENG-03-S01 | Shader System & DSA | ✅ BUILT — awaiting QA | `render_tests.exe` |
| EP-ENG-04-S01 | HUD Framework | ✅ BUILT — awaiting QA | `hud_tests.exe` |
| EP-ENG-04-S02 | Audio Backend | ✅ DONE (QA signed off) | `audio_tests.exe` |
| EP-ENG-05-S01 | Game State Machine | ✅ BUILT — awaiting QA | `statemachine_tests.exe` |

---

## New Files Created

### engine/include/engine/
- `ecs/World.h` — EnTT registry wrapper
- `core/Config.h` — JSON config singleton (dot-notation, graceful defaults)
- `core/IGameState.h` — pure interface (onEnter/onExit/onUpdate/onPause/onResume)
- `core/GameStateMachine.h` — push/pop/replace state stack
- `render/Shader.h` — GLSL file loader, bind/unbind, setUniform
- `render/GpuBuffer.h` — DSA GpuBuffer + VertexArray wrappers
- `hud/HudSystem.h` — ImGui docking panel registry

### engine/src/
- `ecs/World.cpp`
- `core/Config.cpp`
- `core/GameStateMachine.cpp`
- `render/Shader.cpp`
- `render/GpuBuffer.cpp`
- `hud/HudSystem.cpp`

### src/shaders/
- `test_quad.vert` — passthrough vertex shader
- `test_quad.frag` — uColor fragment shader

### tests/
- `ecs/test_entity_lifecycle.cpp` (3 tests)
- `ecs/test_component_add_remove.cpp` (4 tests)
- `ecs/test_view_query.cpp` (4 tests)
- `core/test_config_read.cpp` (4 tests)
- `core/test_config_missing_key_default.cpp` (3 tests)
- `core/test_config_write_persist.cpp` (3 tests)
- `render/test_shader_compile.cpp`
- `render/test_shader_missing_file_error.cpp`
- `render/test_dsa_buffer_upload.cpp`
- `hud/test_imgui_init.cpp` (2 tests)
- `hud/test_panel_lifecycle.cpp` (4 tests)
- `core/test_state_push_pop.cpp` (5 tests)
- `core/test_state_lifecycle_hooks.cpp` (7 tests)
- `core/test_empty_stack_safe.cpp` (5 tests)

---

## Notable Implementation Findings

| Sprint | Finding |
|---|---|
| EP-ENG-02-S01 | `entt::registry::alive()` removed in EnTT 3.x — `entityCount()` uses `storage<entt::entity>()->free_list()` |
| EP-ENG-04-S01 | `DockSpaceOverViewport` in ImGui 1.92.7 takes `(ImGuiID, viewport*, ...)` not single-arg form |
| EP-ENG-03-S01 | `EXPECT_NO_FATAL_FAILURE` with multi-statement blocks unreliable on MinGW GTest — replaced with scope + `SUCCEED()` |

---

## Cross-Sprint Dependencies

- `Config` is standalone — no other engine dependency
- `GameStateMachine` is standalone — no other engine dependency
- `World` (ECS) is standalone — EnTT header-only
- `Shader` + `GpuBuffer` depend on GLAD/GLFW (already in engine)
- `HudSystem` depends on GLAD/GLFW + ImGui

---

## Recommended QA Review Order

1. **`statemachine_tests.exe`** — pure logic, no GPU, runs anywhere
2. **`ecs_tests.exe`** — pure logic, no GPU, runs anywhere
3. **`config_tests.exe`** — filesystem only, no GPU, runs anywhere
4. **`render_tests.exe`** — needs GPU (GTEST_SKIP on no context)
5. **`hud_tests.exe`** — needs GPU (GTEST_SKIP on no context)

All GL-dependent tests guard with `GTEST_SKIP()` when run on machines without OpenGL 4.5 drivers.

---

## Remaining Phase 1 Sprints (not yet started)

| Sprint | Name |
|---|---|
| EP-ENG-02-S03 | Input System |
| EP-ENG-02-S04 | Logging |
| EP-ENG-03-S02 | Mesh Loading & Asset Loader |
| EP-ENG-03-S03 | Camera System |
| EP-ENG-05-S02 | Save/Load Framework |
| EP-ENG-05-S03 | Time System |
