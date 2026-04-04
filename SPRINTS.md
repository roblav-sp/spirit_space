# Spirit Space — Sprint Plan

> Status: **Draft**
> Last updated: 2026-04-04

Each sprint has a goal, feature specs, acceptance criteria, required unit tests, and a QA audit gate. No sprint begins until the previous sprint's QA audit is signed off by the project owner.

---

## Sprint 0 — Foundation

**Goal**: Project infrastructure, build pipeline, data download, empty window renders.

### Features
- CMake project with vcpkg integration
- GLFW + GLAD OpenGL 4.5 window (1920×1080, core profile)
- OpenGL triangle render test (smoke test)
- Data download scripts (HYG star database, NASA Exoplanet Archive CSV)
- Data validation scripts (Python 3) — schema check, null checks, outlier report
- GitHub repository + Actions CI (build + test on push)
- Google Test integration (empty test suite passes)

### Acceptance Criteria
- [ ] `cmake --build` succeeds on Windows with MSVC or MinGW
- [ ] Application opens a window, renders a red triangle, closes cleanly
- [ ] HYG CSV downloaded and validated (119k+ records, no critical nulls)
- [ ] Exoplanet CSV downloaded and validated
- [ ] GitHub Actions green on main branch
- [ ] GTest runs and reports 0 failures

### Unit Tests
- `test_data_loader`: parse HYG CSV, assert record count > 100000
- `test_data_loader`: parse exoplanet CSV, assert required columns present
- `test_window`: OpenGL context created successfully (mock-able)

### QA Audit Gate — Sprint 0
**Human review required before Sprint 1 begins.**
- [ ] Owner reviews build output on their machine
- [ ] Owner reviews downloaded data sample (first 10 rows of each dataset)
- [ ] Owner confirms GitHub repo and CI are set up correctly

---

## Sprint 1 — Star Field

**Goal**: Render the HYG star catalog as a 3D point cloud with correct colors and sizes.

### Features
- Star data ingestion: parse RA/Dec/distance/spectral class/magnitude
- Convert RA/Dec/dist to 3D Cartesian coordinates
- OpenGL point sprite rendering for stars
- Star color from spectral class (OBAFGKM → color lookup table)
- Star apparent size from magnitude (brighter = larger point)
- Bloom/glow pass (simple Gaussian blur on bright points)
- Orbit camera (mouse drag rotate, scroll zoom) for development inspection

### Acceptance Criteria
- [ ] All HYG stars rendered within 1000 ly radius
- [ ] Star colors visually match spectral class reference chart
- [ ] Bright stars (magnitude < 3) visually dominate the field
- [ ] FPS ≥ 60 with full star field on target hardware
- [ ] No GPU memory errors (validated with RenderDoc or similar)

### Unit Tests
- `test_spectral_color`: each class O/B/A/F/G/K/M returns correct RGB range
- `test_coordinate_conversion`: RA/Dec/dist → XYZ round-trip within epsilon
- `test_star_loader`: magnitude values clamped to expected render range

### QA Audit Gate — Sprint 1
**Human review required.**
- [ ] Owner visually inspects star field — recognizable constellations present
- [ ] Owner confirms color distribution looks correct
- [ ] Performance confirmed on owner's hardware

---

## Sprint 2 — Solar System

**Goal**: Render the Solar System with real planet positions from NASA Horizons.

### Features
- Horizons ephemeris data download (offline snapshot + optional live update)
- Sun rendered as a large glowing sphere at origin
- 8 planets rendered with correct relative positions and sizes (scaled for visibility)
- Planet color/type from data (rocky vs gas giant visual distinction)
- Labels: planet names displayed in world space
- Scale mode: toggle between realistic and exaggerated scale

### Acceptance Criteria
- [ ] All 8 planets visible and positioned correctly relative to Sun
- [ ] Planet sizes visually proportional (with scale toggle working)
- [ ] Labels always face camera (billboarding)
- [ ] Orbit paths drawn as ellipses (using Keplerian elements)
- [ ] Ephemeris date matches a known reference date (verified against NASA website)

### Unit Tests
- `test_ephemeris_parser`: parse Horizons output, extract XYZ positions
- `test_kepler`: compute orbit ellipse points from a/e/i, compare against reference
- `test_planet_scale`: scaled radii within expected visual range

### QA Audit Gate — Sprint 2
**Human review required.**
- [ ] Owner verifies planet layout matches a reference (NASA Eyes screenshot)
- [ ] Owner confirms orbit paths look correct
- [ ] Labels readable at camera distances used in testing

---

## Sprint 3 — Planetary Flight (Mode 3)

**Goal**: First-person ship flight near a planet with keyboard controls.

### Features
- Ship entity with position, orientation, velocity
- 6-DOF antigravity movement (no gravity constraint)
- Keyboard controls: WASD (forward/back/strafe), QE (roll), RF (up/down)
- Mouse look for camera orientation
- Damped acceleration — smooth start/stop, no instant velocity change
- Dead-zone and response curve tuning
- Basic ship model (placeholder cube or simple mesh)
- Planet surface visible at close range (texture from stb_image)

### Acceptance Criteria
- [ ] Ship moves in all 6 axes with keyboard
- [ ] Controls feel smooth (damping coefficients configurable in config file)
- [ ] No clipping through planet surface (simple sphere collision)
- [ ] Planet texture visible at orbital altitude
- [ ] FPS ≥ 60 during planetary flight

### Unit Tests
- `test_ship_physics`: apply thrust, assert velocity accumulates correctly
- `test_damping`: velocity converges to zero when no thrust applied
- `test_collision`: sphere intersection test planet surface

