# Spirit Space — Epics & Sprint Plan

> Version: 2.0
> Date: 2026-04-04
> Status: Approved — Baseline
> Owner: Roberto Lavoie

---

## 1. Two-Phase Development Model

The project is split into two phases with a hard gate between them.

**Phase 1** produces a fully functional, standalone space simulation (Sandbox Mode).
**Phase 2** adds all gameplay systems on top of the proven simulation (Game Mode).

No Phase 2 work begins until Phase 1 passes a full QA playthrough sign-off.

```
┌─────────────────────────────────────────────────────────┐
│  PHASE 1 — SPACE SIMULATION                             │
│  Sandbox Mode: real universe, all flight, no gameplay   │
│  EP-01 → EP-02 → EP-03 → EP-05 → EP-06-SIM → EP-12-P1 │
└───────────────────────┬─────────────────────────────────┘
                        │
              PHASE 1 QA MILESTONE
              (owner sign-off required)
                        │
┌───────────────────────▼─────────────────────────────────┐
│  PHASE 2 — GAME LAYER                                   │
│  Game Mode: adds gameplay on top of simulation          │
│  EP-04 → EP-06-GAME                                     │
│  EP-07 (Combat)                                         │
│  EP-05-EXT (HUD game additions)                         │
│  EP-08 → EP-09 (Aliens & Tech Tree)                     │
│  EP-10 (Spirit Realm)                                   │
│  EP-11 (Audio)                                          │
│  EP-12-P2 (Alien Assets)                                │
└─────────────────────────────────────────────────────────┘
```

---

## 2. Phase 1 Work Tracks

Phase 1 has **4 parallel tracks** after EP-01 closes. Each track is self-contained.

```
TRACK A — RENDERING
  EP-01 → EP-02

TRACK B — FLIGHT
  EP-01 → EP-03

TRACK C — HUD & NAVIGATION
  EP-01 → EP-05-SIM

TRACK D — SIMULATION FRAMEWORK
  EP-01 → EP-06-SIM

TRACK F — PLAYER SHIP ASSET (fully independent)
  EP-12-P1
```

**What Phase 1 includes:**
- Real star field (HYG 119k+ stars, spectral color, magnitude size, LOD)
- Full Solar System (Horizons ephemeris, all planets/moons, orbital rings)
- Skybox (NASA/ESA Milky Way panorama + procedural nebula)
- All three flight modes (Mode 3, Mode 2, Mode 1)
- Gravity proximity warning; no atmosphere entry
- Time warp (1x–10000x)
- Core HUD (speed, nearest body, alerts)
- Solar system minimap + galaxy/star map
- Main menu with Sandbox / Game mode select
- Settings (graphics, audio, remappable controls)
- Save and restore position
- Player ship 3D model

**What Phase 1 does NOT include:**
- Energy pool or shield system
- Weapons or combat
- Aliens or factions
- Tech tree or progression
- Spirit Realm
- Ship damage model
- Game over

---

## 3. Phase 2 Work Tracks

Phase 2 has **5 parallel tracks** once Phase 1 is signed off. Each track extends or adds to the Phase 1 simulation engine.

```
TRACK A2 — SHIP SYSTEMS & GAME FRAMEWORK
  EP-04 → EP-06-GAME

TRACK B2 — COMBAT & SENSORS
  EP-07

TRACK C2 — HUD GAME EXTENSIONS
  EP-05-EXT

TRACK D2 — ALIENS, FACTIONS & TECH TREE
  EP-08 → EP-09

TRACK E2 — SPIRIT REALM
  EP-10

TRACK F2 — AUDIO & REMAINING ASSETS
  EP-11 (parallel)
  EP-12-P2 (parallel)
```

Cross-track interfaces (not blocking — coded to interface, integrated on delivery):
- EP-07 (Combat) consumes ship energy API from EP-04
- EP-05-EXT (HUD) consumes energy/shield data from EP-04
- EP-08 (Alien) consumes HUD panel API from EP-05-EXT
- EP-09 (Tech Tree) consumes ship stats API from EP-04
- EP-10 (Spirit Realm) consumes render pipeline from EP-02

---

## 4. Epic Definitions

---

## ═══════════════════════════════
## PHASE 1 — SPACE SIMULATION
## ═══════════════════════════════

---

### EP-01 — Foundation
**Phase**: 1 | **Track**: All (shared gate)
**Goal**: Build system, CI, all data downloaded and validated, window opens.
**Blocks**: All other epics.

