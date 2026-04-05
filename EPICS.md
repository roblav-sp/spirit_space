# Spirit Space — Epics & Sprint Plan

> Version: 1.0
> Date: 2026-04-04
> Status: Approved — Baseline
> Owner: Roberto Lavoie

---

## 1. Work Tracks Overview

Work is organised into **6 parallel tracks**. Each track is a stream of epics that can progress independently after the shared foundation (EP-01) is complete. A track is only blocked by its own prior epic — never by another track.

```
TRACK A — ENGINE & RENDERING
  EP-01 → EP-02 → EP-10

TRACK B — FLIGHT & COMBAT
  EP-01 → EP-03 → EP-07

TRACK C — SHIP SYSTEMS & GAME FRAMEWORK
  EP-01 → EP-04 → EP-06

TRACK D — UI, HUD & PROGRESSION
  EP-01 → EP-05 → EP-08 → EP-09

TRACK E — AUDIO
  EP-01 → EP-11

TRACK F — VISUAL ASSETS (fully independent after kickoff)
  EP-12 (starts in parallel with EP-02, delivers assets to other tracks)
```

**EP-01 is the only shared gate.** Once it closes, all other tracks proceed independently.

---

## 2. Dependency Map

```
EP-01 (Foundation)
├── EP-02 (Rendering)          → EP-10 (Spirit Realm)
├── EP-03 (Flight)             → EP-07 (Combat & Sensors)
├── EP-04 (Ship Systems)       → EP-06 (Game Framework)
├── EP-05 (HUD & Navigation)   → EP-08 (Alien Factions) → EP-09 (Tech Tree)
├── EP-11 (Audio)
└── EP-12 (Visual Assets) — independent; delivers to EP-02, EP-08, EP-10
```

Cross-track integration points (not blocking — interface-based):
- EP-07 (Combat) consumes ship energy API from EP-04
- EP-08 (Alien) consumes HUD panel API from EP-05
- EP-09 (Tech Tree) consumes ship stats API from EP-04
- EP-10 (Spirit Realm) consumes render pipeline from EP-02

All cross-track consumption is through defined interfaces. A track does not wait for another track — it codes to the interface and integrates when the other track delivers.

---

## 3. Epic Definitions

---

### EP-01 — Foundation
**Track**: All (shared gate)
**Goal**: Project infrastructure, build system, data pipeline, validated datasets, working window.
**Blocks**: All other epics.
**Requirements**: REQ-INFRA-001 through REQ-INFRA-010

#### Sprints

**EP-01-S01 — Build System & Repository**
| Item | Detail |
|---|---|
| Goal | CMake project, vcpkg, GitHub repo, CI pipeline green |
| Deliverables | `CMakeLists.txt`, `.github/workflows/ci.yml`, vcpkg manifest, GTest passing |
| Acceptance | `cmake --build` succeeds; CI green on push; empty test suite passes |
| Unit Tests | Smoke test: build artifact exists and executes |
| QA Gate | Owner confirms build on own machine; CI confirmed green |

**EP-01-S02 — Window & OpenGL Context**
| Item | Detail |
|---|---|
| Goal | GLFW window opens, GLAD loads OpenGL 4.5 context, triangle renders |
| Deliverables | `src/core/Window.cpp`, GLAD + GLFW integrated via vcpkg |
| Acceptance | 1920×1080 window opens; red triangle renders; app closes cleanly |
| Unit Tests | `test_window_context`: OpenGL version string ≥ 4.5 |
| QA Gate | Owner launches app and sees window |

**EP-01-S03 — Data Download & Validation**
| Item | Detail |
|---|---|
| Goal | Automated download scripts for all datasets; validation scripts pass |
| Deliverables | `scripts/download_data.py`, `scripts/validate_data.py` |
| Acceptance | HYG CSV downloaded (119k+ rows); exoplanet CSV downloaded; Horizons data downloaded; all validation scripts pass with 0 critical errors |
| Unit Tests | `test_hyg_parse`: 119k+ records; `test_exoplanet_parse`: required columns present; `test_horizons_parse`: positions extracted for all 8 planets |
| QA Gate | Owner reviews first 10 rows of each dataset; confirms content looks correct |

---

### EP-02 — Rendering Engine
**Track**: A
**Goal**: Full star field, Solar System visuals, and skybox.
**Depends on**: EP-01
**Requirements**: REQ-RENDER-001 through REQ-RENDER-018

