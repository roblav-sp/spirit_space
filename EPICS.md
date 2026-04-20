# Spirit Space — Epics & Sprint Plan

> Version: 3.0
> Date: 2026-04-12
> Status: Approved — Baseline
> Owner: Roberto Lavoie

---

## 1. Three-Phase Development Model

The project is split into three phases with hard gates between them.

**Phase 1** produces a standalone, reusable **Game Engine** — generic, zero domain knowledge.
**Phase 2** proves the engine by building a **Star Simulation** on top of it (Sandbox Mode).
**Phase 3** adds all gameplay on top of the proven simulation — **Spirit Space Game** (Game Mode).

No Phase 2 work begins until Phase 1 passes its QA gate.
No Phase 3 work begins until Phase 2 passes its QA gate.

```
┌─────────────────────────────────────────────────────────────┐
│  PHASE 1 — GAME ENGINE                                      │
│  Reusable: ECS, renderer, audio, input, config, HUD, save  │
│  EP-ENG-01 → EP-ENG-02 → EP-ENG-03 → EP-ENG-04 → EP-ENG-05│
└───────────────────────┬─────────────────────────────────────┘
                        │
              PHASE 1 QA MILESTONE
         (smoke-test app using engine only)
                        │
┌───────────────────────▼─────────────────────────────────────┐
│  PHASE 2 — STAR SIMULATION (Sandbox Mode)                   │
│  Proves engine: real stars, solar system, all flight modes  │
│  EP-SIM-01 → EP-SIM-02 → EP-SIM-03 → EP-SIM-04             │
│  EP-SIM-05 → EP-SIM-06 → EP-SIM-07 → EP-SIM-08             │
└───────────────────────┬─────────────────────────────────────┘
                        │
              PHASE 2 QA MILESTONE
          (full simulation playthrough)
                        │
┌───────────────────────▼─────────────────────────────────────┐
│  PHASE 3 — SPIRIT SPACE GAME (Game Mode)                    │
│  Gameplay layer on top of proven simulation                 │
│  EP-GAME-01 through EP-GAME-09                              │
└─────────────────────────────────────────────────────────────┘
```

---

## 2. Phase 1 Work Tracks

Phase 1 has **2 sequential tracks** — foundation first, then all other engine systems run in parallel.

```
TRACK 0 — FOUNDATION (must close before anything else starts)
  EP-ENG-01

TRACK A — RENDERING FRAMEWORK
  EP-ENG-03

TRACK B — CORE SYSTEMS
  EP-ENG-02

TRACK C — HUD & AUDIO
  EP-ENG-04

TRACK D — ENGINE SERVICES
  EP-ENG-05
```

**What the engine includes:**
- Window + OpenGL 4.5 context (GLFW + GLAD)
- EnTT ECS setup with component registration pattern
- Renderer framework: shader compilation, VAO/VBO/DSA helpers, mesh loading, camera interface
- Input system: remappable bindings, GLFW event handling
- Config system: JSON read/write (nlohmann/json), config hot-reload
- Asset loader: Assimp (3D models), stb_image (textures)
- HUD framework: Dear ImGui docking integration, panel layout utilities
- Audio backend: miniaudio integration
- Save/load framework: generic JSON serialize/deserialize base
- Game state machine: push/pop state stack
- Time system: clock + warp multiplier
- Logging: rotating log file

**What the engine does NOT include:**
- Any star, planet, ship, or game domain types
- HYG data, Horizons ephemeris, or astronomical datasets
- Flight modes, warp, or simulation-specific logic
- Any Phase 3 gameplay (energy, combat, aliens, tech tree, Spirit Realm)

---

## 3. Phase 2 Work Tracks

Phase 2 runs **4 parallel tracks** after EP-SIM-01 (data pipeline) closes.

```
TRACK A — RENDERING (sim content)
  EP-SIM-02 → EP-SIM-03 → EP-SIM-04

TRACK B — FLIGHT
  EP-SIM-05

TRACK C — HUD & NAVIGATION
  EP-SIM-06

TRACK D — SIMULATION FRAMEWORK + ASSETS
  EP-SIM-07 → EP-SIM-08 (parallel)
```

**What the simulation includes:**
- Data download + validation (HYG 119k+ stars, Horizons ephemeris, exoplanet catalog)
- Star field (HYG data, spectral colour, magnitude size, LOD)
- Skybox (NASA/ESA Milky Way panorama + procedural nebula shader)
- Full Solar System (planets, moons, orbital rings, NASA textures)
- All three flight modes (Planetary, System Warp, Super Warp)
- Simulation HUD (speed, nearest body, alerts)
- Solar system minimap + galaxy/star map
- Main menu with Sandbox mode, settings, save position, time warp
- Player ship 3D model

**What the simulation does NOT include:**
- Energy pool, shields, hull damage
- Weapons or combat
- Aliens or factions
- Tech tree or progression
- Spirit Realm

---

## 4. Phase 3 Work Tracks

Phase 3 runs **6 parallel tracks** once Phase 2 is signed off.

```
TRACK A3 — SHIP SYSTEMS & GAME FRAMEWORK
  EP-GAME-01 → EP-GAME-02

TRACK B3 — COMBAT & SENSORS
  EP-GAME-03

TRACK C3 — HUD GAME EXTENSIONS
  EP-GAME-04

TRACK D3 — ALIENS, FACTIONS & TECH TREE
  EP-GAME-05 → EP-GAME-06

TRACK E3 — SPIRIT REALM
  EP-GAME-07

TRACK F3 — AUDIO & ASSETS (parallel, independent)
  EP-GAME-08
  EP-GAME-09
```

---

## 5. Epic Definitions

---

## ═══════════════════════════════
## PHASE 1 — GAME ENGINE
## ═══════════════════════════════

---

### EP-ENG-01 — Foundation
**Phase**: 1 | **Track**: 0 (shared gate — all tracks blocked until this closes)
**Goal**: Build system, CI, window opens, OpenGL 4.5 context confirmed.
**Status**: ✅ ALL SPRINTS COMPLETE — owner sign-off 2026-04-19