#### EP-01-S01 — Build System & Repository
| Item | Detail |
|---|---|
| Goal | CMake + vcpkg project, GitHub repo, CI pipeline |
| Deliverables | `CMakeLists.txt`, `.github/workflows/ci.yml`, vcpkg manifest, GTest integrated |
| Acceptance | `cmake --build` succeeds on Windows; CI green on push; empty test suite passes |
| Unit Tests | Smoke: build artifact exists and runs |
| QA Gate | Owner builds on own machine; CI confirmed green |

#### EP-01-S02 — Window & OpenGL Context
| Item | Detail |
|---|---|
| Goal | GLFW window, GLAD loads OpenGL 4.5 context, triangle renders |
| Deliverables | `src/core/Window.cpp`, GLFW + GLAD via vcpkg |
| Acceptance | 1920×1080 window opens; red triangle renders; closes cleanly |
| Unit Tests | `test_window_context`: GL version string ≥ 4.5 |
| QA Gate | Owner launches app and sees window |

#### EP-01-S03 — Data Download & Validation
| Item | Detail |
|---|---|
| Goal | Automated download scripts for all datasets; validation passes |
| Deliverables | `scripts/download_data.py`, `scripts/validate_data.py` |
| Acceptance | HYG (119k+ rows), exoplanet CSV, Horizons data all downloaded; 0 critical validation errors |
| Unit Tests | `test_hyg_parse`, `test_exoplanet_parse`, `test_horizons_parse` |
| QA Gate | Owner reviews first 10 rows of each dataset |

---

### EP-02 — Rendering Engine
**Phase**: 1 | **Track**: A
**Goal**: Star field, Solar System, skybox — all visuals for the simulation.
**Depends on**: EP-01

#### EP-02-S01 — Star Field
| Item | Detail |
|---|---|
| Goal | Render HYG stars as point sprites with correct spectral colour and magnitude size |
| Deliverables | `src/render/StarRenderer.cpp`, GLSL star shaders, spectral colour LUT |
| Acceptance | Stars render; colour matches spectral class chart; size scales with magnitude; FPS ≥ 60 |
| Unit Tests | `test_spectral_color`: each class O–M returns correct RGB range; `test_coord_conversion`: RA/Dec/dist ↔ XYZ round-trip |
| QA Gate | Owner inspects star field; recognisable constellations present |

#### EP-02-S02 — LOD Benchmarking
| Item | Detail |
|---|---|
| Goal | Test LOD strategies; implement chosen approach; quality tier toggle |
| Deliverables | Benchmark results doc; LOD implementation; Low/Medium/High tiers working |
| Acceptance | All 3 tiers produce measurable FPS difference; High ≥ 60fps on target hardware |
| Unit Tests | `test_lod_cull`: frustum culling reduces rendered count correctly |
| QA Gate | Owner tests all 3 tiers; confirms visual/performance trade-off acceptable |

#### EP-02-S03 — Skybox
| Item | Detail |
|---|---|
| Goal | NASA/ESA Milky Way panorama as skybox with procedural nebula overlay |
| Deliverables | `src/render/Skybox.cpp`, cubemap loader, nebula GLSL shader |
| Acceptance | Skybox renders at all orientations; no seams; nebula blends naturally |
| Unit Tests | `test_skybox_load`: cubemap loads without errors |
| QA Gate | Owner rotates 360° and confirms visual quality |

#### EP-02-S04 — Solar System
| Item | Detail |
|---|---|
| Goal | Sun, 8 planets, major moons, dwarf planets, orbital rings with NASA textures |
| Deliverables | `src/render/PlanetRenderer.cpp`, orbit ellipse renderer, ring system shader |
| Acceptance | All 8 planets at correct positions for a reference date; orbit rings correct; rings on Saturn/Uranus; atmosphere shaders on habitable planets |
| Unit Tests | `test_kepler`: ellipse from a/e/i matches reference; `test_planet_scale`: radii in expected range; `test_ephemeris`: positions match NASA for test date |
| QA Gate | Owner compares layout against NASA Eyes screenshot; orbits confirmed |

---

### EP-03 — Flight & Physics
**Phase**: 1 | **Track**: B
**Goal**: All three flight modes functional. No combat, no energy system.
**Depends on**: EP-01
**Note**: In Sandbox Mode the ship has unlimited propulsion — no energy management.