#### Sprints

**EP-02-S01 — Star Field**
| Item | Detail |
|---|---|
| Goal | Render all HYG stars as point sprites with correct colour and size |
| Deliverables | `src/render/StarRenderer.cpp`, GLSL star shaders, spectral color LUT |
| Acceptance | Stars visible; colours match spectral class chart; magnitude scaling correct; FPS ≥ 60 |
| Unit Tests | `test_spectral_color`: each class O–M returns correct RGB range; `test_coord_conversion`: RA/Dec/dist → XYZ round-trip within epsilon |
| QA Gate | Owner inspects star field; recognisable constellations present; colour distribution correct |

**EP-02-S02 — LOD Benchmarking**
| Item | Detail |
|---|---|
| Goal | Test LOD strategies; select implementation; document result |
| Deliverables | Benchmark results doc; chosen strategy implemented; quality tier toggle working |
| Acceptance | Low/Medium/High tiers produce measurable FPS difference; High ≥ 60fps on target hardware |
| Unit Tests | `test_lod_cull`: frustum culling reduces rendered star count correctly |
| QA Gate | Owner tests all 3 quality tiers; confirms acceptable visual/performance trade-off |

**EP-02-S03 — Skybox**
| Item | Detail |
|---|---|
| Goal | NASA/ESA Milky Way panorama as skybox with procedural nebula overlay |
| Deliverables | `src/render/Skybox.cpp`, cubemap loader, nebula GLSL shader |
| Acceptance | Skybox renders at all camera orientations; no seams; nebula overlay blends naturally |
| Unit Tests | `test_skybox_load`: texture loaded without errors |
| QA Gate | Owner rotates view 360° and confirms visual quality |

**EP-02-S04 — Solar System Rendering**
| Item | Detail |
|---|---|
| Goal | Sun, 8 planets, major moons, orbital rings with NASA textures |
| Deliverables | `src/render/PlanetRenderer.cpp`, Keplerian orbit ellipse renderer, ring system shader |
| Acceptance | All 8 planets visible; positions match reference for a known date; orbit ellipses correct; rings on Saturn/Uranus |
| Unit Tests | `test_kepler`: orbit ellipse from a/e/i matches reference; `test_planet_scale`: visual radii in expected range |
| QA Gate | Owner compares layout against NASA Eyes screenshot; orbit paths confirmed |

---

### EP-03 — Flight & Physics
**Track**: B
**Goal**: All three flight modes fully functional with keyboard controls.
**Depends on**: EP-01
**Requirements**: REQ-FLIGHT-001 through REQ-FLIGHT-015

#### Sprints

**EP-03-S01 — Planetary Flight (Mode 3)**
| Item | Detail |
|---|---|
| Goal | 6-DOF antigravity ship movement, damped controls, camera, collision |
| Deliverables | `src/flight/Ship.cpp`, `src/flight/FlightController.cpp`, `src/core/Camera.cpp` (interface-backed) |
| Acceptance | Ship moves all 6 axes; smooth damping; no planet surface clip; FPS ≥ 60 |
| Unit Tests | `test_ship_physics`: thrust → velocity accumulation; `test_damping`: velocity converges to zero; `test_sphere_collision`: planet surface intersection |
| QA Gate | Owner flies around Earth for 5 min; controls feel right; damping acceptable |

**EP-03-S02 — System Warp (Mode 2)**
| Item | Detail |
|---|---|
| Goal | In-system warp with damped curve, HUD ETA display, lockout zone |
| Deliverables | `src/flight/SystemWarp.cpp`, warp curve implementation |
| Acceptance | Warp to any planet; no overshoot; lockout within gravity zone; ETA shown |
| Unit Tests | `test_warp_curve`: displacement integrates correctly; `test_warp_lockout`: blocked inside min radius; `test_energy_deduction`: correct cost for distance |
| QA Gate | Owner warps Earth → Mars → Jupiter; arrival position and feel confirmed |

**EP-03-S03 — Super Warp (Mode 1)**
| Item | Detail |
|---|---|
| Goal | Interstellar warp to any HYG star with tunnel visual |
| Deliverables | `src/flight/SuperWarp.cpp`, warp tunnel GLSL effect |
| Acceptance | Player selects star, warps, arrives at outer boundary; tunnel effect ~3s; energy depleted significantly |
| Unit Tests | `test_star_lookup`: name search returns correct HYG record; `test_super_warp_energy`: energy cost formula correct; `test_mode_transition`: state machine correct |
| QA Gate | Owner warps to Alpha Centauri, Sirius, random distant star; tunnel looks acceptable |