### QA Audit Gate — Sprint 3
**Human review required.**
- [ ] Owner flies around Earth — controls feel right
- [ ] Owner confirms damping parameters (may request tuning)
- [ ] No clipping issues observed in 5 minutes of play

---

## Sprint 4 — System Warp (Mode 2)

**Goal**: Warp between planets within a star system.

### Features
- Warp target selector UI (ImGui overlay, list of planets)
- Warp acceleration curve (fast acceleration → cruise → deceleration to stop)
- Automatic orientation toward target at warp start
- Speed readout in AU/s on HUD
- Warp lockout zone (cannot engage within X km of a planet)
- Energy cost deducted on warp (stub energy system)

### Acceptance Criteria
- [ ] Player can select any planet and warp to it
- [ ] Ship decelerates and stops at safe orbit distance from target
- [ ] Cannot overshoot (guaranteed stop)
- [ ] Lockout zone prevents warp too close to planet
- [ ] Energy stub deducted correctly

### Unit Tests
- `test_warp_curve`: acceleration/deceleration profile integrates to correct displacement
- `test_warp_lockout`: warp blocked inside minimum radius
- `test_energy_deduction`: correct energy subtracted for given distance

### QA Audit Gate — Sprint 4
**Human review required.**
- [ ] Owner warps Earth → Mars, confirms arrival position and feel
- [ ] Owner tests lockout zone behavior
- [ ] Owner confirms energy drain is visible in HUD stub

---

## Sprint 5 — Super Warp (Mode 1)

**Goal**: Interstellar travel — warp to another star system.

### Features
- Star selection UI (search by name or click in star field)
- Super Warp sequence: charge animation → tunnel effect → arrival
- Arrival at target star's outer system boundary
- Energy cost — very high; potentially requires full energy pool
- Camera transitions between flight modes seamlessly

### Acceptance Criteria
- [ ] Player can select a named star and initiate super warp
- [ ] Warp tunnel visual effect plays for ~3 seconds
- [ ] Player arrives near correct star
- [ ] Energy pool fully or nearly depleted after super warp
- [ ] Mode transition (interstellar → system) is seamless

### Unit Tests
- `test_star_lookup`: search by name returns correct HYG record
- `test_super_warp_energy`: energy cost formula returns expected value for given distance
- `test_mode_transition`: state machine transitions correctly

### QA Audit Gate — Sprint 5
**Human review required.**
- [ ] Owner warps to Alpha Centauri, Sirius, and a random distant star
- [ ] Tunnel effect looks acceptable
- [ ] Owner confirms energy is meaningfully depleted

---

## Sprint 6 — Energy & Shield Systems

**Goal**: Full energy pool implementation with manual allocation UI.

### Features
- Energy pool with passive regeneration rate
- Energy allocation slider UI: Shield / Weapons / Warp reserve
- Shield bubble renderer (translucent sphere, opacity = shield level)
- Shield absorbs hits (debris/collisions drain shield energy)
- Ship stat readouts: current energy, regen rate, shield %, warp reserve

### Acceptance Criteria
- [ ] Energy drains and regenerates correctly across all systems
- [ ] Slider changes allocation in real time
- [ ] Shield bubble visible and opacity changes with allocation
- [ ] Collision with debris drains shield instead of destroying ship
- [ ] Ship cannot engage warp if warp reserve energy < threshold

### Unit Tests
- `test_energy_pool`: drain + regen over time, assert conservation
- `test_shield_absorption`: collision applies correct drain formula
- `test_allocation`: changing slider recalculates per-system budgets

### QA Audit Gate — Sprint 6
**Human review required.**
- [ ] Owner reviews energy allocation UI — clarity and responsiveness
- [ ] Owner deliberately drains shield and observes behavior
- [ ] Owner confirms warp lockout when energy is low

---

## Sprint 7 — HUD Polish & Exoplanet Integration

**Goal**: Complete HUD and integrate exoplanet catalog.

### Features
- Contextual info panel: nearest star + planet (name, type, distance, spectral class)
- Full energy/shield/warp gauges
- Flight mode indicator
- Exoplanet data integrated: when near a star with known exoplanets, HUD lists them
- Exoplanet rendering (approximate position, size from radius data)

### Acceptance Criteria
- [ ] HUD updates in real time as player moves
- [ ] Nearest star shown within 1 frame of crossing distance threshold
- [ ] Exoplanets visible near their host stars
- [ ] No HUD elements overlap or clip at 1920×1080 and 2560×1440

### Unit Tests
- `test_nearest_star`: spatial query returns correct nearest star from test set
- `test_hud_layout`: all elements within screen bounds at multiple resolutions
- `test_exoplanet_position`: computed position matches reference for known system

### QA Audit Gate — Sprint 7
**Human review required.**
- [ ] Full playthrough: spawn → fly → warp → arrive → inspect HUD
- [ ] Owner reviews exoplanet display for known systems (Kepler-452, TRAPPIST-1)
- [ ] Sign-off on HUD design (may request visual changes)

---

## Sprint 8 — Weapons & Energy Combat

**Goal**: Energy weapons competing with shield/warp for energy budget.

*Detailed spec to be written when Sprint 7 is complete.*

---

## Future Sprints (9+)

- Audio (engine hum, warp sound, ambient space)
- Procedural planet surface textures (FastNoise2)
- Asteroid fields and debris simulation
- Additional game mechanics (TBD by project owner)
- Save/load system

---

*Sprint plan is a living document. Sprints may be split or merged based on QA audit outcomes.*