#### EP-03-S01 — Planetary Flight (Mode 3)
| Item | Detail |
|---|---|
| Goal | 6-DOF antigravity ship movement, damped controls, first-person camera, collision |
| Deliverables | `src/flight/Ship.cpp`, `src/flight/FlightController.cpp`, `src/core/Camera.cpp` (interface-backed) |
| Acceptance | Ship moves all 6 axes; smooth damping; no planet clip; camera abstraction in place for future third-person |
| Unit Tests | `test_ship_physics`: thrust → velocity; `test_damping`: converges to zero; `test_collision`: surface intersection |
| QA Gate | Owner flies around Earth 5 min; controls feel right; no clipping |

#### EP-03-S02 — System Warp (Mode 2)
| Item | Detail |
|---|---|
| Goal | In-system warp with damped curve, HUD ETA, lockout zone, minimap active |
| Deliverables | `src/flight/SystemWarp.cpp`, warp speed curve |
| Acceptance | Warp to any planet; no overshoot; lockout within gravity zone; ETA shown on HUD |
| Unit Tests | `test_warp_curve`: displacement integrates correctly; `test_warp_lockout`: blocked inside min radius |
| QA Gate | Owner warps Earth → Mars → Jupiter; feel and arrival position confirmed |

#### EP-03-S03 — Super Warp (Mode 1)
| Item | Detail |
|---|---|
| Goal | Interstellar warp to any HYG star with tunnel visual; galaxy map integration |
| Deliverables | `src/flight/SuperWarp.cpp`, warp tunnel GLSL shader |
| Acceptance | Select star on galaxy map → warp → arrive at outer boundary; tunnel ~3s; mode transition seamless |
| Unit Tests | `test_star_lookup`: name search returns correct HYG record; `test_mode_transition`: state machine correct |
| QA Gate | Owner warps to Alpha Centauri, Sirius, and a random distant star; tunnel acceptable |

---

### EP-05-SIM — HUD & Navigation (Simulation)
**Phase**: 1 | **Track**: C
**Goal**: Simulation HUD — speed, nearest body, minimap, galaxy map. No gameplay gauges.
**Depends on**: EP-01

#### EP-05-SIM-S01 — Core Simulation HUD
| Item | Detail |
|---|---|
| Goal | Flight mode indicator, speed readout, nearest star/planet info panel, alerts |
| Deliverables | `src/hud/HUD.cpp`, Dear ImGui HUD layout |
| Acceptance | All elements update in real time; units auto-scale; no overlap at 1080p and 1440p |
| Unit Tests | `test_nearest_star`: spatial query returns correct result; `test_hud_layout`: all elements within screen bounds |
| QA Gate | Owner flies and confirms HUD legibility and correctness |

#### EP-05-SIM-S02 — Solar System Minimap
| Item | Detail |
|---|---|
| Goal | Lower-left minimap with orbital rings, planet dots, ship X marker, distance to nearest |
| Deliverables | `src/hud/Minimap.cpp` |
| Acceptance | Minimap visible in Mode 2 and 3 only; ship position correct; inner-system zoom toggle works |
| Unit Tests | `test_minimap_position`: ship X placed on correct orbital ring |
| QA Gate | Owner warps between planets; minimap tracks correctly |

#### EP-05-SIM-S03 — Galaxy / Star Map
| Item | Detail |
|---|---|
| Goal | Full-screen star map with 10/50/100 LY filters, spectral colours, click-to-set-warp-target |
| Deliverables | `src/hud/GalaxyMap.cpp` |
| Acceptance | Filters work; stars coloured by spectral class; click sets Super Warp target; explored systems marked |
| Unit Tests | `test_galaxy_filter`: filter returns only stars within band; `test_warp_target_set`: click sets correct target |
| QA Gate | Owner browses map, sets target, warps — end-to-end confirmed |

---

### EP-06-SIM — Simulation Framework
**Phase**: 1 | **Track**: D
**Goal**: Main menu with mode select, settings, remappable controls, save position, time warp.
**Depends on**: EP-01

#### EP-06-SIM-S01 — Main Menu & Mode Select
| Item | Detail |
|---|---|
| Goal | Main menu: Sandbox Mode, Game Mode (locked in Phase 1), Settings, Quit |
| Deliverables | `src/core/GameStateMachine.cpp`, main menu UI |
| Acceptance | Sandbox Mode launches correctly; Game Mode shows "Coming Soon" or is locked; all navigation works |
| Unit Tests | `test_state_machine`: all menu → game transitions correct |
| QA Gate | Owner navigates all menus; confirms flow |