---

### EP-04 — Ship Systems
**Track**: C
**Goal**: Energy pool, shields, damage model, multi-ship config.
**Depends on**: EP-01
**Requirements**: REQ-SHIP-001 through REQ-SHIP-014

#### Sprints

**EP-04-S01 — Energy Pool & Allocation**
| Item | Detail |
|---|---|
| Goal | Single energy pool with passive regen and manual allocation sliders |
| Deliverables | `src/ship/EnergySystem.cpp`, `src/ship/AllocationUI.cpp` |
| Acceptance | Energy drains/regens correctly; allocation sliders update per-system budgets in real time |
| Unit Tests | `test_energy_pool`: drain + regen over time; `test_allocation`: slider recalculates budgets |
| QA Gate | Owner adjusts sliders; confirms real-time response |

**EP-04-S02 — Shield Bubble & Damage**
| Item | Detail |
|---|---|
| Goal | Shield renderer, impact absorption, hull damage model, game-over trigger |
| Deliverables | `src/ship/ShieldSystem.cpp`, `src/render/ShieldRenderer.cpp`, `src/ship/DamageModel.cpp` |
| Acceptance | Shield bubble visible; opacity matches level; impacts drain shield; hull damaged at 0% shield; game over at 0% hull |
| Unit Tests | `test_shield_absorption`: impact drains correct energy; `test_hull_damage`: accrues only at 0% shield; `test_game_over`: triggers at 0% hull |
| QA Gate | Owner drains shield deliberately; observes hull damage; triggers game over |

**EP-04-S03 — Multi-Ship Config**
| Item | Detail |
|---|---|
| Goal | `config/ships.json` drives all ship definitions; ship selector screen |
| Deliverables | `config/ships.json` (3 ships), `src/ship/ShipConfig.cpp`, ship selector UI |
| Acceptance | All ships load from JSON; new ship added to JSON appears in game with no recompile; stat differences are meaningful |
| Unit Tests | `test_ship_config_parse`: required fields present; `test_ship_stats_apply`: selected ship populates systems correctly |
| QA Gate | Owner adds a custom ship to config manually; confirms it appears and plays differently |

---

### EP-05 — HUD & Navigation
**Track**: D
**Goal**: All HUD elements, solar minimap, galaxy map.
**Depends on**: EP-01
**Requirements**: REQ-HUD-001 through REQ-HUD-016

#### Sprints

**EP-05-S01 — Core HUD**
| Item | Detail |
|---|---|
| Goal | Flight mode, speed, nearest body, energy/shield/hull gauges, alert area |
| Deliverables | `src/hud/HUD.cpp`, Dear ImGui HUD layout |
| Acceptance | All gauges update in real time; correct units auto-scale; alerts show/hide correctly; no overlap at 1080p and 1440p |
| Unit Tests | `test_nearest_star`: spatial query returns correct result; `test_hud_layout`: elements within screen bounds |
| QA Gate | Owner inspects HUD while flying; all elements legible and correctly positioned |

**EP-05-S02 — Solar System Minimap**
| Item | Detail |
|---|---|
| Goal | Lower-left minimap with orbital rings, planet dots, ship X marker, distance label |
| Deliverables | `src/hud/Minimap.cpp` |
| Acceptance | Minimap visible in Mode 2 and 3 only; ship position correct; inner-system zoom toggle works |
| Unit Tests | `test_minimap_position`: ship X placed on correct orbital ring for test positions |
| QA Gate | Owner warps between planets and confirms minimap tracks correctly |

**EP-05-S03 — Galaxy / Star Map**
| Item | Detail |
|---|---|
| Goal | Full-screen star map overlay with LY filters, spectral colors, click-to-warp-target |
| Deliverables | `src/hud/GalaxyMap.cpp` |
| Acceptance | 10/50/100 LY filters work; stars coloured by spectral class; clicking star sets Super Warp target; explored systems marked |
| Unit Tests | `test_galaxy_filter`: filter returns only stars within distance band; `test_warp_target_set`: click sets correct target |
| QA Gate | Owner browses galaxy map; sets a warp target; confirms it works end-to-end |

