# Spirit Space — Project Summary

> Version: 0.2 — 2026-04-04

---

## Vision

Spirit Space is a realistic space simulation game built on real astronomical data from space agencies (NASA, ESA, and others). Players pilot an antigravity ship through an accurate representation of the universe — from Earth orbit to the depths of interstellar space. The game blends scientific accuracy with approachable flight controls and emergent gameplay driven by energy management.

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

| Source | Data | Format |
|---|---|---|
| HYG Star Database | 119,614 stars — spectral class, position, magnitude | CSV |
| NASA Exoplanet Archive | Confirmed exoplanet catalog | CSV / API |
| NASA/JPL Horizons | Solar System body ephemeris (positions) | API / text |
| ESA Gaia DR3 | High-precision star astrometry | CSV |
| NASA Solar System Exploration | Planet/moon textures (high-res, free) | PNG/JPG |
| ESA/NASA Milky Way panorama | Skybox base image (CC-licensed) | FITS/PNG |

Data pre-downloaded and bundled at build time. Optional runtime API refresh for ephemeris (Horizons). Exoplanet orbital positions are approximated (random phase — true 3D positions are not publicly available for most systems).

---

## Coordinate System

- **Origin**: Sol (the Sun) at world origin `(0, 0, 0)`
- **Internal units**: AU (Astronomical Units) within the Solar System; Light Years (LY) at interstellar scale
- **HUD display**: switches automatically — m/s, km/s, AU/s, LY/s based on flight mode
- **Floating-point precision**: double precision throughout; origin-shifting applied when far from Sol to avoid precision loss at interstellar distances
- **Reference frame**: J2000 ecliptic plane aligned with XZ, Y = north ecliptic pole

---

## Solar System Scope

All major Solar System bodies are included:

| Category | Included | Notes |
|---|---|---|
| Sun | Yes | At world origin |
| 8 Planets | Yes | Real positions from Horizons ephemeris |
| Dwarf Planets | Yes | Pluto, Eris, Ceres, Makemake, Haumea |
| Major Moons | Yes | Earth's Moon, Galilean moons, Titan, etc. |
| Asteroid Belt | Yes | Simplified — representative distribution, not all 1M+ objects |
| Kuiper Belt | Partial | Visual representation, major KBOs |
| Earth Satellites | Optional | Toggle in settings — ISS, Hubble, etc. |
| Natural Satellites (small) | No | Out of scope |

---

## Starting State

- **Spawn location**: Low Earth orbit (~400 km altitude)
- **Initial orientation**: Ship facing toward the Sun
- **Starting time**: Real-world date at launch (live ephemeris position)
- **Ship**: Default starter ship (mid-tier energy ratings)

---

## Game States

```
Main Menu
  ├── New Game → spawn in Earth orbit
  ├── Load Game → restore saved position, ship state, time
  ├── Settings → graphics, audio, controls, ship config
  └── Quit

In-Game
  ├── HUD active (flight / exploration)
  ├── Pause Menu → resume / save / settings / quit
  └── Game Over → ship destroyed (damage = 0%) → option to reload save
```

Save system stores: player position, orientation, current ship, energy state, in-game date/time, damage levels.

---

## Flight Mechanics

### Mode 1 — Interstellar Super Warp
- Travel between star systems
- Player selects target star via star map or HUD overlay
- Arrives at outer system boundary in seconds (warp tunnel visual)
- Energy cost scales with distance — very high drain
- Cannot engage if energy below minimum threshold

### Mode 2 — System Warp
- Travel between planets within a star system
- Damped speed curve — acceleration → cruise → guaranteed deceleration stop
- HUD shows ETA and energy cost estimate
- Warp lock engages if too close to a planet (gravity warning)
- Speed readout in AU/s

### Mode 3 — Planetary / Orbital Flight
- 6-DOF antigravity flight near planets and moons
- No gravity force on the ship (antigravity active)
- **Gravity proximity warning**: when entering a planet's gravitational influence zone, HUD warns that atmosphere entry is not available (out of scope — architecture designed to support it later)
- Smooth keyboard controls with configurable dead-zone and damping (via config file)
- Speed readout in m/s / km/s

**Camera**: First-person (cockpit view) for all modes in current scope. Third-person external view planned for a future sprint — architecture will expose a camera abstraction layer to support this.

---

## Ship System

### Multiple Ships
The game supports multiple ship types, each with different stat profiles. Ships are defined in a config file (see Configuration). Stat differences:

| Stat | Effect |
|---|---|
| Max Energy | Total energy pool size |
| Energy Regen Rate | Passive recharge speed |
| Warp Efficiency | Energy cost per AU / LY of warp |
| Shield Strength | Max shield absorption per hit |
| Weapon Power | Damage output |
| Agility | Turn rate, strafe speed |

Starting ship is a balanced mid-tier vessel. Additional ships unlocked or selected at main menu.

---

### Energy Pool
Single rechargeable energy resource shared across all ship systems. Regenerates passively at the ship's regen rate.

| System | Draw Level |
|---|---|
| Antigravity Propulsion | Low (always on, base drain) |
| System Warp | Medium |
| Super Warp | Very High |
| Shield Bubble | Low–High (player-allocated) |
| Energy Weapons | Medium–High (player-allocated) |

Manual energy allocation: player distributes budget between Shield, Weapons, and Warp Reserve using allocation sliders. Changes take effect immediately.

---

### Shield Bubble (Star Trek model)
- Deflects debris, projectiles, and impacts using the antigravity field
- Translucent sphere visible around the ship — opacity reflects current shield level
- Shield absorbs hits progressively (many hits to deplete, like Star Trek)
- Once shield reaches 0%, ship hull becomes fully exposed
- Energy allocated to shields determines recharge speed and absorption capacity

---

### Ship Damage
- Hull integrity: **0–100%**
- At **100%**: undamaged
- At **0%**: ship destroyed → Game Over screen → option to load last save
- Damage accrues when shield is at 0% and ship takes hits
- In-system damage model: individual subsystems can be degraded (e.g. warp offline, shields weakened) at specific hull thresholds
- Subsystem damage thresholds: TBD in Sprint 6 spec (damage model sprint)
- Primary defense is shields — hull damage should be rare until shields collapse

---

## HUD

### Flight HUD
- **Top center**: Flight mode indicator (PLANETARY / SYSTEM WARP / INTERSTELLAR WARP)
- **Top right**: Speed readout (unit auto-scales with mode)
- **Left panel**: Nearest star and planet — name, type, spectral class, distance
- **Right panel**: Energy gauges — total pool, per-subsystem allocation bars, shield %, hull %
- **Center warning area**: Gravity proximity warning, low energy warning, warp lockout notice

### Solar System Minimap (lower left)
- Small 2D panel showing the Sun and planet orbital rings
- Planets shown as dots on their orbital ring
- Ship position shown as **X** marker on the correct orbital ring (or between rings)
- Distance label from ship to nearest planet
- Scale: fixed to fit all 8 planets; toggle to zoom in on inner system
- Only visible in Mode 2 (System Warp) and Mode 3 (Planetary Flight)

### Galaxy / Star Map (full-screen overlay, toggle)
- Shows local star cluster around Sol
- Distance filter: **10 LY / 50 LY / 100 LY** toggle
- Stars shown as colored dots (spectral class color)
- Known exoplanet systems highlighted
- Ship position at center
- Click a star to set as Super Warp target

---

## Visual Generation

### Stars
- Color: derived from spectral class (O/B/A/F/G/K/M → blue through red)
- Size: derived from absolute magnitude and distance
- Glow / bloom: intensity from luminosity
- Binary/multiple star systems: each component rendered as a separate object at its actual position

### Skybox / Deep Space Background
- Base layer: NASA/ESA Milky Way panoramic image (CC-licensed, mapped to skybox cube)
- Procedural layer: noise-based nebula and dust lane shader overlaid on the panorama
- The procedural layer adds visual depth without conflicting with real star data
- Style reference: Elite Dangerous star field aesthetics — realistic but visually rich

### Planets & Moons
- Textures: NASA Solar System Exploration free high-res textures (real photography)
- Gas giants: procedural band shader layered on texture
- Atmosphere: scattering shader on habitable-zone planets
- Rings: Saturn and Uranus ring systems from real data

### LOD Strategy
- Start with a minimal LOD tier targeting 60fps on mid-range hardware at 1080p
- In-game graphics quality setting (Low / Medium / High) controlling:
  - Star render count and glow passes
  - Planet texture resolution
  - Shadow and atmospheric quality
  - Bloom intensity
- Strategies to benchmark: distance-based point culling, frustum culling, GPU instancing for stars
- Hardware profiling built into Sprint 1 before committing to a strategy

---

## Time System
- In-game clock tracks a real calendar date/time (starting from real-world launch date)
- Drives ephemeris queries — planet positions update with time
- **Time warp**: multiplier control (1x, 10x, 100x, 1000x, 10000x)
- At high time warp, players can watch orbital motion in real time
- Time warp automatically disables in combat or during active warp travel