#### EP-ENG-01-S01 — Build System & Repository ✅ DONE
| Item | Detail |
|---|---|
| Goal | Makefile + vcpkg project, GitHub repo, CI pipeline |
| Deliverables | `Makefile`, `.github/workflows/ci.yml`, vcpkg manifest, GTest integrated |
| Acceptance | `make` succeeds in WSL; CI green on push; smoke tests pass |
| Unit Tests | Smoke: 4/4 passed |
| QA Gate | ✅ Owner builds confirmed; all smoke tests pass |

#### EP-ENG-01-S02 — Window & OpenGL Context ✅ DONE
| Item | Detail |
|---|---|
| Goal | GLFW window, GLAD loads OpenGL 4.5 context, triangle renders |
| Deliverables | `engine/src/core/Window.cpp`, GLFW + GLAD via vcpkg |
| Acceptance | 1920×1080 window opens; red triangle renders; fullscreen toggle works; closes cleanly |
| Unit Tests | `test_window_context`: GL 4.5 ✅; `test_fullscreen_toggle`: no crash ✅ |
| QA Gate | ✅ Owner confirmed: OpenGL 4.5.0 Intel, window opened, audio ready, all tests pass |

---

### EP-ENG-02 — Core Engine Systems
**Phase**: 1 | **Track**: B
**Goal**: ECS layer, config system, input system, logging.
**Depends on**: EP-ENG-01

#### EP-ENG-02-S01 — ECS Layer
| Item | Detail |
|---|---|
| Goal | EnTT registry setup, component registration pattern, entity lifecycle |
| Deliverables | `engine/src/ecs/World.cpp`, component conventions documented |
| Acceptance | Create/destroy 10k entities with 3 components; no memory leak; component prefix `C` enforced in tests |
| Unit Tests | `test_entity_lifecycle`; `test_component_add_remove`; `test_view_query` |
| QA Gate | Owner reviews component API; confirms pattern is clean for sim use |

#### EP-ENG-02-S02 — Config System
| Item | Detail |
|---|---|
| Goal | JSON config read/write; graceful defaults on missing keys |
| Deliverables | `engine/src/core/Config.cpp` |
| Acceptance | Read key → correct value; missing key → default; write → persists on reload; no crash on corrupt file |
| Unit Tests | `test_config_read`; `test_config_missing_key_default`; `test_config_write_persist` |
| QA Gate | Owner manually edits config; confirms app reads new value |

#### EP-ENG-02-S03 — Input System
| Item | Detail |
|---|---|
| Goal | Remappable key bindings backed by config; GLFW event dispatch |
| Deliverables | `engine/src/core/InputSystem.cpp`, binding schema in `config/game.json` |
| Acceptance | All bindings remappable; rebind persists across sessions; no duplicate binding allowed |
| Unit Tests | `test_remap_binding`; `test_binding_persist`; `test_no_duplicate_binding` |
| QA Gate | Owner remaps a key; restarts app; confirms new binding active |

#### EP-ENG-02-S04 — Logging
| Item | Detail |
|---|---|
| Goal | Rotating log file; levels: DEBUG, INFO, WARN, ERROR |
| Deliverables | `engine/src/core/Log.cpp` |
| Acceptance | Log file created on launch; rotation at configured size; ERROR entries flushed immediately |
| Unit Tests | `test_log_rotation`; `test_log_level_filter` |
| QA Gate | Owner inspects log after run; confirms format and levels |

---

### EP-ENG-03 — Renderer Framework
**Phase**: 1 | **Track**: A
**Goal**: Shader system, DSA helpers, mesh loading, camera interface — generic, domain-free.
**Depends on**: EP-ENG-01

#### EP-ENG-03-S01 — Shader System & DSA Helpers
| Item | Detail |
|---|---|
| Goal | Compile/link GLSL shaders; DSA wrapper for VAO/VBO/UBO/texture |
| Deliverables | `engine/src/render/Shader.cpp`, `engine/src/render/GpuBuffer.cpp` |
| Acceptance | Shader compiles from file; compile errors surface in log; DSA calls succeed with no legacy bind calls |
| Unit Tests | `test_shader_compile`; `test_shader_missing_file_error`; `test_dsa_buffer_upload` |
| QA Gate | Owner confirms a coloured quad renders via DSA path only |

#### EP-ENG-03-S02 — Mesh Loading & Asset Loader
| Item | Detail |
|---|---|
| Goal | Assimp mesh import (OBJ/FBX/GLTF); stb_image texture loader; GPU upload |
| Deliverables | `engine/src/render/MeshLoader.cpp`, `engine/src/render/TextureLoader.cpp` |
| Acceptance | OBJ and GLTF load correctly; texture uploads without error; missing file returns error, no crash |
| Unit Tests | `test_mesh_load_obj`; `test_texture_load_png`; `test_mesh_missing_file_error` |
| QA Gate | Owner sees a loaded mesh (any test model) rendered in the window |

#### EP-ENG-03-S03 — Camera System
| Item | Detail |
|---|---|
| Goal | Camera abstracted behind `ICamera` interface; first-person implementation |
| Deliverables | `engine/include/render/ICamera.h`, `engine/src/render/FirstPersonCamera.cpp` |
| Acceptance | Camera interface returns view/projection matrices; first-person impl tracks mouse; interface can be swapped without touching flight code |
| Unit Tests | `test_camera_view_matrix`; `test_camera_projection_matrix`; `test_camera_interface_swap` |
| QA Gate | Owner moves camera around test scene; confirms smooth tracking |

---

### EP-ENG-04 — HUD Framework & Audio Backend
**Phase**: 1 | **Track**: C
**Goal**: Dear ImGui docking integration; miniaudio backend. Both generic — zero sim/game domain content.
**Depends on**: EP-ENG-01

#### EP-ENG-04-S01 — HUD Framework
| Item | Detail |
|---|---|
| Goal | Dear ImGui docking branch integrated; panel layout utilities; render loop hook |
| Deliverables | `engine/src/hud/HudSystem.cpp`, ImGui via vcpkg |
| Acceptance | ImGui docking works; panels dock/undock; render calls integrate cleanly with engine loop; zero game-specific widgets |
| Unit Tests | `test_imgui_init`; `test_panel_lifecycle` |
| QA Gate | Owner docks/undocks test panels; confirms layout persists |