---

### EP-06 — Game Framework
**Track**: C (continues from EP-04)
**Goal**: Main menu, pause, save/load, game over, settings, time system.
**Depends on**: EP-04
**Requirements**: REQ-GAME-001 through REQ-GAME-014

#### Sprints

**EP-06-S01 — Game State Machine**
| Item | Detail |
|---|---|
| Goal | Main menu, pause menu, game over screen, state transitions |
| Deliverables | `src/core/GameStateMachine.cpp`, main menu UI, pause UI, game over UI |
| Acceptance | All states reachable; transitions correct; no state leaks |
| Unit Tests | `test_state_machine`: all transitions tested for correctness |
| QA Gate | Owner navigates all menus; confirms no dead ends |

**EP-06-S02 — Save / Load System**
| Item | Detail |
|---|---|
| Goal | Save and restore full game state to JSON; multiple save slots |
| Deliverables | `src/core/SaveSystem.cpp`, save file schema |
| Acceptance | Save/load round-trip preserves all fields; game over → load restores correctly; multiple slots work |
| Unit Tests | `test_save_roundtrip`: all fields survive serialize/deserialize; `test_save_corruption`: malformed file handled gracefully |
| QA Gate | Owner plays, saves, quits, reloads — verifies full state restoration |

**EP-06-S03 — Settings & Time System**
| Item | Detail |
|---|---|
| Goal | Remappable controls, graphics/audio settings, time warp control |
| Deliverables | `src/core/Settings.cpp`, `src/core/TimeSystem.cpp`, settings UI |
| Acceptance | Settings persist to `config/game.json`; all bindings remappable; time warp 1x–10000x changes planet positions visibly |
| Unit Tests | `test_settings_persist`: write and reload settings; `test_time_warp`: planet position advances correctly at 10000x |
| QA Gate | Owner remaps controls; adjusts time warp; confirms orbital motion visible at high speed |

---

### EP-07 — Combat & Sensors
**Track**: B (continues from EP-03)
**Goal**: Energy weapons, physics-based detection, red alert, realm-crossing effect.
**Depends on**: EP-03
**Requirements**: REQ-COMBAT-001 through REQ-COMBAT-010

#### Sprints

**EP-07-S01 — Energy Weapons**
| Item | Detail |
|---|---|
| Goal | Laser/plasma burst weapons drawing from energy pool |
| Deliverables | `src/combat/WeaponSystem.cpp`, weapon projectile renderer |
| Acceptance | Weapons fire; energy depletes from weapons budget; hits register on targets |
| Unit Tests | `test_weapon_fire`: energy deducted correctly; `test_projectile_hit`: collision detection fires callback |
| QA Gate | Owner fires weapons at a test target; confirms energy drain and visual |

**EP-07-S02 — Physics-Based Detection**
| Item | Detail |
|---|---|
| Goal | EM/heat/light detection model; weapons-hot red alert; sensor contact HUD alert |
| Deliverables | `src/combat/SensorSystem.cpp` |
| Acceptance | Enemy detected within range; red alert only fires when weapons are confirmed hot; alert appears on HUD |
| Unit Tests | `test_detection_range`: ship detected within range, not outside; `test_red_alert`: only triggers on weapons-hot |
| QA Gate | Owner approaches enemy ship; confirms detection behaviour and red alert trigger |

**EP-07-S03 — Realm Crossing Effect & Subsystem Damage**
| Item | Detail |
|---|---|
| Goal | Star Trek shimmer effect for realm crossings; subsystem damage at hull thresholds |
| Deliverables | `src/render/RealmCrossShader.glsl`, `src/ship/SubsystemDamage.cpp` |
| Acceptance | Crossing shimmer effect plays on enemy materialisation; subsystem degradation triggers at correct hull % |
| Unit Tests | `test_subsystem_damage`: correct subsystem degrades at correct hull threshold |
| QA Gate | Owner observes enemy materialize from Spirit Realm; observes subsystem failure at low hull |

---

### EP-08 — Alien Civilisations & Factions
**Track**: D (continues from EP-05)
**Goal**: Alien encounters, dialogue, reputation system, game intro contact event.
**Depends on**: EP-05
**Requirements**: REQ-ALIEN-001 through REQ-ALIEN-012

#### Sprints

