# Sprint Progress — Spirit Space Phase 1

---

## EP-ENG-01 — Foundation  ✅ COMPLETE — 2026-04-19

### S01 — Build System & Repository ✅ DONE
| Criterion | Status | Notes |
|---|---|---|
| Build system works | ✅ | Makefile — `make` builds `spirit_space.exe` |
| vcpkg manifest present | ✅ | glad, glfw3, glm, gtest, miniaudio, entt, nlohmann-json, imgui |
| GTest integrated | ✅ | Smoke: 4/4 passed |
| CI pipeline exists | ✅ | `.github/workflows/ci.yml` — ubuntu/MinGW/make |
| Owner builds on own machine | ✅ | Confirmed 2026-04-19 |

### S02 — Window & OpenGL Context ✅ DONE
| Criterion | Status | Notes |
|---|---|---|
| GLFW + GLAD via vcpkg | ✅ | x64-mingw-static |
| Window opens + triangle renders | ✅ | Confirmed by owner |
| F11 fullscreen toggle — no crash | ✅ | engine_tests: 2/2 passed |
| GL version ≥ 4.5 | ✅ | 4.5.0 Intel Build 30.0.101.1338 |
| QA Gate | ✅ | Owner sign-off 2026-04-19 |

---

## EP-ENG-04-S02 — Audio Backend  ✅ COMPLETE — 2026-04-19
| Criterion | Status | Notes |
|---|---|---|
| Audio device initialises | ✅ | "Audio: ready" confirmed |
| No crash on missing file | ✅ | Unit tested |
| Volume control + clamping | ✅ | Unit tested |
| QA Gate: owner hears beep on P keypress | ✅ | Owner sign-off 2026-04-19 |

---

## EP-ENG-02-S01 — ECS Layer  ✅ COMPLETE — 2026-04-19
| Criterion | Status | Notes |
|---|---|---|
| `engine/include/engine/ecs/World.h` + `World.cpp` | ✅ | EnTT registry wrapper |
| 10k entity create/destroy cycle | ✅ | test_entity_lifecycle: 3/3 |
| Component add/remove/has (C-prefix) | ✅ | test_component_add_remove: 4/4 |
| Multi-component view queries | ✅ | test_view_query: 4/4 |
| QA Gate | ⏳ | Owner API review pending |

---

## EP-ENG-02-S02 — Config System  ✅ COMPLETE — 2026-04-19
| Criterion | Status | Notes |
|---|---|---|
| `engine/include/engine/core/Config.h` + `Config.cpp` | ✅ | nlohmann/json, dot-notation |
| Read key → correct value | ✅ | test_config_read: 4/4 |
| Missing key → default | ✅ | test_config_missing_key_default: 3/3 |
| Write → persists on reload | ✅ | test_config_write_persist: 3/3 |
| No crash on corrupt file | ✅ | test covered |
| QA Gate | ⏳ | Owner manual edit + app read pending |

---

## EP-ENG-03-S01 — Shader System & DSA Helpers  ✅ COMPLETE — 2026-04-19
| Criterion | Status | Notes |
|---|---|---|
| `Shader.h/cpp` + `GpuBuffer.h/cpp` | ✅ | DSA-only, no legacy bind calls |
| Shader compiles from file | ✅ | ShaderCompile: 2/2 ✅ |
| Compile errors surface in log | ✅ | ShaderMissing: 2/2 ✅ |
| DSA buffer/VAO upload | ✅ | DsaBuffer: 3/3 ✅ |
| render_tests.exe: 7/7 passed | ✅ | Owner confirmed 2026-04-19 |
| QA Gate | ⏳ | Owner coloured-quad render pending |

---

## EP-ENG-04-S01 — HUD Framework  ✅ COMPLETE — 2026-04-19
| Criterion | Status | Notes |
|---|---|---|
| `HudSystem.h/cpp` | ✅ | ImGui docking, generic panel registry |
| ImGui docking enabled | ✅ | DockSpaceOverViewport in beginFrame |
| Panels register/unregister | ✅ | PanelLifecycle: 4/4 ✅ |
| ImGui context init/destroy | ✅ | HudInit: 2/2 ✅ |
| hud_tests.exe: 6/6 passed | ✅ | Owner confirmed 2026-04-19 |
| QA Gate | ⏳ | Owner dock/undock panels pending |

---

## EP-ENG-05-S01 — Game State Machine  ✅ COMPLETE — 2026-04-19
| Criterion | Status | Notes |
|---|---|---|
| `IGameState.h` + `GameStateMachine.h/cpp` | ✅ | Push/pop/replace stack |
| enter/exit fire exactly once per transition | ✅ | test_state_lifecycle_hooks: 7/7 |
| push/pop mechanics | ✅ | test_state_push_pop: 5/5 |
| Empty stack safe | ✅ | test_empty_stack_safe: 5/5 |
| QA Gate | ⏳ | Owner API review pending |

---

## QA Gates Remaining

| Sprint | Gate |
|---|---|
| EP-ENG-02-S01 | Owner reviews World/component API — confirms clean for sim use |
| EP-ENG-02-S02 | Owner edits `config/game.json`, confirms app reads new value |
| EP-ENG-03-S01 | Owner sees coloured quad rendered via DSA path |
| EP-ENG-04-S01 | Owner docks/undocks test panels, confirms layout persists |
| EP-ENG-05-S01 | Owner reviews GameStateMachine API — confirms no game domain refs |