#### EP-ENG-04-S02 — Audio Backend ✅ DONE
| Item | Detail |
|---|---|
| Goal | miniaudio integration; play/stop/volume API; positional audio stubs |
| Deliverables | `engine/src/audio/AudioSystem.cpp`, miniaudio single-header via vcpkg |
| Acceptance | Audio file plays; volume control works; no crash on missing file; API surface is generic (no game audio types) |
| Unit Tests | `test_audio_play_stop`; `test_audio_volume`; `test_audio_missing_file_no_crash` |
| QA Gate | ✅ Owner heard 440Hz beep play and stop on P keypress — sign-off 2026-04-19 |

---

### EP-ENG-05 — Engine Services
**Phase**: 1 | **Track**: D
**Goal**: Game state machine, save/load framework, time system. All generic — no game domain types.
**Depends on**: EP-ENG-01

#### EP-ENG-05-S01 — Game State Machine
| Item | Detail |
|---|---|
| Goal | Push/pop state stack; state transitions: enter/update/exit lifecycle |
| Deliverables | `engine/src/core/GameStateMachine.cpp` |
| Acceptance | Push/pop states in tests; enter/exit fire exactly once per transition; no state leaks |
| Unit Tests | `test_state_push_pop`; `test_state_lifecycle_hooks`; `test_empty_stack_safe` |
| QA Gate | Owner reviews state machine API; confirms it's generic (no menu/game references) |

#### EP-ENG-05-S02 — Save/Load Framework
| Item | Detail |
|---|---|
| Goal | Generic JSON serialize/deserialize base class; field registration pattern |
| Deliverables | `engine/src/core/SaveSystem.cpp`, `engine/include/core/ISaveable.h` |
| Acceptance | ISaveable implementors round-trip all fields via serialize/deserialize; extra fields in JSON ignored gracefully |
| Unit Tests | `test_saveable_roundtrip`; `test_extra_field_ignored`; `test_missing_field_default` |
| QA Gate | Owner reviews ISaveable API; confirms sim and game types can implement it independently |

#### EP-ENG-05-S03 — Time System
| Item | Detail |
|---|---|
| Goal | Engine clock; configurable warp multiplier; pause support |
| Deliverables | `engine/src/core/TimeSystem.cpp` |
| Acceptance | Clock advances correctly at 1x; warp multiplier scales dt correctly; pause freezes time; warp factor readable from config |
| Unit Tests | `test_time_advance_1x`; `test_time_warp_scale`; `test_time_pause` |
| QA Gate | Owner confirms time advances at 1x and a visible warp speed in test |

---

## ╔══════════════════════════════════════╗
## ║  PHASE 1 QA MILESTONE — ENGINE      ║
## ║                                      ║
## ║  A standalone smoke-test app built   ║
## ║  using only engine/ headers must:   ║
## ║  [ ] Open a window                  ║
## ║  [ ] Render a mesh via DSA          ║
## ║  [ ] Load a config value            ║
## ║  [ ] Create ECS entities            ║
## ║  [ ] Play an audio clip             ║
## ║  [ ] Round-trip a save file         ║
## ║  [ ] Zero sim/ or game/ includes    ║
## ║  [ ] Owner sign-off recorded        ║
## ╚══════════════════════════════════════╝

---

## ═══════════════════════════════
## PHASE 2 — STAR SIMULATION
## ═══════════════════════════════

---

### EP-SIM-01 — Data Pipeline
**Phase**: 2 | **Track**: 0 (shared gate for all sim tracks)
**Goal**: All astronomical datasets downloaded, validated, and parsed.
**Depends on**: Phase 1 complete

#### EP-SIM-01-S01 — Data Download & Validation
| Item | Detail |
|---|---|
| Goal | Automated download scripts for HYG, Horizons, exoplanet catalog; validation passes |
| Deliverables | `scripts/download_data.py`, `scripts/validate_data.py` |
| Acceptance | HYG (119k+ rows), exoplanet CSV, Horizons data all downloaded; 0 critical validation errors |
| Unit Tests | `test_hyg_parse`; `test_exoplanet_parse`; `test_horizons_parse` |
| QA Gate | Owner reviews first 10 rows of each dataset |

---

### EP-SIM-02 — Star Field
**Phase**: 2 | **Track**: A
**Goal**: Render HYG stars as point sprites with correct spectral colour and magnitude size.
**Depends on**: EP-SIM-01

#### EP-SIM-02-S01 — Star Renderer
| Item | Detail |
|---|---|
| Goal | HYG data loaded into ECS; stars rendered as point sprites; spectral colour + magnitude size correct |
| Deliverables | `sim/src/render/StarRenderer.cpp`, GLSL star shaders, spectral colour LUT |
| Acceptance | Stars render; colour matches spectral class chart; size scales with magnitude; FPS ≥ 60 |
| Unit Tests | `test_spectral_color`: each class O–M returns correct RGB range; `test_coord_conversion`: RA/Dec/dist ↔ XYZ round-trip |
| QA Gate | Owner inspects star field; recognisable constellations present |

#### EP-SIM-02-S02 — LOD Benchmarking
| Item | Detail |
|---|---|
| Goal | Test LOD strategies; implement chosen approach; quality tier toggle (Low/Medium/High) |
| Deliverables | Benchmark results doc; LOD implementation |
| Acceptance | All 3 tiers produce measurable FPS difference; High ≥ 60fps on target hardware |
| Unit Tests | `test_lod_cull`: frustum culling reduces rendered count correctly |
| QA Gate | Owner tests all 3 tiers; confirms visual/performance trade-off acceptable |

---

### EP-SIM-03 — Skybox
**Phase**: 2 | **Track**: A (continues from EP-SIM-02)
**Goal**: NASA/ESA Milky Way panorama as skybox with procedural nebula overlay.
**Depends on**: EP-SIM-01

#### EP-SIM-03-S01 — Skybox & Nebula
| Item | Detail |
|---|---|
| Goal | Cubemap skybox from panorama; procedural nebula GLSL shader layered on top |
| Deliverables | `sim/src/render/Skybox.cpp`, nebula GLSL shader |
| Acceptance | Skybox renders at all orientations; no seams; nebula blends naturally |
| Unit Tests | `test_skybox_load`: cubemap loads without errors |
| QA Gate | Owner rotates 360° and confirms visual quality |

