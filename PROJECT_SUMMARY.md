# Spirit Space — Project Summary

> Version: 0.5 — 2026-04-12

---

## Vision

Spirit Space is a realistic space simulation game built on real astronomical data from space agencies (NASA, ESA, and others). Players pilot an antigravity ship through an accurate representation of the universe — from Earth orbit to the depths of interstellar space — and eventually into a parallel dimension known as the **Spirit Realm**, home to alien civilisations and the spirits of the dead. The game blends scientific accuracy with approachable flight controls, emergent gameplay driven by energy management, faction diplomacy, and a technology progression tree unlocked through alien contact.

---

## Development Philosophy — Engine First, Then Simulation, Then Game

The project is built in **three distinct phases**:

**Phase 1 — Game Engine** is completed first. The result is a standalone, reusable engine with zero domain knowledge: ECS (EnTT), OpenGL renderer, asset loader, audio backend, input system, config system, HUD framework, save/load framework, game state machine, and time system. Any game could be built from it.

**Phase 2 — Star Simulation** proves the engine by building a fully functional real-universe simulation on top of it: accurate stars, real planet positions, all flight modes, time control, and free exploration. This phase produces **Sandbox Mode** — a clean, gameplay-free experience that stands on its own.

**Phase 3 — Spirit Space Game** adds all gameplay on top of the proven simulation: energy systems, combat, alien factions, tech progression, and the Spirit Realm. This produces **Game Mode**.

The three phases are strictly sequential — each phase gates the next. Sandbox Mode is never removed from the shipped product — it remains the pure simulation experience.

---

## Application Modes

### Sandbox Mode
- Pure space simulation — no aliens, no combat, no energy management, no damage
- Free exploration of the real universe using all three flight modes
- Real star field (HYG 119k+ stars), Solar System with live ephemeris positions
- Time warp: watch orbital mechanics in real time
- Minimap, galaxy map, contextual HUD (nearest body, speed, position)
- Save and restore position
- Target audience: anyone who wants to explore real space data

### Game Mode
- Everything in Sandbox Mode, plus all gameplay systems
- Energy pool, shields, hull damage, weapons
- Alien civilisations — Federation (80% friendly) and aggressive (20%)
- Faction reputation, tech gifts, Earth R&D, technology tree
- Spirit Realm layer — parallel dimension with additional visual layer and content
- Game states: main menu, ship selection, save/load full game state, game over

The mode is selected at the main menu. Both modes use the same simulation engine and world data.

---

## Core Pillars

| Pillar | Phase | Description |
|---|---|---|
| **Real Data** | 1 | Stars, planets, exoplanets from agency datasets |
| **Authentic Visuals** | 1 | Star color/size from spectral class and luminosity |
| **Layered Flight** | 1 | Three flight modes based on proximity and speed |
| **Exploration First** | 1 | Discovery and traversal as primary loop |
| **Energy Strategy** | 2 | One energy pool shared between propulsion, shields, weapons |
| **Two Realms** | 2 | Normal space + Spirit Realm — same map, layered |
| **Faction Diplomacy** | 2 | Reputation with alien civilisations shapes access and threat |
| **Technology Progression** | 2 | Alien gifts + Earth R&D unlock ship capabilities |

---

## Lore & Narrative Hook

At the beginning of the game an alien race makes contact — communicating from the **Spirit Realm**, a parallel dimension overlaid on the same universe. They reveal that this realm is inhabited by the spirits of people who have died; their consciousness persists there, and a vast civilisation of spirit-beings has grown around Earth and across the galaxy.

The game name — **Spirit Space** — refers to this realm and to the player's journey toward understanding and eventually entering it.