#### EP-06-SIM-S02 — Settings & Remappable Controls
| Item | Detail |
|---|---|
| Goal | Graphics quality, audio volume, fully remappable key bindings, all persisted |
| Deliverables | `src/core/Settings.cpp`, settings UI, `config/game.json` write/read |
| Acceptance | All bindings remappable; changes persist across sessions; graphics quality changes visible |
| Unit Tests | `test_settings_persist`: write → reload → assert equality |
| QA Gate | Owner remaps controls; adjusts quality; restarts app; confirms persistence |

#### EP-06-SIM-S03 — Save Position & Time Warp
| Item | Detail |
|---|---|
| Goal | Save/restore player position + orientation + in-game time; time warp control |
| Deliverables | `src/core/SaveSystem.cpp` (simulation subset), `src/core/TimeSystem.cpp` |
| Acceptance | Save/load restores position within epsilon; time warp 1x–10000x changes planet positions visibly; warp disengages during active flight warp |
| Unit Tests | `test_save_position`: position survives serialize/deserialize; `test_time_warp`: planet advances at 10000x |
| QA Gate | Owner saves, quits, reloads — position confirmed; time warp orbital motion confirmed |

---

### EP-12-P1 — Player Ship Asset
**Phase**: 1 | **Track**: F (independent)
**Goal**: At least one player ship 3D model ready for integration.
**Depends on**: Nothing — runs in parallel from project start.

#### EP-12-P1-S01 — Player Ship Model
| Item | Detail |
|---|---|
| Goal | 1+ player ship `.obj`/`.fbx` with UV textures, Blender source |
| Deliverables | Ship model files + textures |
| Acceptance | Loads in OpenGL; polygon count real-time appropriate; UV mapped |
| Unit Tests | `test_model_load`: mesh loads; vertex count within budget |
| QA Gate | Owner reviews ship model in test render |

---

## ╔══════════════════════════════════════╗
## ║  PHASE 1 QA MILESTONE               ║
## ║                                      ║
## ║  Full simulation playthrough by      ║
## ║  owner required before Phase 2       ║
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
## PHASE 2 — GAME LAYER
## ═══════════════════════════════

---

### EP-04 — Ship Systems
**Phase**: 2 | **Track**: A2
**Goal**: Energy pool, shields, hull damage, multi-ship config. Adds game mechanics to the simulation ship.
**Depends on**: Phase 1 complete

#### EP-04-S01 — Energy Pool & Allocation
| Item | Detail |
|---|---|
| Goal | Single shared energy pool with passive regen and manual allocation sliders |
| Deliverables | `src/ship/EnergySystem.cpp`, `src/ship/AllocationUI.cpp` |
| Acceptance | Energy drains/regens correctly; allocation sliders update per-system budgets in real time; warp lockout at low energy |
| Unit Tests | `test_energy_pool`: drain + regen over time; `test_allocation`: slider recalculates system budgets |
| QA Gate | Owner adjusts sliders; confirms energy depletion locks warp |

#### EP-04-S02 — Shield Bubble & Hull Damage
| Item | Detail |
|---|---|
| Goal | Shield renderer, progressive absorption, hull damage on shield collapse, game over trigger |
| Deliverables | `src/ship/ShieldSystem.cpp`, `src/render/ShieldRenderer.cpp`, `src/ship/DamageModel.cpp` |
| Acceptance | Shield bubble visible; opacity matches level; hull damaged only at 0% shield; game over at 0% hull; carried tech gifts lost on destruction |
| Unit Tests | `test_shield_absorption`; `test_hull_damage`; `test_game_over` |
| QA Gate | Owner drains shield deliberately; confirms hull damage; triggers game over |

#### EP-04-S03 — Multi-Ship Config
| Item | Detail |
|---|---|
| Goal | `config/ships.json` drives all ship definitions; 3 starter ships |
| Deliverables | `config/ships.json`, `src/ship/ShipConfig.cpp`, ship selector UI |
| Acceptance | All ships load from JSON; adding new ship requires no recompile; stat differences meaningful |
| Unit Tests | `test_ship_config_parse`; `test_ship_stats_apply` |
| QA Gate | Owner adds custom ship to config; confirms it appears in-game |

---

### EP-06-GAME — Game Framework Extensions
**Phase**: 2 | **Track**: A2 (continues from EP-04)
**Goal**: Extend simulation framework with full game states: ship selector, full save/load, game over, pause.
**Depends on**: EP-04