**EP-08-S01 — Alien Spawning & Encounter Trigger**
| Item | Detail |
|---|---|
| Goal | Alien ships spawn at foreign star systems; 80/20 faction split; encounter detection |
| Deliverables | `src/alien/AlienSpawner.cpp`, `src/alien/FactionConfig.cpp` |
| Acceptance | Aliens appear at non-Sol stars; frequency increases with distance; 80% Federation, 20% aggressive |
| Unit Tests | `test_faction_split`: over N spawns, ratio within tolerance of 80/20; `test_spawn_zone`: no spawns in Sol system |
| QA Gate | Owner explores 3 foreign systems; confirms alien encounters and faction split |

**EP-08-S02 — Dialogue & Tech Gift System**
| Item | Detail |
|---|---|
| Goal | Text dialogue panel with optional portrait; tech gift accept/decline |
| Deliverables | `src/alien/DialogueSystem.cpp`, `src/hud/AlienContactPanel.cpp` |
| Acceptance | Dialogue panel opens on Federation contact; race portrait loads if available; gift can be accepted/declined; gift added to carried inventory |
| Unit Tests | `test_dialogue_trigger`: contact with Federation ship opens panel; `test_gift_accept`: gift added to inventory |
| QA Gate | Owner encounters Federation alien; reviews dialogue UI; accepts a gift |

**EP-08-S03 — Reputation System**
| Item | Detail |
|---|---|
| Goal | Per-race reputation tracking, hostile state, mission-based recovery |
| Deliverables | `src/alien/ReputationSystem.cpp`, stub mission framework |
| Acceptance | Attacking Federation ship sets race to Hostile; Hostile race attacks on sight; mission completion restores reputation |
| Unit Tests | `test_reputation_attack`: attack → hostile state; `test_reputation_recovery`: mission complete → reputation increments |
| QA Gate | Owner attacks a Federation alien; observes hostility; completes recovery mission |

**EP-08-S04 — Game Start Narrative Event**
| Item | Detail |
|---|---|
| Goal | Opening Spirit Realm alien contact event introducing the lore |
| Deliverables | `src/alien/IntroEvent.cpp`, intro dialogue content |
| Acceptance | Event triggers on New Game; dialogue introduces Spirit Realm concept; skippable |
| Unit Tests | `test_intro_trigger`: fires once on new game, not on load |
| QA Gate | Owner plays through intro event; confirms tone and clarity of lore introduction |

---

### EP-09 — Technology Progression
**Track**: D (continues from EP-08)
**Goal**: Tech tree, Earth R&D loop, permanent unlocks, new ship construction.
**Depends on**: EP-08
**Requirements**: REQ-TECH-001 through REQ-TECH-010

#### Sprints

**EP-09-S01 — Tech Tree Data & UI**
| Item | Detail |
|---|---|
| Goal | Tech tree structure (6 branches), UI accessible from menu and pause screen |
| Deliverables | `src/tech/TechTree.cpp`, `src/hud/TechTreeUI.cpp`, tech tree data definition |
| Acceptance | All 6 branches visible; prerequisites correctly gate nodes; unlocked nodes visually distinct |
| Unit Tests | `test_tech_prereqs`: node without met prerequisites cannot be unlocked |
| QA Gate | Owner browses tech tree; confirms layout clarity and branch logic |

**EP-09-S02 — Alien Gift Inventory & Earth R&D**
| Item | Detail |
|---|---|
| Goal | Carried gift inventory; deposit at Earth; convert to R&D progress; permanent unlock |
| Deliverables | `src/tech/GiftInventory.cpp`, `src/tech/RnDSystem.cpp` |
| Acceptance | Gifts appear in inventory on accept; depositing at Earth queues R&D; unlock completes after cost met; unlocks survive death |
| Unit Tests | `test_gift_lost_on_death`: carried gifts cleared on hull = 0%; `test_rnd_persist`: deposited R&D survives save/load/death |
| QA Gate | Owner collects gifts, deposits at Earth, unlocks a tech node, dies, confirms deposit survived |

**EP-09-S03 — Tech Applying to Ships**
| Item | Detail |
|---|---|
| Goal | New ship builds at Earth incorporate unlocked tech nodes |
| Deliverables | Integration between `TechTree` and `ShipConfig` |
| Acceptance | Unlocking a tech node changes stats on next ship build; already-built ship unaffected until rebuilt |
| Unit Tests | `test_tech_applies_to_build`: unlocked sensor node increases sensor range on new build |
| QA Gate | Owner unlocks propulsion tech; builds new ship; confirms improved warp efficiency |

