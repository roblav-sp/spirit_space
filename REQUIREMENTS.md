# Spirit Space — Product Requirements Document (PRD)

> Version: 1.0
> Date: 2026-04-04
> Status: Approved — Baseline
> Owner: Roberto Lavoie

---

## 1. Document Purpose

This document defines the complete functional and non-functional requirements for **Spirit Space**. It serves as the contractual baseline between design intent and engineering delivery. All sprint acceptance criteria must trace back to a requirement in this document.

---

## 2. Product Overview

Spirit Space is a first-person space simulation game rendered from real astronomical data. The player pilots an antigravity ship through the observable universe and, through technological progression, gains access to the Spirit Realm — a parallel dimension overlaid on normal space. The game features physics-based detection, faction diplomacy, a technology progression tree, and an emergent energy management system.

---

## 3. Requirement Conventions

| Field | Description |
|---|---|
| **ID** | Unique identifier — `REQ-[DOMAIN]-[NNN]` |
| **Priority** | P0 = must ship / P1 = should ship / P2 = nice to have |
| **Epic** | Parent epic — see `EPICS.md` |
| **Status** | Draft / Approved / Deferred |

Priority definitions:
- **P0** — Blocking. Game cannot ship without this.
- **P1** — Core feature. Significant degradation if missing.
- **P2** — Enhancement. Deferrable to a future release.

---

## 4. Functional Requirements

---

### 4.1 Infrastructure & Data Pipeline (Domain: INFRA)

| ID | Requirement | Priority | Epic |
|---|---|---|---|
| REQ-INFRA-001 | The build system shall use CMake 3.20+ with vcpkg for dependency management | P0 | EP-01 |
| REQ-INFRA-002 | The project shall build successfully on Windows 10/11 with MSVC or MinGW | P0 | EP-01 |
| REQ-INFRA-003 | A CI pipeline (GitHub Actions) shall build and run all unit tests on every push to main | P0 | EP-01 |
| REQ-INFRA-004 | All third-party data shall be downloadable via automated scripts with no manual steps | P0 | EP-01 |
| REQ-INFRA-005 | Downloaded data shall be validated by schema, null checks, and outlier detection before use | P0 | EP-01 |
| REQ-INFRA-006 | The HYG star database (119,614+ records) shall be downloaded and parsed at build time | P0 | EP-01 |
| REQ-INFRA-007 | The NASA Exoplanet Archive catalog shall be downloaded and parsed at build time | P0 | EP-01 |
| REQ-INFRA-008 | NASA/JPL Horizons ephemeris data shall be downloaded for all Solar System bodies | P0 | EP-01 |
| REQ-INFRA-009 | All configuration shall reside in `config/game.json` and `config/ships.json` | P0 | EP-01 |
| REQ-INFRA-010 | The application shall log errors and warnings to a rotating log file | P1 | EP-01 |

---

### 4.2 Rendering Engine (Domain: RENDER)