---

## Audio

Deferred to Sprint 9. Architecture will use a pluggable audio backend (OpenAL or miniaudio).

**Asset strategy** (no custom studio work needed):
| Asset Type | Source |
|---|---|
| Ambient space / music | Meta AudioCraft / MusicGen (AI, free, local) |
| Ship engine hum | Freesound.org (CC-licensed) or AudioCraft |
| Warp charge / jump | ElevenLabs Sound Effects (text-to-sound) or Stable Audio |
| UI sounds | Freesound.org |
| Impact / shield hit | Freesound.org + AudioCraft |

---

## Visual Assets

| Asset Type | Source |
|---|---|
| Planet textures | NASA Solar System Exploration (free, official) |
| Milky Way skybox | ESA/NASA panoramic images (CC-licensed) |
| Ship 3D model | Meshy.ai or Tripo3D (AI text-to-3D, free tier) + Blender cleanup |
| UI icons / HUD elements | Midjourney or DALL-E 3 → SVG trace |
| Nebula / dust overlays | Procedural GLSL shader (no external asset needed) |

---

## Technology Stack (see `TECH_STACK.md` for full rationale)

| Layer | Choice |
|---|---|
| Language | C++17 |
| Build | CMake 3.20+ + vcpkg |
| Graphics | OpenGL 4.5 core + GLAD + GLFW 3 |
| Math | GLM (double precision) |
| UI / HUD | Dear ImGui (docking branch) |
| Data parsing | nlohmann/json, fast-cpp-csv-parser, libcurl, stb_image |
| Testing | Google Test |
| CI | GitHub Actions |
| Sound (Sprint 9+) | OpenAL or miniaudio |

---

## Configuration System

All runtime-configurable values live in a **JSON config file** (`config/game.json`). This includes:

- Graphics settings (resolution, fullscreen, quality tier)
- Control bindings (fully remappable — keyboard/mouse)
- Ship definitions (stats for each ship in the game)
- Active ship selection
- Energy allocation defaults
- LOD quality tier
- Audio volume levels
- Time warp speed options

Ship definitions in config allow new ships to be added without recompiling. Config is the single source of truth for ship stats.

---

## Development Methodology

### Workflow
1. **Tech Stack** → finalize `TECH_STACK.md`
2. **Specs** → detailed feature spec per subsystem before any code
3. **Sprints** → time-boxed with acceptance criteria
4. **Unit Tests** → required to pass before sprint closes
5. **QA Audit** → human review gate (project owner) at each sprint boundary
6. **Next Sprint** → begins only after QA sign-off

### Sprint Roadmap
| Sprint | Focus |
|---|---|
| 0 | Repo, build pipeline, CMake, data download + validation scripts, empty window |
| 1 | Star field — HYG data, spectral color, magnitude size, LOD benchmarking |
| 2 | Solar System — Horizons ephemeris, all planets/moons, orbital rings, skybox |
| 3 | Planetary flight — Mode 3 6-DOF controls, config file, gravity warning |
| 4 | System Warp — Mode 2, minimap, warp curve, lockout zone, time warp |
| 5 | Super Warp — Mode 1, star map overlay, LY filter, tunnel effect |
| 6 | Energy & Shield — pool, allocation UI, shield bubble, damage model |
| 7 | HUD polish — all gauges, exoplanet integration, galaxy map |
| 8 | Game states — main menu, pause, save/load, game over screen |
| 9 | Multiple ships — config-driven ship definitions, ship selector |
| 10 | Weapons — energy weapons, combat damage, in-system subsystem damage |
| 11 | Audio — ambient, engine, warp, UI sounds |
| 12 | Polish — procedural planet detail, asteroid field, LOD tuning, QA |
| 13+ | Future: third-person camera, landing mode (architecture hook), satellites toggle |

### Architecture Notes for Future Expansion
- **Landing / atmosphere entry**: collision layer and gravity zone checks are designed as hooks — surface flight mode can be added without restructuring the flight state machine
- **Third-person camera**: camera abstracted behind an interface from Sprint 3 onward; external camera added by implementing the interface
- **Additional ships**: fully data-driven via config file — no code changes required for new ship types

---

## Conventions

- Human review gates surfaced explicitly before proceeding
- Agents and skills used to minimize token usage on repetitive tasks
- No feature implemented without a spec and acceptance criteria
- Real data always takes precedence over hardcoded values
- Config file is the single source of truth for tunable values
- All coordinate math uses double precision

---

*Document version: 0.2 — 2026-04-04*