---

### EP-10 — Spirit Realm
**Track**: A (continues from EP-02)
**Goal**: Spirit Realm render layer, entry mechanics, crossing effect, realm content.
**Depends on**: EP-02 (and EP-09 for entry tech unlock)
**Requirements**: REQ-SPIRIT-001 through REQ-SPIRIT-011

#### Sprints

**EP-10-S01 — Realm Render Layer**
| Item | Detail |
|---|---|
| Goal | Second render pass activates Spirit Realm visual overlay on existing scene |
| Deliverables | `src/render/SpiritRealmRenderer.cpp`, realm overlay GLSL shaders |
| Acceptance | Toggling realm switches second pass on/off; normal space scene unaffected; no FPS regression > 10% |
| Unit Tests | `test_realm_toggle`: render pass count changes on toggle |
| QA Gate | Owner toggles realm in Earth orbit; visual layer appears and disappears cleanly |

**EP-10-S02 — Spirit Realm Content (Solar System)**
| Item | Detail |
|---|---|
| Goal | Earth city layer, planet energy hubs, star civilisation activity in Spirit Realm |
| Deliverables | Spirit Realm geometry assets, `src/world/SpiritContent.cpp` |
| Acceptance | Earth shows city structures in Spirit Realm; all planets show orbital hubs; stars show activity |
| Unit Tests | `test_spirit_content_load`: all content objects load without errors |
| QA Gate | Owner enters Spirit Realm at Earth; reviews Earth city layer and planetary structures |

**EP-10-S03 — Realm Entry & Transition Effect**
| Item | Detail |
|---|---|
| Goal | Spirit Realm drive (tech-gated) and stargate entry; shimmer transition shader |
| Deliverables | `src/flight/RealmEntry.cpp`, shimmer transition GLSL shader |
| Acceptance | Entry requires tech unlock OR active stargate; shimmer plays on entry/exit; HUD indicator switches |
| Unit Tests | `test_realm_entry_gated`: entry blocked without tech unlock; `test_shimmer_shader`: effect plays and completes |
| QA Gate | Owner enters Spirit Realm via stargate and via drive; shimmer effect reviewed; HUD indicator confirms |

---

### EP-11 — Audio
**Track**: E
**Goal**: Full audio system with ambient, engine, warp, combat, and Spirit Realm sounds.
**Depends on**: EP-01
**Requirements**: REQ-AUDIO-001 through REQ-AUDIO-008

#### Sprints

**EP-11-S01 — Audio Backend & Ambient**
| Item | Detail |
|---|---|
| Goal | Audio engine integrated; normal space ambient soundscape playing |
| Deliverables | `src/audio/AudioSystem.cpp` (OpenAL or miniaudio backend), ambient track |
| Acceptance | Ambient plays on game start; volume control works; no audio glitches |
| Unit Tests | `test_audio_init`: backend initialises without error |
| QA Gate | Owner confirms ambient audio in normal space |

**EP-11-S02 — Flight & Combat Audio**
| Item | Detail |
|---|---|
| Goal | Engine hum, warp audio cues, combat sounds |
| Deliverables | Audio assets sourced + integrated; `src/audio/FlightAudio.cpp`, `src/audio/CombatAudio.cpp` |
| Acceptance | Engine hum varies with speed; warp charge/exit distinct; shield hit, hull hit, weapon fire all have audio |
| Unit Tests | `test_audio_events`: correct sound triggered for each game event |
| QA Gate | Owner flies, warps, and fires weapons; confirms all audio cues present |

**EP-11-S03 — Spirit Realm Soundscape**
| Item | Detail |
|---|---|
| Goal | Distinct Spirit Realm ambient; realm transition audio effect |
| Deliverables | Spirit Realm audio assets, realm transition sound |
| Acceptance | Realm ambient switches when entering Spirit Realm; transition has distinct audio |
| Unit Tests | `test_realm_audio_switch`: audio source swaps on realm toggle |
| QA Gate | Owner enters and exits Spirit Realm; confirms distinct soundscapes |

---