| ID | Requirement | Priority | Epic |
|---|---|---|---|
| REQ-RENDER-001 | The application shall use OpenGL 4.5 core profile with Direct State Access (DSA) | P0 | EP-02 |
| REQ-RENDER-002 | The application shall target 60 FPS minimum on mid-range hardware at 1080p | P0 | EP-02 |
| REQ-RENDER-003 | The application shall support 1920×1080 resolution with fullscreen toggle | P0 | EP-02 |
| REQ-RENDER-004 | In-game quality setting (Low / Medium / High) shall adjust render load without restart | P0 | EP-02 |
| REQ-RENDER-005 | All HYG stars within 100 LY shall be rendered as point sprites with correct spectral color | P0 | EP-02 |
| REQ-RENDER-006 | Star color shall be derived from spectral class (O/B/A/F/G/K/M) via lookup table | P0 | EP-02 |
| REQ-RENDER-007 | Star apparent size shall scale with absolute magnitude | P0 | EP-02 |
| REQ-RENDER-008 | Stars brighter than magnitude 3 shall render with a bloom/glow effect | P1 | EP-02 |
| REQ-RENDER-009 | Binary and multiple star systems shall render each component as a separate object | P0 | EP-02 |
| REQ-RENDER-010 | The skybox shall use a NASA/ESA Milky Way panoramic image as the base layer | P0 | EP-02 |
| REQ-RENDER-011 | A procedural noise-based nebula/dust shader shall be layered over the skybox | P1 | EP-02 |
| REQ-RENDER-012 | All 8 planets shall render with real textures from NASA Solar System Exploration | P0 | EP-02 |
| REQ-RENDER-013 | Planet orbital paths shall be rendered as ellipses from Keplerian elements | P0 | EP-02 |
| REQ-RENDER-014 | Gas giant planets shall render procedural band shaders layered on textures | P1 | EP-02 |
| REQ-RENDER-015 | Habitable-zone planets shall render an atmospheric scattering shader | P1 | EP-02 |
| REQ-RENDER-016 | Saturn and Uranus ring systems shall be rendered from real ring data | P1 | EP-02 |
| REQ-RENDER-017 | All major moons shall be rendered with appropriate textures | P0 | EP-02 |
| REQ-RENDER-018 | LOD strategy shall be benchmarked in Sprint 1 before final implementation | P0 | EP-02 |
| REQ-RENDER-019 | The renderer shall support a Spirit Realm render pass as a scene overlay | P0 | EP-10 |
| REQ-RENDER-020 | Spirit Realm structures shall have their own LOD quality tier | P1 | EP-10 |

---

### 4.3 Flight & Physics (Domain: FLIGHT)

| ID | Requirement | Priority | Epic |
|---|---|---|---|
| REQ-FLIGHT-001 | The ship shall support three distinct flight modes: Planetary (M3), System Warp (M2), Interstellar Warp (M1) | P0 | EP-03 |
| REQ-FLIGHT-002 | Mode 3 shall provide 6-DOF antigravity movement with no gravity force applied to the ship | P0 | EP-03 |
| REQ-FLIGHT-003 | Mode 3 controls: WASD (forward/back/strafe), QE (roll), RF (up/down), mouse look | P0 | EP-03 |
| REQ-FLIGHT-004 | Mode 3 shall use configurable damped acceleration (dead-zone and response curve via config) | P0 | EP-03 |
| REQ-FLIGHT-005 | Mode 2 shall execute a damped warp curve: acceleration → cruise → guaranteed stop at target | P0 | EP-03 |
| REQ-FLIGHT-006 | Mode 2 shall display ETA and estimated energy cost on HUD before and during warp | P0 | EP-03 |
| REQ-FLIGHT-007 | Mode 2 warp shall lock out if the ship is within a planet's gravity influence zone | P0 | EP-03 |
| REQ-FLIGHT-008 | Mode 1 shall allow the player to select a target star and warp to its outer system boundary | P0 | EP-03 |
| REQ-FLIGHT-009 | Mode 1 shall play a warp tunnel visual effect lasting approximately 3 seconds | P1 | EP-03 |
| REQ-FLIGHT-010 | Mode 1 shall not engage if ship energy is below the minimum warp threshold | P0 | EP-03 |
| REQ-FLIGHT-011 | When the ship enters a planet's gravitational influence zone, the HUD shall display a proximity warning | P0 | EP-03 |
| REQ-FLIGHT-012 | The ship shall not clip through planet surfaces — sphere collision required | P0 | EP-03 |
| REQ-FLIGHT-013 | Ship speed readout shall auto-scale units: m/s, km/s, AU/s, LY/s based on mode | P0 | EP-03 |
| REQ-FLIGHT-014 | Camera shall be first-person (cockpit view) for all current flight modes | P0 | EP-03 |
| REQ-FLIGHT-015 | Camera system shall be abstracted behind an interface to support future third-person view | P0 | EP-03 |

---

### 4.4 Ship Systems (Domain: SHIP)