---

### EP-SIM-04 — Solar System
**Phase**: 2 | **Track**: A (continues from EP-SIM-03)
**Goal**: Sun, 8 planets, major moons, dwarf planets, orbital rings with NASA textures.
**Depends on**: EP-SIM-01

#### EP-SIM-04-S01 — Planet Renderer & Orbits
| Item | Detail |
|---|---|
| Goal | All 8 planets at correct Horizons ephemeris positions; orbital rings; rings on Saturn/Uranus; atmosphere shaders |
| Deliverables | `sim/src/render/PlanetRenderer.cpp`, orbit ellipse renderer, ring system shader |
| Acceptance | All 8 planets correct for a reference date; orbit rings correct; rings on Saturn/Uranus; atmosphere shaders on habitable planets |
| Unit Tests | `test_kepler`: ellipse from a/e/i matches reference; `test_planet_scale`: radii in expected range; `test_ephemeris`: positions match NASA for test date |
| QA Gate | Owner compares layout against NASA Eyes screenshot; orbits confirmed |

---

### EP-SIM-05 — Flight Modes
**Phase**: 2 | **Track**: B
**Goal**: All three flight modes functional. No energy management — unlimited propulsion in sandbox.
**Depends on**: EP-SIM-01

#### EP-SIM-05-S01 — Planetary Flight (Mode 3)
| Item | Detail |
|---|---|
| Goal | 6-DOF antigravity ship movement, damped controls, first-person camera, collision |
| Deliverables | `sim/src/flight/Ship.cpp`, `sim/src/flight/FlightController.cpp` |
| Acceptance | Ship moves all 6 axes; smooth damping; no planet clip; camera interface in place |
| Unit Tests | `test_ship_physics`: thrust → velocity; `test_damping`: converges to zero; `test_collision`: surface intersection |
| QA Gate | Owner flies around Earth 5 min; controls feel right; no clipping |

#### EP-SIM-05-S02 — System Warp (Mode 2)
| Item | Detail |
|---|---|
| Goal | In-system warp with damped curve, HUD ETA, lockout zone |
| Deliverables | `sim/src/flight/SystemWarp.cpp`, warp speed curve |
| Acceptance | Warp to any planet; no overshoot; lockout within gravity zone; ETA shown on HUD |
| Unit Tests | `test_warp_curve`: displacement integrates correctly; `test_warp_lockout`: blocked inside min radius |
| QA Gate | Owner warps Earth → Mars → Jupiter; feel and arrival position confirmed |

#### EP-SIM-05-S03 — Super Warp (Mode 1)
| Item | Detail |
|---|---|
| Goal | Interstellar warp to any HYG star with tunnel visual; galaxy map integration |
| Deliverables | `sim/src/flight/SuperWarp.cpp`, warp tunnel GLSL shader |
| Acceptance | Select star on galaxy map → warp → arrive at outer boundary; tunnel ~3s; mode transition seamless |
| Unit Tests | `test_star_lookup`: name search returns correct HYG record; `test_mode_transition`: state machine correct |
| QA Gate | Owner warps to Alpha Centauri, Sirius, and a random distant star; tunnel acceptable |

---

### EP-SIM-06 — HUD & Navigation
**Phase**: 2 | **Track**: C
**Goal**: Simulation HUD — speed, nearest body, minimap, galaxy map. No gameplay gauges.
**Depends on**: EP-SIM-01

#### EP-SIM-06-S01 — Core Simulation HUD
| Item | Detail |
|---|---|
| Goal | Flight mode indicator, speed readout, nearest star/planet info panel, alerts |
| Deliverables | `sim/src/hud/SimHUD.cpp`, Dear ImGui layout via engine HUD framework |
| Acceptance | All elements update in real time; units auto-scale; no overlap at 1080p and 1440p |
| Unit Tests | `test_nearest_star`: spatial query returns correct result; `test_hud_layout`: all elements within screen bounds |
| QA Gate | Owner flies and confirms HUD legibility and correctness |

#### EP-SIM-06-S02 — Solar System Minimap
| Item | Detail |
|---|---|
| Goal | Lower-left minimap: orbital rings, planet dots, ship X marker, distance to nearest |
| Deliverables | `sim/src/hud/Minimap.cpp` |
| Acceptance | Minimap visible in Mode 2 and 3 only; ship position correct; inner-system zoom toggle works |
| Unit Tests | `test_minimap_position`: ship X placed on correct orbital ring |
| QA Gate | Owner warps between planets; minimap tracks correctly |

#### EP-SIM-06-S03 — Galaxy / Star Map
| Item | Detail |
|---|---|
| Goal | Full-screen star map with 10/50/100 LY filters, spectral colours, click-to-set-warp-target |
| Deliverables | `sim/src/hud/GalaxyMap.cpp` |
| Acceptance | Filters work; stars coloured by spectral class; click sets Super Warp target; explored systems marked |
| Unit Tests | `test_galaxy_filter`: filter returns only stars within band; `test_warp_target_set`: click sets correct target |
| QA Gate | Owner browses map, sets target, warps — end-to-end confirmed |

---

### EP-SIM-07 — Simulation Framework
**Phase**: 2 | **Track**: D
**Goal**: Main menu (Sandbox mode), settings, remappable controls, save position, time warp.
**Depends on**: EP-SIM-01

#### EP-SIM-07-S01 — Main Menu & Mode Select
| Item | Detail |
|---|---|
| Goal | Main menu: Sandbox Mode (active), Game Mode (locked — "Coming Soon"), Settings, Quit |
| Deliverables | `sim/src/core/SimStateMachine.cpp`, main menu UI |
| Acceptance | Sandbox Mode launches correctly; Game Mode locked with placeholder; all navigation works |
| Unit Tests | `test_state_machine`: all menu → sandbox transitions correct |
| QA Gate | Owner navigates all menus; confirms flow |