#### EP-06-GAME-S01 — Ship Selector & Game Mode Launch
| Item | Detail |
|---|---|
| Goal | Game Mode main menu path: ship selector → New Game → spawn in Earth orbit |
| Deliverables | Ship selector screen, Game Mode state |
| Acceptance | Game Mode accessible from main menu; ship selector shows all ships with stats; New Game spawns at Earth orbit facing Sun |
| Unit Tests | `test_spawn_position`: spawn within Earth orbit bounds |
| QA Gate | Owner launches Game Mode, selects ship, spawns — confirmed correct |

#### EP-06-GAME-S02 — Full Save / Load & Game Over
| Item | Detail |
|---|---|
| Goal | Save full game state (position, ship, energy, hull, reputation, tech tree, gifts, time, realm) |
| Deliverables | Extended `SaveSystem.cpp` with game-layer fields |
| Acceptance | All game-state fields survive save/load round-trip; game over → reload restores correctly; gifts lost on death not restored |
| Unit Tests | `test_full_save_roundtrip`; `test_gifts_lost_on_death` |
| QA Gate | Owner plays, saves, dies, reloads — full state verified |

#### EP-06-GAME-S03 — Pause Menu
| Item | Detail |
|---|---|
| Goal | In-game pause: Resume, Save, Tech Tree, Settings, Quit to Menu |
| Deliverables | Pause menu UI |
| Acceptance | Pause freezes simulation; all options functional; time warp disabled while paused |
| Unit Tests | `test_pause_state`: simulation time does not advance while paused |
| QA Gate | Owner pauses mid-flight; confirms all pause options |

---

### EP-07 — Combat & Sensors
**Phase**: 2 | **Track**: B2
**Goal**: Energy weapons, physics-based detection, red alert, subsystem damage, realm-crossing effect.
**Depends on**: Phase 1 complete

#### EP-07-S01 — Energy Weapons
| Item | Detail |
|---|---|
| Goal | Laser/plasma burst weapons drawing from energy Weapons budget |
| Deliverables | `src/combat/WeaponSystem.cpp`, projectile renderer |
| Acceptance | Weapons fire; energy depletes from weapons allocation; hits register |
| Unit Tests | `test_weapon_fire`: energy deducted; `test_projectile_hit`: collision callback fires |
| QA Gate | Owner fires at test target; confirms energy drain and visual |

#### EP-07-S02 — Physics-Based Detection
| Item | Detail |
|---|---|
| Goal | EM/heat/light detection model; weapons-hot → red alert only; sensor contact HUD alert |
| Deliverables | `src/combat/SensorSystem.cpp` |
| Acceptance | Enemy detected within range; red alert fires only on confirmed weapons hot; sensor contact shown on HUD |
| Unit Tests | `test_detection_range`; `test_red_alert_weapons_hot` |
| QA Gate | Owner approaches enemy; confirms detection timing and red alert trigger |

#### EP-07-S03 — Realm Crossing & Subsystem Damage
| Item | Detail |
|---|---|
| Goal | Star Trek shimmer effect on realm crossings; subsystem degradation at hull thresholds |
| Deliverables | `src/render/RealmCrossShader.glsl`, `src/ship/SubsystemDamage.cpp` |
| Acceptance | Shimmer effect plays on enemy materialisation; correct subsystem degrades at defined hull % |
| Unit Tests | `test_subsystem_damage`: correct system degrades at correct threshold |
| QA Gate | Owner observes enemy cross from Spirit Realm; observes subsystem failure sequence |

---

### EP-05-EXT — HUD Game Extensions
**Phase**: 2 | **Track**: C2
**Goal**: Add game-layer HUD elements on top of simulation HUD (energy gauges, combat alerts, realm indicator).
**Depends on**: EP-05-SIM complete, EP-04 interface available

#### EP-05-EXT-S01 — Energy & Combat HUD
| Item | Detail |
|---|---|
| Goal | Energy pool bar, per-subsystem allocation bars, shield %, hull %, realm indicator, red alert overlay |
| Deliverables | Extended `HUD.cpp` with game-layer panels |
| Acceptance | All gauges update in real time; red alert overlay visible; realm indicator switches correctly |
| Unit Tests | `test_hud_layout_game`: all game elements within screen bounds |
| QA Gate | Owner enters combat; confirms all gauges respond correctly |