| ID | Requirement | Priority | Epic |
|---|---|---|---|
| REQ-SHIP-001 | All ship definitions shall be data-driven via `config/ships.json` — no recompile for new ships | P0 | EP-04 |
| REQ-SHIP-002 | Ships shall have the following configurable stats: Max Energy, Regen Rate, Warp Efficiency, Shield Strength, Weapon Power, Agility, Sensor Range, Tech Slots | P0 | EP-04 |
| REQ-SHIP-003 | The player shall have a single shared energy pool consumed by all systems | P0 | EP-04 |
| REQ-SHIP-004 | Energy allocation between Shield, Weapons, and Warp Reserve shall be manually adjustable via sliders | P0 | EP-04 |
| REQ-SHIP-005 | Energy shall regenerate passively at the ship's configured regen rate | P0 | EP-04 |
| REQ-SHIP-006 | The shield bubble shall deflect debris and projectiles; effectiveness scales with energy allocation | P0 | EP-04 |
| REQ-SHIP-007 | The shield bubble shall be rendered as a translucent sphere with opacity proportional to shield level | P0 | EP-04 |
| REQ-SHIP-008 | Shields shall absorb multiple hits progressively before depletion (Star Trek model) | P0 | EP-04 |
| REQ-SHIP-009 | Hull integrity shall track from 100% (undamaged) to 0% (destroyed) | P0 | EP-04 |
| REQ-SHIP-010 | Hull damage shall only accrue when shields are at 0% | P0 | EP-04 |
| REQ-SHIP-011 | Ship subsystems shall degrade at defined hull thresholds (warp, shields, sensors) | P1 | EP-04 |
| REQ-SHIP-012 | At hull 0% the Game Over state shall trigger; all carried tech gifts shall be lost | P0 | EP-04 |
| REQ-SHIP-013 | All ship stats shall be readable from `config/ships.json` at runtime | P0 | EP-04 |
| REQ-SHIP-014 | A minimum of 3 ship types shall be defined in initial config (balanced, explorer, combat) | P1 | EP-04 |

---

### 4.5 Navigation & HUD (Domain: HUD)

| ID | Requirement | Priority | Epic |
|---|---|---|---|
| REQ-HUD-001 | The HUD shall display the current flight mode and realm (Normal / Spirit Realm) | P0 | EP-05 |
| REQ-HUD-002 | The HUD shall display real-time speed with auto-scaled units | P0 | EP-05 |
| REQ-HUD-003 | The HUD shall display the nearest star and planet: name, type, distance, spectral class | P0 | EP-05 |
| REQ-HUD-004 | The HUD shall display energy pool level, per-subsystem allocation bars, shield %, hull % | P0 | EP-05 |
| REQ-HUD-005 | The HUD shall display contextual alerts: gravity warning, low energy, warp lockout, sensor contact, red alert | P0 | EP-05 |
| REQ-HUD-006 | A solar system minimap shall appear in the lower-left during Mode 2 and Mode 3 | P0 | EP-05 |
| REQ-HUD-007 | The minimap shall show orbital rings, planet positions, and the ship as an X marker | P0 | EP-05 |
| REQ-HUD-008 | The minimap shall display distance to the nearest planet | P0 | EP-05 |
| REQ-HUD-009 | The minimap shall support inner-system zoom toggle | P1 | EP-05 |
| REQ-HUD-010 | A galaxy/star map overlay shall be accessible via toggle (full screen) | P0 | EP-05 |
| REQ-HUD-011 | The galaxy map shall filter by 10 LY / 50 LY / 100 LY distance bands | P0 | EP-05 |
| REQ-HUD-012 | Stars on the galaxy map shall be coloured by spectral class | P0 | EP-05 |
| REQ-HUD-013 | Explored systems and alien-contact systems shall be visually flagged on the galaxy map | P0 | EP-05 |
| REQ-HUD-014 | Clicking a star on the galaxy map shall set it as the Super Warp target | P0 | EP-05 |
| REQ-HUD-015 | All HUD elements shall be within screen bounds at 1920×1080 and 2560×1440 | P0 | EP-05 |
| REQ-HUD-016 | Planet labels shall billboard (always face camera) at all viewing distances | P1 | EP-05 |

---

### 4.6 Game Framework (Domain: GAME)