#### EP-SIM-07-S02 — Settings & Remappable Controls
| Item | Detail |
|---|---|
| Goal | Graphics quality, audio volume, fully remappable key bindings, all persisted |
| Deliverables | Settings UI using engine Config + InputSystem; `config/game.json` write/read |
| Acceptance | All bindings remappable; changes persist across sessions; graphics quality changes visible |
| Unit Tests | `test_settings_persist`: write → reload → assert equality |
| QA Gate | Owner remaps controls; adjusts quality; restarts app; confirms persistence |

#### EP-SIM-07-S03 — Save Position & Time Warp
| Item | Detail |
|---|---|
| Goal | Save/restore player position + orientation + in-game time via engine save framework; time warp control |
| Deliverables | `sim/src/core/SimSaveState.cpp` (implements ISaveable), `sim/src/core/TimeController.cpp` |
| Acceptance | Save/load restores position within epsilon; time warp 1x–10000x changes planet positions visibly; warp disengages during active flight warp |
| Unit Tests | `test_save_position`: position survives serialize/deserialize; `test_time_warp`: planet advances at 10000x |
| QA Gate | Owner saves, quits, reloads — position confirmed; time warp orbital motion confirmed |

---

### EP-SIM-08 — Player Ship Asset
**Phase**: 2 | **Track**: D (parallel with EP-SIM-07, independent)
**Goal**: At least one player ship 3D model ready for integration.
**Depends on**: Nothing — runs in parallel from Phase 2 start.

#### EP-SIM-08-S01 — Player Ship Model
| Item | Detail |
|---|---|
| Goal | 1+ player ship `.obj`/`.fbx` with UV textures, Blender source |
| Deliverables | Ship model files + textures in `assets/ships/` |
| Acceptance | Loads via engine MeshLoader; polygon count real-time appropriate; UV mapped |
| Unit Tests | `test_model_load`: mesh loads; vertex count within budget |
| QA Gate | Owner reviews ship model in test render |

---

## ╔══════════════════════════════════════╗
## ║  PHASE 2 QA MILESTONE — SIMULATION  ║
## ║                                      ║
## ║  Full simulation playthrough by      ║
## ║  owner required before Phase 3       ║
## ║  begins. Checklist:                  ║
## ║  [ ] Fly around Earth orbit          ║
## ║  [ ] System warp across solar system ║
## ║  [ ] Super warp to another star      ║
## ║  [ ] Browse galaxy map               ║
## ║  [ ] Use time warp — see planets move║
## ║  [ ] Save and reload position        ║
## ║  [ ] Test all 3 quality tiers        ║
## ║  [ ] Owner sign-off recorded         ║
## ╚══════════════════════════════════════╝

---

## ═══════════════════════════════
## PHASE 3 — SPIRIT SPACE GAME
## ═══════════════════════════════

---

### EP-GAME-01 — Ship Systems
**Phase**: 3 | **Track**: A3
**Goal**: Energy pool, shields, hull damage, multi-ship config.
**Depends on**: Phase 2 complete

#### EP-GAME-01-S01 — Energy Pool & Allocation
| Item | Detail |
|---|---|
| Goal | Single shared energy pool with passive regen and manual allocation sliders |
| Deliverables | `game/src/ship/EnergySystem.cpp`, `game/src/ship/AllocationUI.cpp` |
| Acceptance | Energy drains/regens correctly; allocation sliders update per-system budgets in real time; warp lockout at low energy |
| Unit Tests | `test_energy_pool`: drain + regen over time; `test_allocation`: slider recalculates system budgets |
| QA Gate | Owner adjusts sliders; confirms energy depletion locks warp |

#### EP-GAME-01-S02 — Shield Bubble & Hull Damage
| Item | Detail |
|---|---|
| Goal | Shield renderer, progressive absorption, hull damage on shield collapse, game over trigger |
| Deliverables | `game/src/ship/ShieldSystem.cpp`, `game/src/render/ShieldRenderer.cpp`, `game/src/ship/DamageModel.cpp` |
| Acceptance | Shield bubble visible; opacity matches level; hull damaged only at 0% shield; game over at 0% hull; carried tech gifts lost on destruction |
| Unit Tests | `test_shield_absorption`; `test_hull_damage`; `test_game_over` |
| QA Gate | Owner drains shield deliberately; confirms hull damage; triggers game over |

#### EP-GAME-01-S03 — Multi-Ship Config
| Item | Detail |
|---|---|
| Goal | `config/ships.json` drives all ship definitions; 3 starter ships |
| Deliverables | `config/ships.json`, `game/src/ship/ShipConfig.cpp`, ship selector UI |
| Acceptance | All ships load from JSON; adding new ship requires no recompile; stat differences meaningful |
| Unit Tests | `test_ship_config_parse`; `test_ship_stats_apply` |
| QA Gate | Owner adds custom ship to config; confirms it appears in-game |

---

### EP-GAME-02 — Game Framework
**Phase**: 3 | **Track**: A3 (continues from EP-GAME-01)
**Goal**: Full game states — ship selector, complete save/load, game over, pause.
**Depends on**: EP-GAME-01

#### EP-GAME-02-S01 — Ship Selector & Game Mode Launch
| Item | Detail |
|---|---|
| Goal | Game Mode main menu path: ship selector → New Game → spawn in Earth orbit |
| Deliverables | Ship selector screen, Game Mode state extending engine state machine |
| Acceptance | Game Mode accessible from main menu; ship selector shows all ships with stats; New Game spawns at Earth orbit facing Sun |
| Unit Tests | `test_spawn_position`: spawn within Earth orbit bounds |
| QA Gate | Owner launches Game Mode, selects ship, spawns — confirmed correct |

#### EP-GAME-02-S02 — Full Save / Load & Game Over
| Item | Detail |
|---|---|
| Goal | Save full game state (position, ship, energy, hull, reputation, tech tree, gifts, time, realm) |
| Deliverables | `game/src/core/GameSaveState.cpp` (implements ISaveable) with game-layer fields |
| Acceptance | All game-state fields survive save/load round-trip; game over → reload restores correctly; gifts lost on death not restored |
| Unit Tests | `test_full_save_roundtrip`; `test_gifts_lost_on_death` |
| QA Gate | Owner plays, saves, dies, reloads — full state verified |

