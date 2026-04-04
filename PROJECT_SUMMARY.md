# Spirit Space — Project Summary

## Vision

Spirit Space is a realistic space simulation game built on real astronomical data from space agencies (NASA, ESA, and others). Players pilot an antigravity ship through an accurate representation of the universe — from the surface of planets to the depths of interstellar space. The game blends scientific accuracy with approachable flight controls and emergent gameplay driven by energy management.

---

## Core Pillars

| Pillar | Description |
|---|---|
| **Real Data** | Stars, planets, and exoplanets rendered from actual agency datasets |
| **Authentic Visuals** | Star color and size derived from spectral class and luminosity data |
| **Layered Flight** | Three distinct control modes based on proximity and speed |
| **Energy Strategy** | One energy pool shared between propulsion, shields, and weapons |
| **Exploration First** | Discovery and traversal are the primary gameplay loops |

---

## Data Sources

- **NASA Exoplanet Archive** — exoplanet catalog (CSV / API)
- **HYG Star Database** — 119,614 stars with spectral data, positions, magnitudes
- **NASA/JPL Horizons** — Solar System body ephemeris (real-time positions)
- **ESA Gaia DR3** — high-precision star astrometry
- **NASA Eyes on the Solar System** — planetary texture references

Data will be pre-downloaded and bundled at build time, with optional runtime API refresh for ephemeris data.

---

## Flight Mechanics

### Mode 1 — Interstellar Warp
- Used to travel between star systems
- **Super Warp**: selects a target star, arrives in seconds
- Energy cost scales with distance; heavy drain on energy pool
- Visual: tunnel / warp effect, then star emergence

### Mode 2 — System Warp
- Used inside a star system to move between planets
- Speed tuned to prevent overshoot; damped acceleration curve
- HUD shows estimated arrival time and fuel consumption
- Warp locks off if too close to a gravity well

### Mode 3 — Planetary / Orbital Flight
- Standard antigravity ship flight around and near planets
- Simplified Newtonian feel — no complex orbital mechanics required from player
- Free 6-DOF movement (antigravity removes gravity constraint)
- Smooth keyboard controls with dead-zone and response tuning

---

## Ship Systems

### Energy Pool
Single rechargeable energy resource shared across all ship systems. Energy regenerates passively at a base rate.

| System | Draw |
|---|---|
| Antigravity Propulsion | Low (always on) |
| System Warp | Medium |
| Super Warp | Very High |
| Shield Bubble | Low–High (configurable) |
| Energy Weapons | Medium–High (configurable) |

Energy allocation is manual — the player decides how to split budget between shield, weapons, and warp. Reducing shield or warp allocation allows more power to weapons, and vice versa.

### Shield Bubble
- Deflects objects (micrometeorites, debris, projectiles) using the antigravity field
- Effectiveness scales with energy allocated
- Visible as a translucent energy sphere around the ship
- Drains extra energy when absorbing impacts

### Weapons (future milestone)
- Energy-based weapons only (lasers, plasma bursts)
- Compete directly with shield and warp for energy budget
- Details deferred to a later sprint

---

## HUD

- Contextual info panel: nearest star(s) and planet(s) with real name, type, and distance
- Energy bar split into subsystem gauges
- Current flight mode indicator
- Warp target selector overlay (interstellar mode)
- Speed readout in m/s, km/s, AU/s, ly/s depending on mode

---

## Visual Generation

Stars are rendered using real spectral and luminosity data:
- **Color**: derived from spectral class (O/B/A/F/G/K/M → blue → red)
- **Size**: derived from absolute magnitude and distance
- **Glow / bloom**: intensity based on luminosity
- Planet textures generated procedurally from type data (rocky, gas giant, ice, ocean)
- Atmospheric scattering shader for habitable-zone planets

---

## Technology Stack (to be finalized — see `TECH_STACK.md`)

- **Language**: C++17 or C++20
- **Graphics**: OpenGL 4.5+ with GLSL shaders
- **Windowing**: SDL2 or GLFW
- **Math**: GLM
- **UI / HUD**: Dear ImGui
- **Data Parsing**: nlohmann/json, fast-cpp-csv-parser or similar
- **Networking / API**: libcurl (optional ephemeris updates)
- **Build System**: CMake
- **Testing**: Google Test (unit), custom QA audit framework
- **Version Control**: Git + GitHub

---

## Development Methodology

### Workflow
1. **Tech Stack Selection** → finalize libraries, write `TECH_STACK.md`
2. **Specs** → write detailed feature specifications per subsystem
3. **Sprints** → time-boxed work units with acceptance criteria
4. **Unit Tests** → per subsystem, required to pass before sprint closes
5. **QA Audit** → human review gate at end of each sprint
6. **Next Sprint** → only begins after QA sign-off

### Sprint Structure (planned)
| Sprint | Focus |
|---|---|
| 0 | Repo setup, build pipeline, data download scripts |
| 1 | Star rendering from HYG data (OpenGL point stars, color, size) |
| 2 | Solar System — planet positions from Horizons, orbital display |
| 3 | Camera / flight — Mode 3 planetary flight with keyboard controls |
| 4 | System Warp — Mode 2 with HUD and speed curve |
| 5 | Super Warp — Mode 1 interstellar travel |
| 6 | Ship systems — energy pool, shield bubble |
| 7 | HUD polish, exoplanet data integration |
| 8 | Weapons, energy allocation UI |
| 9+ | Audio, procedural planets, additional game mechanics |

Each sprint will have:
- Feature specification
- Acceptance criteria checklist
- Unit test requirements
- QA audit checkpoint (human review)

---

## Conventions

- All human review gates will be clearly surfaced before proceeding
- Agents and skills will be used to minimize token usage on repetitive tasks
- No feature will be implemented without a spec and acceptance criteria
- Real data always takes precedence over hardcoded values

---

*Document version: 0.1 — 2026-04-04*