#### EP-05-EXT-S02 — Alien Contact Panel
| Item | Detail |
|---|---|
| Goal | Side panel for Federation alien dialogue with race portrait and accept/decline options |
| Deliverables | `src/hud/AlienContactPanel.cpp` |
| Acceptance | Panel opens on contact; portrait loads when available; gift accept/decline functional |
| Unit Tests | `test_contact_panel_trigger`: Federation contact opens panel |
| QA Gate | Owner encounters alien; reviews panel UI and interactions |

---

### EP-08 — Alien Civilisations & Factions
**Phase**: 2 | **Track**: D2
**Goal**: Alien encounters, dialogue, reputation, intro narrative event.
**Depends on**: Phase 1 complete, EP-05-EXT interface available

#### EP-08-S01 — Alien Spawning
| Item | Detail |
|---|---|
| Goal | Alien ships at foreign star systems; 80/20 split; encounter detection |
| Deliverables | `src/alien/AlienSpawner.cpp`, `src/alien/FactionConfig.cpp` |
| Acceptance | Aliens appear only at non-Sol systems; frequency scales with exploration distance; 80/20 ratio within tolerance |
| Unit Tests | `test_faction_split`; `test_no_spawn_in_sol` |
| QA Gate | Owner explores 3 systems; confirms encounters and faction ratio |

#### EP-08-S02 — Dialogue & Tech Gift System
| Item | Detail |
|---|---|
| Goal | Text dialogue panel, race portrait, gift accept/decline, gift added to carried inventory |
| Deliverables | `src/alien/DialogueSystem.cpp` |
| Acceptance | Dialogue opens on Federation contact; gift accepted → inventory; declined → no effect |
| Unit Tests | `test_dialogue_trigger`; `test_gift_accept` |
| QA Gate | Owner accepts a gift and sees it in inventory |

#### EP-08-S03 — Reputation System
| Item | Detail |
|---|---|
| Goal | Per-race reputation, hostile state on attack, mission-based recovery |
| Deliverables | `src/alien/ReputationSystem.cpp`, stub mission framework |
| Acceptance | Attack → Hostile; Hostile race attacks on sight; mission complete → rep increments |
| Unit Tests | `test_reputation_attack`; `test_reputation_recovery` |
| QA Gate | Owner attacks Federation alien; observes hostility; completes recovery mission |

#### EP-08-S04 — Intro Narrative Event
| Item | Detail |
|---|---|
| Goal | Opening Spirit Realm alien contact event on New Game — introduces lore |
| Deliverables | `src/alien/IntroEvent.cpp`, intro dialogue |
| Acceptance | Event fires once on New Game; skippable; introduces Spirit Realm concept |
| Unit Tests | `test_intro_fires_once`: does not fire on Load |
| QA Gate | Owner plays intro; confirms lore hook is clear |

---

### EP-09 — Technology Progression
**Phase**: 2 | **Track**: D2 (continues from EP-08)
**Goal**: Tech tree, alien gift inventory, Earth R&D loop, permanent unlocks.
**Depends on**: EP-08

#### EP-09-S01 — Tech Tree Data & UI
| Item | Detail |
|---|---|
| Goal | 6-branch tech tree; UI accessible from pause and main menu |
| Deliverables | `src/tech/TechTree.cpp`, `src/hud/TechTreeUI.cpp` |
| Acceptance | All 6 branches visible; prerequisites gate nodes correctly; unlocked nodes visually distinct |
| Unit Tests | `test_tech_prereqs`: locked node cannot be unlocked without prerequisites |
| QA Gate | Owner browses tree; confirms layout clarity and logic |

#### EP-09-S02 — Gift Inventory & Earth R&D
| Item | Detail |
|---|---|
| Goal | Carried gift inventory; deposit at Earth/Sol; R&D conversion; permanent unlock |
| Deliverables | `src/tech/GiftInventory.cpp`, `src/tech/RnDSystem.cpp` |
| Acceptance | Gifts in inventory on accept; deposit at Sol queues R&D; unlock persists after death; carried gifts lost on death |
| Unit Tests | `test_gift_lost_on_death`; `test_rnd_persist` |
| QA Gate | Owner collects, deposits, dies, confirms deposit survived |

#### EP-09-S03 — Tech Applied to Ship Builds
| Item | Detail |
|---|---|
| Goal | New ships built at Earth reflect current tech tree unlocks |
| Deliverables | Integration between `TechTree` and `ShipConfig` |
| Acceptance | Unlocked tech node changes stats on next ship build; prior ship unaffected |
| Unit Tests | `test_tech_applies_to_build` |
| QA Gate | Owner unlocks propulsion node; builds new ship; confirms improved warp efficiency |