| ID | Requirement | Priority | Epic |
|---|---|---|---|
| REQ-GAME-001 | The application shall present a main menu with: New Game, Load Game, Settings, Quit | P0 | EP-06 |
| REQ-GAME-002 | New Game shall present a ship selection screen before spawning the player | P0 | EP-06 |
| REQ-GAME-003 | New Game shall spawn the player in low Earth orbit (~400 km) facing the Sun | P0 | EP-06 |
| REQ-GAME-004 | The in-game pause menu shall provide: Resume, Save, Settings, Quit to Menu | P0 | EP-06 |
| REQ-GAME-005 | The Settings screen shall expose: graphics quality, audio volume, control bindings, ship config | P0 | EP-06 |
| REQ-GAME-006 | All control bindings shall be fully remappable via the Settings screen | P0 | EP-06 |
| REQ-GAME-007 | Control bindings shall persist to `config/game.json` and reload on next launch | P0 | EP-06 |
| REQ-GAME-008 | The save system shall persist: position, orientation, ship, realm, energy, hull, time, reputation, tech tree, carried gifts | P0 | EP-06 |
| REQ-GAME-009 | The game shall support multiple save slots | P1 | EP-06 |
| REQ-GAME-010 | Game Over shall trigger at hull 0%, display the Game Over screen, and offer reload from last save | P0 | EP-06 |
| REQ-GAME-011 | The in-game clock shall begin at the real-world date/time of New Game creation | P0 | EP-06 |
| REQ-GAME-012 | Time warp shall support multipliers: 1x, 10x, 100x, 1000x, 10000x | P0 | EP-06 |
| REQ-GAME-013 | Time warp shall automatically disengage during combat or active warp travel | P0 | EP-06 |
| REQ-GAME-014 | Planet positions shall update with in-game time via Horizons ephemeris data | P0 | EP-06 |

---

### 4.7 Combat & Sensors (Domain: COMBAT)

| ID | Requirement | Priority | Epic |
|---|---|---|---|
| REQ-COMBAT-001 | The ship shall support energy-based weapons (laser / plasma burst) | P0 | EP-07 |
| REQ-COMBAT-002 | Weapons shall draw from the shared energy pool using the player-allocated Weapons budget | P0 | EP-07 |
| REQ-COMBAT-003 | Ship detection shall be physics-based: electromagnetic emissions, heat signature, drive light | P0 | EP-07 |
| REQ-COMBAT-004 | Detection range and accuracy shall improve with sensor technology upgrades | P0 | EP-07 |
| REQ-COMBAT-005 | A red alert shall trigger on enemy ships only when weapons are confirmed hot (charged) | P0 | EP-07 |
| REQ-COMBAT-006 | The player's own emissions (weapons hot) shall increase the ship's detectability | P1 | EP-07 |
| REQ-COMBAT-007 | At low hull thresholds, individual subsystems shall begin to fail (warp, shields, sensors) | P1 | EP-07 |
| REQ-COMBAT-008 | Enemy ships shall materialise from the Spirit Realm via a Star Trek shimmer/dissolve visual effect | P0 | EP-07 |
| REQ-COMBAT-009 | A sensor contact alert shall appear on the HUD when a ship is detected within range | P0 | EP-07 |
| REQ-COMBAT-010 | An energy intention detector module (unlocked via tech tree) shall reveal hostile/peaceful intent | P1 | EP-07 |

---

### 4.8 Alien Civilisations & Factions (Domain: ALIEN)

| ID | Requirement | Priority | Epic |
|---|---|---|---|
| REQ-ALIEN-001 | Alien ships shall appear at foreign (non-Sol) star systems; frequency increases with exploration distance | P0 | EP-08 |
| REQ-ALIEN-002 | 80% of alien encounters shall be Federation of Civilisations (peaceful) | P0 | EP-08 |
| REQ-ALIEN-003 | 20% of alien encounters shall be aggressive | P0 | EP-08 |
| REQ-ALIEN-004 | Federation and aggressive ships shall have no visual difference at game start | P0 | EP-08 |
| REQ-ALIEN-005 | Federation alien encounters shall trigger a text dialogue panel with optional race portrait | P0 | EP-08 |
| REQ-ALIEN-006 | The dialogue panel shall offer: accept tech gift, accept information, decline | P0 | EP-08 |
| REQ-ALIEN-007 | Reputation shall be tracked per alien race and persisted in the save file | P0 | EP-08 |
| REQ-ALIEN-008 | Attacking a Federation ship shall set reputation with that race to Hostile | P0 | EP-08 |
| REQ-ALIEN-009 | A Hostile race shall not offer dialogue and shall attack the player on sight | P0 | EP-08 |
| REQ-ALIEN-010 | Reputation recovery shall require completing missions assigned by the offended race | P0 | EP-08 |
| REQ-ALIEN-011 | The game start sequence shall include a Spirit Realm alien contact cutscene/event | P0 | EP-08 |
| REQ-ALIEN-012 | The first alien contact (Spirit Realm race) shall introduce the lore narrative hook | P0 | EP-08 |