#### EP-GAME-02-S03 — Pause Menu
| Item | Detail |
|---|---|
| Goal | In-game pause: Resume, Save, Tech Tree, Settings, Quit to Menu |
| Deliverables | Pause menu UI |
| Acceptance | Pause freezes simulation; all options functional; time warp disabled while paused |
| Unit Tests | `test_pause_state`: simulation time does not advance while paused |
| QA Gate | Owner pauses mid-flight; confirms all pause options |

---

### EP-GAME-03 — Combat & Sensors
**Phase**: 3 | **Track**: B3
**Goal**: Energy weapons, physics-based detection, red alert, subsystem damage, realm-crossing effect.
**Depends on**: Phase 2 complete

#### EP-GAME-03-S01 — Energy Weapons
| Item | Detail |
|---|---|
| Goal | Laser/plasma burst weapons drawing from energy Weapons budget |
| Deliverables | `game/src/combat/WeaponSystem.cpp`, projectile renderer |
| Acceptance | Weapons fire; energy depletes from weapons allocation; hits register |
| Unit Tests | `test_weapon_fire`: energy deducted; `test_projectile_hit`: collision callback fires |
| QA Gate | Owner fires at test target; confirms energy drain and visual |

#### EP-GAME-03-S02 — Physics-Based Detection
| Item | Detail |
|---|---|
| Goal | EM/heat/light detection model; weapons-hot → red alert only; sensor contact HUD alert |
| Deliverables | `game/src/combat/SensorSystem.cpp` |
| Acceptance | Enemy detected within range; red alert fires only on confirmed weapons hot; sensor contact shown on HUD |
| Unit Tests | `test_detection_range`; `test_red_alert_weapons_hot` |
| QA Gate | Owner approaches enemy; confirms detection timing and red alert trigger |

#### EP-GAME-03-S03 — Realm Crossing & Subsystem Damage
| Item | Detail |
|---|---|
| Goal | Star Trek shimmer effect on realm crossings; subsystem degradation at hull thresholds |
| Deliverables | `game/src/render/RealmCrossShader.glsl`, `game/src/ship/SubsystemDamage.cpp` |
| Acceptance | Shimmer effect plays on enemy materialisation; correct subsystem degrades at defined hull % |
| Unit Tests | `test_subsystem_damage`: correct system degrades at correct threshold |
| QA Gate | Owner observes enemy cross from Spirit Realm; observes subsystem failure sequence |

---

### EP-GAME-04 — HUD Game Extensions
**Phase**: 3 | **Track**: C3
**Goal**: Add game-layer HUD elements on top of sim HUD (energy gauges, combat alerts, realm indicator).
**Depends on**: Phase 2 complete, EP-GAME-01 interface available

#### EP-GAME-04-S01 — Energy & Combat HUD
| Item | Detail |
|---|---|
| Goal | Energy pool bar, per-subsystem allocation bars, shield %, hull %, realm indicator, red alert overlay |
| Deliverables | `game/src/hud/GameHUD.cpp` extending engine HUD framework |
| Acceptance | All gauges update in real time; red alert overlay visible; realm indicator switches correctly |
| Unit Tests | `test_hud_layout_game`: all game elements within screen bounds |
| QA Gate | Owner enters combat; confirms all gauges respond correctly |

#### EP-GAME-04-S02 — Alien Contact Panel
| Item | Detail |
|---|---|
| Goal | Side panel for Federation alien dialogue with race portrait and accept/decline options |
| Deliverables | `game/src/hud/AlienContactPanel.cpp` |
| Acceptance | Panel opens on contact; portrait loads when available; gift accept/decline functional |
| Unit Tests | `test_contact_panel_trigger`: Federation contact opens panel |
| QA Gate | Owner encounters alien; reviews panel UI and interactions |

---

### EP-GAME-05 — Alien Civilisations & Factions
**Phase**: 3 | **Track**: D3
**Goal**: Alien encounters, dialogue, reputation, intro narrative event.
**Depends on**: Phase 2 complete, EP-GAME-04 interface available

#### EP-GAME-05-S01 — Alien Spawning
| Item | Detail |
|---|---|
| Goal | Alien ships at foreign star systems; 80/20 split; encounter detection |
| Deliverables | `game/src/alien/AlienSpawner.cpp`, `game/src/alien/FactionConfig.cpp` |
| Acceptance | Aliens appear only at non-Sol systems; frequency scales with exploration distance; 80/20 ratio within tolerance |
| Unit Tests | `test_faction_split`; `test_no_spawn_in_sol` |
| QA Gate | Owner explores 3 systems; confirms encounters and faction ratio |

#### EP-GAME-05-S02 — Dialogue & Tech Gift System
| Item | Detail |
|---|---|
| Goal | Text dialogue panel, race portrait, gift accept/decline, gift added to carried inventory |
| Deliverables | `game/src/alien/DialogueSystem.cpp` |
| Acceptance | Dialogue opens on Federation contact; gift accepted → inventory; declined → no effect |
| Unit Tests | `test_dialogue_trigger`; `test_gift_accept` |
| QA Gate | Owner accepts a gift and sees it in inventory |

#### EP-GAME-05-S03 — Reputation System
| Item | Detail |
|---|---|
| Goal | Per-race reputation, hostile state on attack, mission-based recovery |
| Deliverables | `game/src/alien/ReputationSystem.cpp`, stub mission framework |
| Acceptance | Attack → Hostile; Hostile race attacks on sight; mission complete → rep increments |
| Unit Tests | `test_reputation_attack`; `test_reputation_recovery` |
| QA Gate | Owner attacks Federation alien; observes hostility; completes recovery mission |

#### EP-GAME-05-S04 — Intro Narrative Event
| Item | Detail |
|---|---|
| Goal | Opening Spirit Realm alien contact event on New Game — introduces lore |
| Deliverables | `game/src/alien/IntroEvent.cpp`, intro dialogue |
| Acceptance | Event fires once on New Game; skippable; introduces Spirit Realm concept |
| Unit Tests | `test_intro_fires_once`: does not fire on Load |
| QA Gate | Owner plays intro; confirms lore hook is clear |

---