### EP-12 — Visual Assets Pipeline
**Track**: F (fully independent — runs in parallel from project start)
**Goal**: Produce all visual assets needed by other epics.
**Depends on**: Nothing (produces assets consumed by EP-02, EP-08, EP-10)
**Requirements**: REQ-ASSETS-001 through REQ-ASSETS-006

#### Sprints

**EP-12-S01 — Player Ship Model**
| Item | Detail |
|---|---|
| Goal | At least 1 player ship 3D model ready for integration |
| Deliverables | `.obj` or `.fbx` ship model(s) + textures, Blender source file |
| Acceptance | Model loads in OpenGL; polygon count appropriate for real-time; UV mapped |
| Unit Tests | `test_model_load`: mesh loads without errors, vertex count within budget |
| QA Gate | Owner reviews ship model in a test render |

**EP-12-S02 — Alien Ship Models & Portraits**
| Item | Detail |
|---|---|
| Goal | Alien ship models (2+ faction types) and race portrait images (3+ races) |
| Deliverables | Alien ship `.obj` files, portrait PNG images (512×512) |
| Acceptance | Distinct silhouettes per faction; portraits readable at dialogue panel size |
| Unit Tests | `test_alien_model_load`: all models load without errors |
| QA Gate | Owner reviews alien ship models and portraits for visual quality |

**EP-12-S03 — HUD Icons & UI Elements**
| Item | Detail |
|---|---|
| Goal | Consistent icon set for HUD, menus, and tech tree |
| Deliverables | SVG/PNG icon set |
| Acceptance | All icons consistent in style; readable at HUD display sizes |
| Unit Tests | N/A (visual QA only) |
| QA Gate | Owner reviews full icon set for consistency and clarity |

---

## 4. Full Sprint Schedule (Sequential within each track)

| Track | Sprint Sequence |
|---|---|
| **A: Engine/Rendering** | EP-01-S01 → EP-01-S02 → EP-01-S03 → EP-02-S01 → EP-02-S02 → EP-02-S03 → EP-02-S04 → EP-10-S01 → EP-10-S02 → EP-10-S03 |
| **B: Flight/Combat** | EP-01 (gate) → EP-03-S01 → EP-03-S02 → EP-03-S03 → EP-07-S01 → EP-07-S02 → EP-07-S03 |
| **C: Ship/Framework** | EP-01 (gate) → EP-04-S01 → EP-04-S02 → EP-04-S03 → EP-06-S01 → EP-06-S02 → EP-06-S03 |
| **D: HUD/Progression** | EP-01 (gate) → EP-05-S01 → EP-05-S02 → EP-05-S03 → EP-08-S01 → EP-08-S02 → EP-08-S03 → EP-08-S04 → EP-09-S01 → EP-09-S02 → EP-09-S03 |
| **E: Audio** | EP-01 (gate) → EP-11-S01 → EP-11-S02 → EP-11-S03 |
| **F: Assets** | EP-12-S01 → EP-12-S02 → EP-12-S03 (runs in parallel from day 1) |

---

## 5. Integration Milestones

These are points where parallel tracks merge for integration testing. Not blocking — integration is interface-based.

| Milestone | When | Tracks Merging |
|---|---|---|
| **INT-01: Flying in space with stars** | After EP-02-S01 + EP-03-S01 | A + B |
| **INT-02: Ship energy visible in HUD** | After EP-04-S01 + EP-05-S01 | C + D |
| **INT-03: Combat in full world** | After EP-07-S01 + EP-02-S04 + EP-04-S02 | A + B + C |
| **INT-04: Alien contact with gifts** | After EP-08-S02 + EP-09-S01 + EP-05-S01 | D complete |
| **INT-05: Full game loop** | After EP-06-S02 + all tracks at base | All tracks |
| **INT-06: Spirit Realm playable** | After EP-10-S03 + EP-09-S03 + EP-07-S03 | A + B + D |

---

## 6. QA Audit Protocol

Each sprint ends with a QA audit gate. **No next sprint begins until the gate is cleared.**

QA audit checklist per sprint:
- [ ] All acceptance criteria checked off
- [ ] All unit tests passing on CI
- [ ] Owner has run the build on their machine
- [ ] Owner has manually tested the sprint deliverables
- [ ] No P0 regressions introduced
- [ ] Owner sign-off recorded in sprint notes

---

*This document is the engineering baseline. All sprint work traces to a requirement in REQUIREMENTS.md.*