---

### 4.9 Technology Progression (Domain: TECH)

| ID | Requirement | Priority | Epic |
|---|---|---|---|
| REQ-TECH-001 | Alien tech gifts shall be received during Federation dialogue encounters | P0 | EP-09 |
| REQ-TECH-002 | Carried tech gifts shall be lost permanently if the ship is destroyed | P0 | EP-09 |
| REQ-TECH-003 | The player shall be able to deposit carried gifts at Earth/Sol to convert them into permanent R&D progress | P0 | EP-09 |
| REQ-TECH-004 | Permanent tech unlocks shall survive ship destruction and apply to new ship builds | P0 | EP-09 |
| REQ-TECH-005 | The tech tree shall have 6 branches: Propulsion, Sensors, Shields, Weapons, Hull, Spirit Realm | P0 | EP-09 |
| REQ-TECH-006 | Each tech tree node shall have defined prerequisites and a cost in deposited gifts | P0 | EP-09 |
| REQ-TECH-007 | The tech tree UI shall be accessible from the main menu and in-game pause screen | P0 | EP-09 |
| REQ-TECH-008 | New ships built at Earth shall incorporate all currently unlocked tech tree nodes | P0 | EP-09 |
| REQ-TECH-009 | The Spirit Realm drive shall be a tech tree unlock in the Spirit Realm branch | P0 | EP-09 |
| REQ-TECH-010 | The energy intention detector shall be a tech tree unlock in the Sensors branch | P1 | EP-09 |

---

### 4.10 Spirit Realm (Domain: SPIRIT)

| ID | Requirement | Priority | Epic |
|---|---|---|---|
| REQ-SPIRIT-001 | The Spirit Realm shall use the same coordinate system and star map as normal space | P0 | EP-10 |
| REQ-SPIRIT-002 | The Spirit Realm shall be implemented as a second render pass (scene overlay) on the normal world | P0 | EP-10 |
| REQ-SPIRIT-003 | In Spirit Realm, planets shall display a second visual layer: energy hubs and alien architecture | P0 | EP-10 |
| REQ-SPIRIT-004 | In Spirit Realm, stars shall be surrounded by visible civilisation activity | P0 | EP-10 |
| REQ-SPIRIT-005 | Earth in Spirit Realm shall display a city layer representing the spirits of the dead | P0 | EP-10 |
| REQ-SPIRIT-006 | Alien ship density shall be significantly higher in Spirit Realm than in normal space | P0 | EP-10 |
| REQ-SPIRIT-007 | Spirit Realm entry shall require either the Spirit Realm drive (tech tree) or an alien stargate | P0 | EP-10 |
| REQ-SPIRIT-008 | The realm-crossing transition shall use a Star Trek shimmer/dissolve shader effect | P0 | EP-10 |
| REQ-SPIRIT-009 | The HUD realm indicator shall switch between NORMAL and SPIRIT REALM | P0 | EP-10 |
| REQ-SPIRIT-010 | Enemies crossing from Spirit Realm to normal space shall display the shimmer transition effect | P0 | EP-10 |
| REQ-SPIRIT-011 | Spirit Realm structures shall be subject to their own LOD quality tier | P1 | EP-10 |

---

### 4.11 Audio (Domain: AUDIO)