---

### EP-10 — Spirit Realm
**Phase**: 2 | **Track**: E2
**Goal**: Spirit Realm render layer, content, entry mechanics, crossing effect.
**Depends on**: EP-02 (render pipeline), EP-09 (tech for entry unlock)

#### EP-10-S01 — Realm Render Layer
| Item | Detail |
|---|---|
| Goal | Second render pass toggles Spirit Realm overlay on existing scene |
| Deliverables | `src/render/SpiritRealmRenderer.cpp`, realm overlay GLSL shaders |
| Acceptance | Toggling realm activates/deactivates overlay; normal scene unaffected; FPS regression < 10% |
| Unit Tests | `test_realm_toggle`: render pass count changes on toggle |
| QA Gate | Owner toggles realm in Earth orbit; layer appears and disappears cleanly |

#### EP-10-S02 — Spirit Realm Solar System Content
| Item | Detail |
|---|---|
| Goal | Earth city layer, planet energy hubs, star civilisation activity in Spirit Realm |
| Deliverables | Spirit Realm geometry, `src/world/SpiritContent.cpp` |
| Acceptance | Earth shows spirit city structures; planets show orbital hubs; stars show activity rings |
| Unit Tests | `test_spirit_content_load`: all objects load without errors |
| QA Gate | Owner enters Spirit Realm at Earth; reviews city layer, planetary hubs |

#### EP-10-S03 — Realm Entry & Transition Effect
| Item | Detail |
|---|---|
| Goal | Spirit Realm drive (tech-gated) and alien stargate entry; Star Trek shimmer on transition |
| Deliverables | `src/flight/RealmEntry.cpp`, shimmer GLSL shader |
| Acceptance | Entry requires drive unlock OR active stargate; shimmer plays on enter/exit; HUD realm indicator switches |
| Unit Tests | `test_realm_entry_gated`; `test_shimmer_plays` |
| QA Gate | Owner enters via stargate and via drive; shimmer and HUD indicator confirmed |

---

### EP-11 — Audio
**Phase**: 2 | **Track**: F2 (parallel with other Phase 2 tracks)
**Goal**: Full audio system.
**Depends on**: Phase 1 complete

#### EP-11-S01 — Audio Backend & Normal Space Ambient
| Item | Detail |
|---|---|
| Goal | Audio engine; ambient soundscape in normal space |
| Deliverables | `src/audio/AudioSystem.cpp` (OpenAL or miniaudio), ambient track |
| Acceptance | Ambient plays on game start; volume control works; no glitches |
| Unit Tests | `test_audio_init` |
| QA Gate | Owner confirms ambient audio |

#### EP-11-S02 — Flight & Combat Audio
| Item | Detail |
|---|---|
| Goal | Engine hum, warp cues, combat sounds (shield hit, hull hit, weapon fire) |
| Deliverables | `src/audio/FlightAudio.cpp`, `src/audio/CombatAudio.cpp`, sourced assets |
| Acceptance | All events have audio; engine hum varies with speed; warp cues distinct |
| Unit Tests | `test_audio_events`: correct sound for each event |
| QA Gate | Owner flies, warps, and fights; all audio cues confirmed |

#### EP-11-S03 — Spirit Realm Soundscape
| Item | Detail |
|---|---|
| Goal | Distinct Spirit Realm ambient; realm transition audio |
| Deliverables | Spirit Realm audio assets, transition sound |
| Acceptance | Ambient switches on realm toggle; transition has distinct audio effect |
| Unit Tests | `test_realm_audio_switch` |
| QA Gate | Owner enters and exits Spirit Realm; distinct soundscapes confirmed |

---

### EP-12-P2 — Game Layer Assets
**Phase**: 2 | **Track**: F2 (parallel, independent)
**Goal**: Alien ship models, race portraits, HUD icons.
**Depends on**: Nothing — runs in parallel, delivers to EP-08, EP-10

#### EP-12-P2-S01 — Alien Ship Models
| Item | Detail |
|---|---|
| Goal | 2+ alien ship models with distinct faction silhouettes |
| Deliverables | `.obj` + textures per faction type |
| Acceptance | Distinct silhouettes; poly count real-time appropriate |
| Unit Tests | `test_alien_model_load` |
| QA Gate | Owner reviews models — distinct and recognisable |