### EP-GAME-06 — Technology Progression
**Phase**: 3 | **Track**: D3 (continues from EP-GAME-05)
**Goal**: Tech tree, alien gift inventory, Earth R&D loop, permanent unlocks.
**Depends on**: EP-GAME-05

#### EP-GAME-06-S01 — Tech Tree Data & UI
| Item | Detail |
|---|---|
| Goal | 6-branch tech tree; UI accessible from pause and main menu |
| Deliverables | `game/src/tech/TechTree.cpp`, `game/src/hud/TechTreeUI.cpp` |
| Acceptance | All 6 branches visible; prerequisites gate nodes correctly; unlocked nodes visually distinct |
| Unit Tests | `test_tech_prereqs`: locked node cannot be unlocked without prerequisites |
| QA Gate | Owner browses tree; confirms layout clarity and logic |

#### EP-GAME-06-S02 — Gift Inventory & Earth R&D
| Item | Detail |
|---|---|
| Goal | Carried gift inventory; deposit at Earth/Sol; R&D conversion; permanent unlock |
| Deliverables | `game/src/tech/GiftInventory.cpp`, `game/src/tech/RnDSystem.cpp` |
| Acceptance | Gifts in inventory on accept; deposit at Sol queues R&D; unlock persists after death; carried gifts lost on death |
| Unit Tests | `test_gift_lost_on_death`; `test_rnd_persist` |
| QA Gate | Owner collects, deposits, dies, confirms deposit survived |

#### EP-GAME-06-S03 — Tech Applied to Ship Builds
| Item | Detail |
|---|---|
| Goal | New ships built at Earth reflect current tech tree unlocks |
| Deliverables | Integration between `TechTree` and `ShipConfig` |
| Acceptance | Unlocked tech node changes stats on next ship build; prior ship unaffected |
| Unit Tests | `test_tech_applies_to_build` |
| QA Gate | Owner unlocks propulsion node; builds new ship; confirms improved warp efficiency |

---

### EP-GAME-07 — Spirit Realm
**Phase**: 3 | **Track**: E3
**Goal**: Spirit Realm render layer, content, entry mechanics, crossing effect.
**Depends on**: Phase 2 complete (render pipeline), EP-GAME-06 (tech for entry unlock)

#### EP-GAME-07-S01 — Realm Render Layer
| Item | Detail |
|---|---|
| Goal | Second render pass toggles Spirit Realm overlay on existing scene |
| Deliverables | `game/src/render/SpiritRealmRenderer.cpp`, realm overlay GLSL shaders |
| Acceptance | Toggling realm activates/deactivates overlay; normal scene unaffected; FPS regression < 10% |
| Unit Tests | `test_realm_toggle`: render pass count changes on toggle |
| QA Gate | Owner toggles realm in Earth orbit; layer appears and disappears cleanly |

#### EP-GAME-07-S02 — Spirit Realm Solar System Content
| Item | Detail |
|---|---|
| Goal | Earth city layer, planet energy hubs, star civilisation activity in Spirit Realm |
| Deliverables | Spirit Realm geometry, `game/src/world/SpiritContent.cpp` |
| Acceptance | Earth shows spirit city structures; planets show orbital hubs; stars show activity rings |
| Unit Tests | `test_spirit_content_load`: all objects load without errors |
| QA Gate | Owner enters Spirit Realm at Earth; reviews city layer, planetary hubs |

#### EP-GAME-07-S03 — Realm Entry & Transition Effect
| Item | Detail |
|---|---|
| Goal | Spirit Realm drive (tech-gated) and alien stargate entry; Star Trek shimmer on transition |
| Deliverables | `game/src/flight/RealmEntry.cpp`, shimmer GLSL shader |
| Acceptance | Entry requires drive unlock OR active stargate; shimmer plays on enter/exit; HUD realm indicator switches |
| Unit Tests | `test_realm_entry_gated`; `test_shimmer_plays` |
| QA Gate | Owner enters via stargate and via drive; shimmer and HUD indicator confirmed |

---

### EP-GAME-08 — Audio
**Phase**: 3 | **Track**: F3 (parallel, independent)
**Goal**: Full game audio — ambient, flight, combat, Spirit Realm soundscape. Builds on engine audio backend.
**Depends on**: Phase 2 complete (engine audio backend already integrated)

#### EP-GAME-08-S01 — Normal Space Ambient & Flight Audio
| Item | Detail |
|---|---|
| Goal | Normal space ambient soundscape; engine hum varies with speed; warp cues |
| Deliverables | `game/src/audio/FlightAudio.cpp`, sourced ambient + warp assets |
| Acceptance | Ambient plays on game start; engine hum varies with speed; warp cues distinct; volume control works |
| Unit Tests | `test_audio_events`: correct sound for each flight event |
| QA Gate | Owner flies, warps; all audio cues confirmed |

#### EP-GAME-08-S02 — Combat & Spirit Realm Audio
| Item | Detail |
|---|---|
| Goal | Combat sounds (shield hit, hull hit, weapon fire); distinct Spirit Realm ambient; realm transition audio |
| Deliverables | `game/src/audio/CombatAudio.cpp`, Spirit Realm audio assets |
| Acceptance | All combat events have audio; Spirit Realm ambient distinct; transition has distinct audio effect |
| Unit Tests | `test_combat_audio_events`; `test_realm_audio_switch` |
| QA Gate | Owner fights and enters/exits Spirit Realm; all soundscapes confirmed distinct |

---

### EP-GAME-09 — Game Layer Assets
**Phase**: 3 | **Track**: F3 (parallel, independent)
**Goal**: Alien ship models, race portraits, HUD icons.
**Depends on**: Nothing — runs in parallel with all Phase 3 tracks.

#### EP-GAME-09-S01 — Alien Ship Models
| Item | Detail |
|---|---|
| Goal | 2+ alien ship models with distinct faction silhouettes |
| Deliverables | `.obj` + textures per faction type in `assets/ships/alien/` |
| Acceptance | Distinct silhouettes; poly count real-time appropriate; loads via engine MeshLoader |
| Unit Tests | `test_alien_model_load` |
| QA Gate | Owner reviews models — distinct and recognisable |