The early game is a sandbox of exploration. The deeper narrative (the nature of the Spirit Realm, Earth's spiritual population, alien intentions) is revealed gradually through alien dialogue encounters as the player explores further from Sol.

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

Data pre-downloaded and bundled at build time. Optional runtime API refresh for Horizons. Exoplanet orbital positions approximated (random phase).

---

## Coordinate System

- **Origin**: Sol at world origin `(0, 0, 0)`
- **Internal units**: AU within the Solar System; LY at interstellar scale
- **HUD display**: auto-scales — m/s, km/s, AU/s, LY/s
- **Floating-point precision**: double throughout; origin-shifting at interstellar distances
- **Reference frame**: J2000 ecliptic plane (XZ), Y = north ecliptic pole
- **Spirit Realm**: same coordinate system — same star positions, same scale, layered on top

---

## Solar System Scope

| Category | Included | Notes |
|---|---|---|
| Sun | Yes | At world origin |
| 8 Planets | Yes | Real positions from Horizons ephemeris |
| Dwarf Planets | Yes | Pluto, Eris, Ceres, Makemake, Haumea |
| Major Moons | Yes | Earth's Moon, Galilean moons, Titan, etc. |
| Asteroid Belt | Yes | Simplified — representative distribution |
| Kuiper Belt | Partial | Visual representation, major KBOs |
| Earth Satellites | Optional | Toggle in settings — ISS, Hubble, etc. |

---

## Starting State

- **Spawn**: Low Earth orbit (~400 km), facing the Sun
- **Time**: Real-world date at launch
- **Ship**: Default mid-tier vessel
- **Realm**: Normal space — Spirit Realm not yet accessible

---

## Game States

```
Main Menu
  ├── New Game → ship select → spawn in Earth orbit
  ├── Load Game → restore position, ship, realm, reputation, tech tree, in-game time
  ├── Settings → graphics, audio, controls, ship config
  └── Quit

In-Game
  ├── Normal Space — exploration / combat / alien encounters
  ├── Spirit Realm — same map, extra layer active (unlocked via progression)
  ├── Pause Menu → resume / save / settings / quit to menu
  └── Game Over → hull = 0% → lose all carried tech gifts → option to reload save
```

Save system stores: position, orientation, ship, realm state, energy, hull%, in-game date/time, reputation score, technology tree state, carried alien tech gifts.

---

## Flight Mechanics

### Mode 1 — Interstellar Super Warp
- Travel between star systems
- Target selected via star map overlay
- Arrives at outer system boundary in seconds — warp tunnel visual effect
- Very high energy cost; cannot engage below minimum energy threshold

### Mode 2 — System Warp
- Travel between planets within a star system
- Damped speed curve — guaranteed deceleration stop, no overshoot
- HUD shows ETA and energy cost estimate
- Warp lockout if too close to a planet gravity well

### Mode 3 — Planetary / Orbital Flight
- 6-DOF antigravity flight near planets and moons
- No gravity on ship (antigravity active)
- **Gravity proximity warning**: HUD warns atmosphere entry is unavailable (future expansion hook)
- Configurable dead-zone and damping via config file

**Camera**: First-person (cockpit). Third-person camera planned — abstracted behind interface from Sprint 3.

---

## The Spirit Realm

### What It Is
A parallel dimension overlaid on normal space. It occupies the same coordinates and uses the same star map, but presents a different visual layer on top of physical reality. It is the origin of most alien civilisations and the home of human spirits after death.

### Visual Layer
- Same stars, same planets, same positions — but with additional detail overlaid:
  - Planets have a **second visual layer**: energy hubs, orbital structures, alien architecture visible on and around them
  - Stars are surrounded by visible technology and activity
  - Earth's Spirit Realm layer shows cities and populated structures (spirits of the dead)
  - More alien ships visible throughout — fleets, stations, traffic
- Spirit Realm visual palette: same base scene + ethereal overlay (TBD: colour shift, extra luminosity, energy-field shimmer on structures)
- Architecturally: Spirit Realm is a second render pass / scene layer on top of the normal world — same coordinate system, additional geometry and effects

### How to Enter
Two methods (both unlocked through progression — unavailable at game start):
1. **Spirit Realm Technology**: acquired through alien tech gifts and Earth R&D — a drive system that shifts the ship between realms
2. **Alien Stargate**: a fixed-location gate opened by a friendly alien civilisation at a specific star system — player flies through it

### Enemies Crossing Realms
- Aggressive aliens travel between Spirit Realm and normal space
- Crossing visual effect: **Star Trek style** — shimmer, then materialisation
- Before the transition completes there is a brief sensor signature window

---

## Alien Civilisations

### Overview
The vast majority of intelligent life in this universe exists in or originates from the Spirit Realm. Alien ships are encountered in normal space primarily at **foreign star systems** — the further the player explores, the more civilisations they encounter.

### Factions

#### The Federation of Civilisations (80%)
- Peaceful, technologically advanced, open to exchange
- Approach the player with a text dialogue encounter (optional race portrait image on screen)
- Offer **technology gifts** and **information** (lore, star system data, navigation hints)
- Identified initially by their non-aggressive behaviour — **no visual difference from aggressive ships at game start**
- Later unlock: **energy intention detector** — a ship module revealing hostile/peaceful intent before contact

#### Aggressive Aliens (20%)
- Attack on sight or when provoked
- Origin: Spirit Realm; cross into normal space to raid or patrol
- Appear at foreign star systems; frequency increases in deep-space exploration
- Higher density of aggressive encounters near certain star clusters (TBD in lore sprint)

### Faction Reputation System

| State | Description | Consequence |
|---|---|---|
| **Neutral** | Default with all Federation races | Normal encounter dialogue, receive gifts |
| **Friendly** | Built through repeated positive encounters | Better gifts, mission access |
| **Hostile** | Triggered by attacking a Federation ship | No dialogue, attacked on sight by that race |
| **Blacklisted** | Attacking multiple races | Wide hostility across the Federation |

**Reputation Recovery**: Accept and complete missions from the offended race to restore standing. Mission types TBD (explored in a later design sprint).

**Reputation is saved** as part of the game save state (per-race reputation scores).

### Detection & Sensors
Ship detection is **physics-based**:
- Detectable signals: electromagnetic emissions, heat signature, drive light/exhaust
- A powered-down or low-emission ship is harder to detect
- Detection range and accuracy improve with technology upgrades
- **Weapons hot** = red alert triggered on enemy ships (and potentially your own if they can detect you)
- Early game: poor long-range detection — aliens can be close before being detected
- Late game: sensor upgrades extend detection range significantly

---

## Technology Progression

### Overview
The player starts with basic human technology. Through alien contact and Earth R&D, the tech tree expands — unlocking new ship capabilities, better sensors, Spirit Realm access, and more powerful ships.

### Tech Acquisition
| Source | How | Persistence |
|---|---|---|
| **Alien tech gifts** (Federation) | Received during dialogue encounters | Carried on ship — **lost if ship destroyed** |
| **Earth R&D** | Return to Earth with carried gifts; research converts them into permanent upgrades | Permanent — survives ship loss |
| **Mission rewards** | Completing alien missions | TBD |

### Tech Tree Categories (draft)
| Branch | Examples |
|---|---|
| Propulsion | Faster warp, lower energy warp cost, Spirit Realm drive |
| Sensors | Extended detection range, energy intention detector, Spirit Realm scan |
| Shields | Faster recharge, higher absorption, Spirit Realm shield variant |
| Weapons | Higher damage, new weapon types, anti-Spirit-Realm rounds |
| Hull | Higher max integrity, subsystem redundancy |
| Spirit Realm | Realm entry drive, realm navigation, realm communication |

### Earth R&D Loop
1. Explore foreign stars → receive alien tech gifts in dialogue
2. Return to Earth (or Sol) → deposit gifts into R&D queue
3. R&D converts gifts into permanent tech tree unlocks
4. New ship builds at Earth incorporate those unlocks
5. If ship is destroyed before returning: **carried gifts are lost** — only deposited tech persists

### Goal / Win Condition
Primarily sandbox. Progression paths:
- **Explorer path**: tech tree branches that enable travel to more distant stars and eventually Spirit Realm access
- **Combat path**: tech tree branches that maximise combat capability, weapon systems, and shield resilience
- **Diplomat path**: Federation reputation unlocks exclusive tech and missions

---

## Ship System

### Multiple Ships
Ships defined in `config/ships.json` — data-driven, no recompile needed. New ships built at Earth incorporate current tech tree state.

| Stat | Effect |
|---|---|
| Max Energy | Total energy pool size |
| Energy Regen Rate | Passive recharge speed |
| Warp Efficiency | Energy cost per AU / LY |
| Shield Strength | Absorption capacity |
| Weapon Power | Damage output |
| Agility | Turn rate, strafe |
| Sensor Range | Detection distance |
| Tech Slots | How many tech modules can be equipped |

---

### Energy Pool

| System | Draw Level |
|---|---|
| Antigravity Propulsion | Low (always on) |
| System Warp | Medium |
| Super Warp | Very High |
| Spirit Realm Drive | Very High (when unlocked) |
| Shield Bubble | Low–High (player-allocated) |
| Energy Weapons | Medium–High (player-allocated) |
| Sensors (active scan) | Low–Medium |

---

### Shield Bubble
- Star Trek model — many hits to deplete, fast recharge when energy allocated
- Translucent sphere, opacity = shield level
- At 0% shield: hull fully exposed
- Spirit Realm variant (future tech): deflects Spirit Realm-specific projectiles

---

### Ship Damage
- Hull: **0–100%**
- 0% = destruction → Game Over → lose carried tech gifts → option to reload save
- Subsystem degradation at thresholds (warp, shields, sensors degrade before total loss)
- Primary defence is shields; hull rarely takes damage until shields collapse

---

## HUD

### Flight HUD
- **Top center**: Flight mode + Realm indicator (NORMAL / SPIRIT REALM)
- **Top right**: Speed (auto-unit)
- **Left panel**: Nearest star, planet, alien ship — name, type, distance, faction (if known)
- **Right panel**: Energy gauges, allocation bars, shield %, hull %
- **Center alerts**: Gravity warning, low energy, warp lockout, **sensor contact**, **weapons hot / red alert**

### Solar System Minimap (lower left)
- Sun + planet orbital rings; ship as X marker
- Toggle: full system / inner system zoom
- Visible in Mode 2 and Mode 3 only

### Galaxy / Star Map (full-screen overlay)
- Local star cluster; filter: 10 / 50 / 100 LY
- Stars coloured by spectral class; exoplanet systems highlighted
- **Explored systems** marked; **alien contact made** systems flagged
- Click to set Super Warp target

### Alien Contact Panel
- Triggered on encounter with Federation alien
- Side panel or modal: race name, portrait image (when available), text dialogue
- Accept / Decline tech gift prompt
- Reputation indicator for that race

---

## Visual Generation

### Normal Space
- Stars: spectral color + magnitude size + bloom
- Skybox: NASA/ESA Milky Way panorama + procedural nebula/dust shader
- Planets: NASA textures + gas giant procedural bands + atmospheric scattering
- Binary stars: each component separate object

### Spirit Realm Layer
- All normal space visuals remain
- **Additional render pass** over the scene:
  - Planets gain orbital structures, energy hubs, alien architecture (procedural + asset-based)
  - Stars surrounded by visible civilisation activity (ships, stations, energy emissions)
  - Earth gains Spirit Realm city layer (lit structures on/above surface)
  - Ambient colour shift / ethereal luminosity overlay (exact palette TBD in visual design sprint)
  - Enemy/friendly ship density significantly higher
- Realm crossing effect: **Star Trek materialisation shimmer** (dissolve + energy burst shader)

### Ship & NPC Visuals
- Player ships: Meshy.ai / Tripo3D AI-generated + Blender cleanup
- Alien ships: separate models per faction type (silhouette-differentiated even before faction detector unlocked)
- Federation vs. aggressive: **no early visual difference** — later energy intention detector adds a HUD colour overlay

### LOD
- Low / Medium / High quality tiers in-game
- Start with minimal tier; benchmark in Sprint 1
- Spirit Realm layer adds a separate LOD toggle (spirit structures scale with quality setting)

---

## Time System
- In-game clock from real-world date at launch
- Drives ephemeris queries (planet positions)
- **Time warp**: 1x / 10x / 100x / 1000x / 10000x
- Disabled during combat or active warp travel

---

## Audio (Sprint 11+)

| Asset | Source |
|---|---|
| Ambient space | Meta AudioCraft / MusicGen |
| Spirit Realm ambient | Distinct soundscape — ethereal tone (AudioCraft) |
| Ship engine | Freesound.org or AudioCraft |
| Warp / realm transition | ElevenLabs Sound Effects or Stable Audio |
| Alien dialogue tone | AudioCraft (ambient underscore for contact scenes) |
| UI / combat | Freesound.org |

---

## Visual Assets

| Asset | Source |
|---|---|
| Planet textures | NASA Solar System Exploration (free) |
| Milky Way skybox | ESA/NASA panoramic (CC-licensed) |
| Player ship model | Meshy.ai / Tripo3D + Blender |
| Alien ship models | Meshy.ai / Tripo3D + Blender (per faction type) |
| Spirit Realm structures | Procedural shader + AI-generated base mesh |
| Race portrait images | Midjourney / DALL-E 3 |
| UI / HUD icons | Midjourney / DALL-E 3 → SVG |

---

## Technology Stack (see `TECH_STACK.md`)

| Layer | Choice |
|---|---|
| Language | C++17 |
| Build | CMake 3.20+ + vcpkg |
| Graphics | OpenGL 4.5 core + GLAD + GLFW 3 |
| Math | GLM (double precision) |
| UI / HUD | Dear ImGui (docking branch) |
| Data | nlohmann/json, fast-cpp-csv-parser, libcurl, stb_image |
| Testing | Google Test |
| CI | GitHub Actions |
| Audio (Sprint 11+) | OpenAL or miniaudio |

---

## Configuration System

All tunable values in JSON config:
- `config/game.json` — graphics, audio, controls, LOD, time warp options
- `config/ships.json` — all ship definitions (stats + tech slot layout)

New ships and tech upgrades added without recompiling.

---

## Development Methodology

### Workflow
1. **Tech Stack** → `TECH_STACK.md`
2. **Specs** → feature spec before any code
3. **Sprints** → acceptance criteria + unit tests
4. **QA Audit** → human review gate between sprints
5. **Next Sprint** → only after QA sign-off

### Sprint Roadmap

**— PHASE 1: GAME ENGINE —**
*Engine complete and QA-signed before any Phase 2 work begins.*

| Sprint | Epic | Focus |
|---|---|---|
| P1-S01 | EP-ENG-01 | Build system, CI, vcpkg ✅ DONE |
| P1-S02 | EP-ENG-01 | Window + OpenGL 4.5 context |
| P1-S03–S06 | EP-ENG-02 | ECS, config, input, logging |
| P1-S07–S09 | EP-ENG-03 | Shader/DSA, mesh/texture loading, camera interface |
| P1-S10–S11 | EP-ENG-04 | HUD framework (ImGui), audio backend (miniaudio) |
| P1-S12–S14 | EP-ENG-05 | State machine, save/load framework, time system |

> **PHASE 1 QA MILESTONE** — Smoke-test app (engine only, zero sim/game includes) must build and run. Owner sign-off gates Phase 2.

**— PHASE 2: STAR SIMULATION (Sandbox Mode) —**
*Simulation complete and QA-signed before any Phase 3 work begins.*

| Sprint | Epic | Focus |
|---|---|---|
| P2-S01 | EP-SIM-01 | Data download + validation (HYG, Horizons, exoplanets) |
| P2-S02–S03 | EP-SIM-02 | Star field: HYG data, spectral colour, magnitude, LOD |
| P2-S04 | EP-SIM-03 | Skybox: Milky Way panorama + nebula shader |
| P2-S05 | EP-SIM-04 | Solar System: planets, moons, orbital rings, NASA textures |
| P2-S06–S08 | EP-SIM-05 | All three flight modes (Mode 3, Mode 2, Mode 1) |
| P2-S09–S11 | EP-SIM-06 | Simulation HUD, minimap, galaxy/star map |
| P2-S12–S14 | EP-SIM-07 | Main menu, settings, save position, time warp |
| P2-S15 | EP-SIM-08 | Player ship model (AI-generated + Blender) |

> **PHASE 2 QA MILESTONE** — Full simulation playthrough required. Owner sign-off gates Phase 3.

**— PHASE 3: SPIRIT SPACE GAME (Game Mode) —**
*Adds all gameplay on top of the proven simulation.*

| Sprint | Epic | Focus |
|---|---|---|
| P3-S01–S03 | EP-GAME-01 | Energy pool, shields, hull damage, multi-ship config |
| P3-S04–S06 | EP-GAME-02 | Ship selector, full save/load, game over, pause menu |
| P3-S07–S09 | EP-GAME-03 | Energy weapons, physics-based detection, realm crossing |
| P3-S10–S11 | EP-GAME-04 | HUD game extensions, alien contact panel |
| P3-S12–S15 | EP-GAME-05 | Alien spawning, dialogue, reputation, intro narrative |
| P3-S16–S18 | EP-GAME-06 | Tech tree, gift inventory, Earth R&D loop |
| P3-S19–S21 | EP-GAME-07 | Spirit Realm render layer, content, entry mechanics |
| P3-S22–S23 | EP-GAME-08 | Full audio: ambient, flight, combat, Spirit Realm |
| P3-S24–S25 | EP-GAME-09 | Alien ship models, race portraits, HUD icons |
| P3-S26 | — | Full integration polish, LOD tuning, QA pass |
| P3-S27+ | — | Future: third-person camera, landing hook, satellite toggle |

### Architecture Notes
- **Engine boundary**: `engine/` headers contain zero game/sim domain types; enforced by the Phase 1 smoke-test
- **Spirit Realm**: second render pass toggle — same coordinate system, no spatial engine duplication
- **Landing**: gravity zone collision hooks in place from sim flight sprint
- **Third-person camera**: ICamera interface in place from Phase 1 — swap implementation, no flight code changes
- **New ships**: fully data-driven via `config/ships.json`
- **New alien races**: data-driven faction definitions (TBD config schema)

---

## Open Design Questions (to iterate on later)

- [ ] Spirit Realm visual palette — exact colour shift / ethereal effect style
- [ ] Stargate locations — fixed known systems or procedurally placed?
- [ ] Mission types for reputation recovery
- [ ] Alien race roster — how many, what archetypes?
- [ ] Spirit Realm combat differences vs. normal space
- [ ] Multiplayer (likely out of scope — not designed in)
- [ ] Lore depth — how much story vs. sandbox

---

## Conventions

- Human review gates surfaced before proceeding
- Agents and skills used to minimise token usage
- No feature without spec + acceptance criteria
- Real data over hardcoded values
- Config is single source of truth for tunable values
- All coordinate math uses double precision

---

*Document version: 0.5 — 2026-04-12*