#### EP-12-P2-S02 — Race Portraits & HUD Icons
| Item | Detail |
|---|---|
| Goal | 3+ race portrait images (512×512); full HUD icon set |
| Deliverables | Portrait PNGs, icon SVG/PNG set |
| Acceptance | Portraits readable at dialogue panel size; icons consistent in style |
| Unit Tests | N/A |
| QA Gate | Owner reviews portraits and icon set for quality and consistency |

---

## 5. Phase 1 Full Sprint Sequence

| Sprint ID | Epic | Sprint Name |
|---|---|---|
| 1-1 | EP-01 | Build System & Repository |
| 1-2 | EP-01 | Window & OpenGL Context |
| 1-3 | EP-01 | Data Download & Validation |
| 1-4 | EP-02 | Star Field |
| 1-5 | EP-02 | LOD Benchmarking |
| 1-6 | EP-02 | Skybox |
| 1-7 | EP-02 | Solar System |
| 1-8 | EP-03 | Planetary Flight (Mode 3) |
| 1-9 | EP-03 | System Warp (Mode 2) |
| 1-10 | EP-03 | Super Warp (Mode 1) |
| 1-11 | EP-05-SIM | Core Simulation HUD |
| 1-12 | EP-05-SIM | Solar System Minimap |
| 1-13 | EP-05-SIM | Galaxy / Star Map |
| 1-14 | EP-06-SIM | Main Menu & Mode Select |
| 1-15 | EP-06-SIM | Settings & Remappable Controls |
| 1-16 | EP-06-SIM | Save Position & Time Warp |
| 1-17 | EP-12-P1 | Player Ship Model |
| **GATE** | — | **PHASE 1 QA MILESTONE — Owner sign-off** |

*Parallel tracks within Phase 1: EP-02 (A), EP-03 (B), EP-05-SIM (C), EP-06-SIM (D), EP-12-P1 (F) all run independently after EP-01 closes.*

---

## 6. Phase 2 Full Sprint Sequence

| Sprint ID | Epic | Sprint Name |
|---|---|---|
| 2-1 | EP-04 | Energy Pool & Allocation |
| 2-2 | EP-04 | Shield Bubble & Hull Damage |
| 2-3 | EP-04 | Multi-Ship Config |
| 2-4 | EP-06-GAME | Ship Selector & Game Mode Launch |
| 2-5 | EP-06-GAME | Full Save / Load & Game Over |
| 2-6 | EP-06-GAME | Pause Menu |
| 2-7 | EP-07 | Energy Weapons |
| 2-8 | EP-07 | Physics-Based Detection |
| 2-9 | EP-07 | Realm Crossing & Subsystem Damage |
| 2-10 | EP-05-EXT | Energy & Combat HUD |
| 2-11 | EP-05-EXT | Alien Contact Panel |
| 2-12 | EP-08 | Alien Spawning |
| 2-13 | EP-08 | Dialogue & Tech Gift System |
| 2-14 | EP-08 | Reputation System |
| 2-15 | EP-08 | Intro Narrative Event |
| 2-16 | EP-09 | Tech Tree Data & UI |
| 2-17 | EP-09 | Gift Inventory & Earth R&D |
| 2-18 | EP-09 | Tech Applied to Ship Builds |
| 2-19 | EP-10 | Realm Render Layer |
| 2-20 | EP-10 | Spirit Realm Solar System Content |
| 2-21 | EP-10 | Realm Entry & Transition Effect |
| 2-22 | EP-11 | Audio Backend & Normal Space Ambient |
| 2-23 | EP-11 | Flight & Combat Audio |
| 2-24 | EP-11 | Spirit Realm Soundscape |
| 2-25 | EP-12-P2 | Alien Ship Models |
| 2-26 | EP-12-P2 | Race Portraits & HUD Icons |
| 2-27 | — | Full integration polish, LOD tuning, QA pass |
| 2-28+ | — | Future: third-person camera, landing hook, satellite toggle |

*Parallel tracks within Phase 2: A2 (EP-04→EP-06-GAME), B2 (EP-07), C2 (EP-05-EXT), D2 (EP-08→EP-09), E2 (EP-10), F2 (EP-11, EP-12-P2) all run independently after Phase 1 gate.*

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
- [ ] Full exploration playthrough completed by owner
- [ ] All flight modes tested end-to-end
- [ ] Save/load confirmed
- [ ] Time warp orbital motion confirmed
- [ ] Owner formally signs off on simulation quality before Phase 2 begins

---

*All sprint work traces to a requirement in REQUIREMENTS.md. This document is the engineering baseline.*