| ID | Requirement | Priority | Epic |
|---|---|---|---|
| REQ-AUDIO-001 | The audio system shall use a pluggable backend (OpenAL or miniaudio) | P0 | EP-11 |
| REQ-AUDIO-002 | Normal space shall have an ambient soundscape distinct from Spirit Realm | P0 | EP-11 |
| REQ-AUDIO-003 | Ship engine shall produce a hum that varies with speed and flight mode | P1 | EP-11 |
| REQ-AUDIO-004 | Warp engage and warp exit shall have distinct audio cues | P0 | EP-11 |
| REQ-AUDIO-005 | Realm transition (enter/exit Spirit Realm) shall have a distinct audio effect | P0 | EP-11 |
| REQ-AUDIO-006 | Combat events (shield hit, hull hit, weapon fire) shall have audio feedback | P0 | EP-11 |
| REQ-AUDIO-007 | All audio volumes shall be independently configurable via Settings | P0 | EP-11 |
| REQ-AUDIO-008 | All audio assets shall be sourced from CC-licensed libraries or AI generation tools | P0 | EP-11 |

---

### 4.12 Visual Assets (Domain: ASSETS)

| ID | Requirement | Priority | Epic |
|---|---|---|---|
| REQ-ASSETS-001 | At least one player ship 3D model shall be produced (AI-generated + Blender cleanup) | P0 | EP-12 |
| REQ-ASSETS-002 | Each alien faction type shall have a distinct ship silhouette model | P1 | EP-12 |
| REQ-ASSETS-003 | Race portrait images shall be produced for at least 3 alien races at launch | P1 | EP-12 |
| REQ-ASSETS-004 | All planet textures shall use NASA Solar System Exploration source images | P0 | EP-12 |
| REQ-ASSETS-005 | Spirit Realm structure assets shall use procedural shaders as the primary generation method | P0 | EP-12 |
| REQ-ASSETS-006 | All HUD icons and UI elements shall be consistent in style | P1 | EP-12 |

---

## 5. Non-Functional Requirements

| ID | Requirement | Priority |
|---|---|---|
| REQ-NFR-001 | The application shall achieve 60 FPS minimum on mid-range hardware (GTX 1060 / RX 580 class) at 1080p | P0 |
| REQ-NFR-002 | The application shall not exceed 4 GB RAM usage at runtime under normal conditions | P0 |
| REQ-NFR-003 | Load time from main menu to in-game shall not exceed 15 seconds on an SSD | P1 |
| REQ-NFR-004 | All floating-point spatial calculations shall use double precision | P0 |
| REQ-NFR-005 | Save files shall be human-readable JSON | P1 |
| REQ-NFR-006 | All source code shall be C++17 compliant | P0 |
| REQ-NFR-007 | All unit tests shall pass on CI before a sprint is marked complete | P0 |
| REQ-NFR-008 | Each sprint shall require a human QA audit before the next sprint begins | P0 |
| REQ-NFR-009 | No third-party library with a GPL license shall be included | P0 |
| REQ-NFR-010 | The application shall not crash on invalid or missing config values — graceful error and defaults | P0 |
| REQ-NFR-011 | The codebase shall be organised into modules matching the epic structure | P1 |
| REQ-NFR-012 | All configurable values shall live in config files — no magic numbers in code | P1 |

---

## 6. Architecture Constraints

| Constraint | Rationale |
|---|---|
| Camera system abstracted behind an interface | Enables future third-person view without restructuring flight code |
| Spirit Realm is a scene layer, not a separate world | Same coordinate system — no spatial engine duplication |
| Ship stats fully data-driven | New ships require no code changes |
| Gravity zone collision hooks present from Sprint 3 | Enables future landing/atmosphere mode |
| Audio backend pluggable | Can swap OpenAL for miniaudio without touching audio call sites |

---

## 7. Out of Scope (This Release)

- Multiplayer / co-op
- Planet surface landing and terrain traversal
- Atmosphere entry / re-entry model
- Third-person external camera (architecture hook in place, implementation deferred)
- Earth satellite rendering (toggle planned, full implementation deferred)
- Modding / custom content support
- Console or mobile builds

---

*This document is the baseline for all engineering work. Changes require owner sign-off and a version increment.*
