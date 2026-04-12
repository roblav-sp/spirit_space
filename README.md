# Spirit Space

A real-universe space simulation game built on real astronomical data — and the reusable C++ game engine behind it.

---

## What it is

Spirit Space lets you pilot an antigravity ship through an accurate representation of the observable universe: 119,000+ real stars from the HYG database, all eight planets at their correct live positions from NASA/JPL Horizons ephemeris data, and true interstellar distances at double precision. No fictional star map — you navigate the actual sky.

The project is built in three phases:

| Phase | What gets built | Status |
|---|---|---|
| **1 — Game Engine** | Reusable C++ engine: ECS, renderer, audio, input, config, HUD, save system | In progress |
| **2 — Star Simulation** | Sandbox mode proving the engine: real stars, solar system, all flight modes | Pending |
| **3 — Spirit Space Game** | Full game: energy, combat, aliens, tech tree, Spirit Realm dimension | Pending |

Phase 1 produces a standalone engine (`engine/`) that can power future games independently of this project.

---

## Building

### Requirements

| Tool | Version |
|---|---|
| Windows | 10 or 11 (64-bit) |
| Visual Studio | 2019 or 2022 (MSVC, with C++ Desktop workload) |
| CMake | 3.20 or later |
| vcpkg | Any recent version |

### Steps

**1. Clone the repository**

```
git clone https://github.com/<your-handle>/spirit_space.git
cd spirit_space
```

**2. Install dependencies via vcpkg**

```
vcpkg install --triplet x64-windows
```

Dependencies are declared in `vcpkg.json` and installed automatically. No manual library setup needed.

**3. Configure**

```
cmake -B build -G "Visual Studio 16 2019" -A x64 ^
  -DCMAKE_TOOLCHAIN_FILE=<path-to-vcpkg>/scripts/buildsystems/vcpkg.cmake ^
  -DVCPKG_TARGET_TRIPLET=x64-windows
```

Replace `<path-to-vcpkg>` with wherever vcpkg is installed (e.g. `C:/vcpkg`).

**4. Build**

```
cmake --build build --config Release
```

The executable is placed at `build/bin/Release/spirit_space.exe`.

**5. Run**

```
build/bin/Release/spirit_space.exe
```

Config files from `config/` are copied next to the executable automatically on every build.

---

## Controls (Phase 2 — Simulation)

| Key | Action |
|---|---|
| W / S | Thrust forward / backward |
| A / D | Strafe left / right |
| Q / E | Roll left / right |
| R / F | Thrust up / down |
| Mouse | Look |
| SPACE | Engage warp |
| M | Toggle galaxy map |
| . / , | Time warp up / down |
| ESC | Pause / quit |
| F11 | Toggle fullscreen |

All bindings are remappable in `config/game.json` or via the in-game Settings screen.

---

## Running tests

```
ctest --test-dir build -C Release --output-on-failure
```

Tests that require an OpenGL 4.5 GPU driver are automatically skipped (`SKIPPED`, not `FAILED`) on machines without a compatible driver. This is expected behaviour on CI.

---

## Project structure

```
engine/                 Phase 1 — reusable game engine (zero game domain code)
  include/engine/       Public headers — link engine and get these for free
  src/                  Implementation
config/                 Runtime config — source of truth for all tunable values
  game.json             Graphics, audio, controls, flight parameters
  ships.json            All ship definitions (add ships here, no recompile needed)
scripts/                Data download + validation scripts (Phase 2)
src/                    Application entry point (main.cpp)
tests/
  test_smoke.cpp        Build system + C++17 smoke tests (always pass, no GPU needed)
  engine/               Engine unit tests (skip gracefully if no GPU driver)
```

---

## Tech stack

| Layer | Choice | Notes |
|---|---|---|
| Language | C++17 | MSVC + MinGW compatible |
| Build | CMake 3.20+ + vcpkg | Manifest mode, reproducible deps |
| Graphics | OpenGL 4.5 core (DSA) | No legacy bind-to-edit patterns |
| Window | GLFW 3 | Lightweight, OpenGL-native |
| GL loader | GLAD | Generated for 4.5 core profile |
| Math | GLM | Header-only, double precision for world space |
| Entities | EnTT | ECS — all world objects are entities |
| Models | Assimp | OBJ / FBX / GLTF loader |
| UI / HUD | Dear ImGui (docking branch) | In-game HUD and overlays |
| Noise | FastNoise2 | Procedural nebula and terrain |
| Audio | miniaudio | Single-header, no external deps |
| Data | nlohmann/json, fast-cpp-csv-parser, libcurl, stb_image | Config, CSV star data, HTTP, textures |
| Testing | Google Test 1.14 | Unit tests mirroring source structure |
| CI | GitHub Actions | Windows MSVC, every push to master |

All stack decisions are locked. See `TECH_STACK.md` for full rationale.

---

## Configuration

`config/game.json` is the single source of truth for all tunable values — graphics quality, audio volumes, control bindings, flight damping, LOD distances, time warp multipliers. Edit it directly or use the in-game Settings screen.

`config/ships.json` defines all ship types. Add a new ship object to the array and it appears in-game without recompiling.

---

## Data sources (Phase 2)

| Source | Data |
|---|---|
| HYG Star Database | 119,614 stars — position, spectral class, magnitude |
| NASA/JPL Horizons | Live ephemeris for all Solar System bodies |
| NASA Exoplanet Archive | Confirmed exoplanet catalog |
| ESA Gaia DR3 | High-precision star astrometry |
| NASA Solar System Exploration | Planet and moon textures |
| ESA/NASA Milky Way panorama | Skybox base image (CC-licensed) |

Data is downloaded at build time by `scripts/download_data.py` and validated by `scripts/validate_data.py`. No manual download steps.

---

## Coordinate system

- **Origin**: Sol at `(0, 0, 0)`
- **Units**: AU within the Solar System; light-years at interstellar scale
- **Precision**: `double` throughout — required for interstellar distances without floating-point drift
- **Reference frame**: J2000 ecliptic plane

---

## Sprints and requirements

| Document | Contents |
|---|---|
| `EPICS.md` | Full sprint plan — acceptance criteria and unit tests per sprint |
| `REQUIREMENTS.md` | Complete product requirements (REQ-ENGINE, REQ-RENDER, REQ-FLIGHT, …) |
| `PROJECT_SUMMARY.md` | Vision, lore, game design, flight mechanics, HUD, Spirit Realm |
| `TECH_STACK.md` | Library decisions with rationale |

---

## CI

GitHub Actions runs on every push to `master`:

1. Bootstrap and cache vcpkg packages
2. CMake configure + MSVC Release build
3. `ctest` — all tests, output on failure
4. GLSL shader validation via `glslangValidator` (once shaders are added in Phase 1)

Badge: see Actions tab on GitHub.