#### EP-GAME-09-S02 — Race Portraits & HUD Icons
| Item | Detail |
|---|---|
| Goal | 3+ race portrait images (512×512); full HUD icon set |
| Deliverables | Portrait PNGs in `assets/portraits/`; icon SVG/PNG set in `assets/ui/` |
| Acceptance | Portraits readable at dialogue panel size; icons consistent in style |
| Unit Tests | N/A |
| QA Gate | Owner reviews portraits and icon set for quality and consistency |

---

## 6. Full Sprint Sequence

### Phase 1 — Engine

| Sprint ID | Epic | Sprint Name |
|---|---|---|
| P1-S01 | EP-ENG-01 | Build System & Repository ✅ DONE |
| P1-S02 | EP-ENG-01 | Window & OpenGL Context |
| P1-S03 | EP-ENG-02 | ECS Layer |
| P1-S04 | EP-ENG-02 | Config System |
| P1-S05 | EP-ENG-02 | Input System |
| P1-S06 | EP-ENG-02 | Logging |
| P1-S07 | EP-ENG-03 | Shader System & DSA Helpers |
| P1-S08 | EP-ENG-03 | Mesh Loading & Asset Loader |
| P1-S09 | EP-ENG-03 | Camera System |
| P1-S10 | EP-ENG-04 | HUD Framework |
| P1-S11 | EP-ENG-04 | Audio Backend |
| P1-S12 | EP-ENG-05 | Game State Machine |
| P1-S13 | EP-ENG-05 | Save/Load Framework |
| P1-S14 | EP-ENG-05 | Time System |
| **GATE** | — | **PHASE 1 QA MILESTONE — Smoke-test app sign-off** |

### Phase 2 — Star Simulation

| Sprint ID | Epic | Sprint Name |
|---|---|---|
| P2-S01 | EP-SIM-01 | Data Download & Validation |
| P2-S02 | EP-SIM-02 | Star Renderer |
| P2-S03 | EP-SIM-02 | LOD Benchmarking |
| P2-S04 | EP-SIM-03 | Skybox & Nebula |
| P2-S05 | EP-SIM-04 | Planet Renderer & Orbits |
| P2-S06 | EP-SIM-05 | Planetary Flight (Mode 3) |
| P2-S07 | EP-SIM-05 | System Warp (Mode 2) |
| P2-S08 | EP-SIM-05 | Super Warp (Mode 1) |
| P2-S09 | EP-SIM-06 | Core Simulation HUD |
| P2-S10 | EP-SIM-06 | Solar System Minimap |
| P2-S11 | EP-SIM-06 | Galaxy / Star Map |
| P2-S12 | EP-SIM-07 | Main Menu & Mode Select |
| P2-S13 | EP-SIM-07 | Settings & Remappable Controls |
| P2-S14 | EP-SIM-07 | Save Position & Time Warp |
| P2-S15 | EP-SIM-08 | Player Ship Model |
| **GATE** | — | **PHASE 2 QA MILESTONE — Full simulation sign-off** |

### Phase 3 — Spirit Space Game

| Sprint ID | Epic | Sprint Name |
|---|---|---|
| P3-S01 | EP-GAME-01 | Energy Pool & Allocation |
| P3-S02 | EP-GAME-01 | Shield Bubble & Hull Damage |
| P3-S03 | EP-GAME-01 | Multi-Ship Config |
| P3-S04 | EP-GAME-02 | Ship Selector & Game Mode Launch |
| P3-S05 | EP-GAME-02 | Full Save / Load & Game Over |
| P3-S06 | EP-GAME-02 | Pause Menu |
| P3-S07 | EP-GAME-03 | Energy Weapons |
| P3-S08 | EP-GAME-03 | Physics-Based Detection |
| P3-S09 | EP-GAME-03 | Realm Crossing & Subsystem Damage |
| P3-S10 | EP-GAME-04 | Energy & Combat HUD |
| P3-S11 | EP-GAME-04 | Alien Contact Panel |
| P3-S12 | EP-GAME-05 | Alien Spawning |
| P3-S13 | EP-GAME-05 | Dialogue & Tech Gift System |
| P3-S14 | EP-GAME-05 | Reputation System |
| P3-S15 | EP-GAME-05 | Intro Narrative Event |
| P3-S16 | EP-GAME-06 | Tech Tree Data & UI |
| P3-S17 | EP-GAME-06 | Gift Inventory & Earth R&D |
| P3-S18 | EP-GAME-06 | Tech Applied to Ship Builds |
| P3-S19 | EP-GAME-07 | Realm Render Layer |
| P3-S20 | EP-GAME-07 | Spirit Realm Solar System Content |
| P3-S21 | EP-GAME-07 | Realm Entry & Transition Effect |
| P3-S22 | EP-GAME-08 | Normal Space Ambient & Flight Audio |
| P3-S23 | EP-GAME-08 | Combat & Spirit Realm Audio |
| P3-S24 | EP-GAME-09 | Alien Ship Models |
| P3-S25 | EP-GAME-09 | Race Portraits & HUD Icons |
| P3-S26 | — | Full integration polish, LOD tuning, QA pass |
| P3-S27+ | — | Future: third-person camera, landing hook, satellite toggle |

---

## 7. QA Audit Protocol

**Every sprint ends with a QA gate. No next sprint starts until it clears.**

Per-sprint checklist:
- [ ] All acceptance criteria checked off
- [ ] All unit tests passing on CI
- [ ] Owner has built and run on own machine
- [ ] Owner has manually tested the deliverables
- [ ] No P0 regressions
- [ ] Owner sign-off recorded

**Phase 1 QA Milestone** (additional):
- [ ] Smoke-test application builds using only `engine/` headers
- [ ] Zero `sim/` or `game/` includes in engine smoke-test
- [ ] Owner formally signs off on engine quality before Phase 2 begins

**Phase 2 QA Milestone** (additional):
- [ ] Full exploration playthrough completed by owner
- [ ] All flight modes tested end-to-end
- [ ] Save/load confirmed
- [ ] Time warp orbital motion confirmed
- [ ] Owner formally signs off on simulation quality before Phase 3 begins

---

*All sprint work traces to a requirement in REQUIREMENTS.md. This document is the engineering baseline.*
