# Spirit Space — Technology Stack

> Status: **Finalised**
> Last updated: 2026-04-04

All decisions are locked. Rationale recorded per subsystem.

---

## Evaluation Criteria

| Criterion | Weight | Notes |
|---|---|---|
| C++ compatibility | High | Must integrate cleanly with C++17 |
| Active maintenance | High | No abandoned libraries |
| License | High | MIT/Apache/BSD only — no GPL |
| Performance | High | Real-time rendering target 60 FPS |
| Ease of integration | Medium | CMake/vcpkg friendly, header-only preferred |
| Community / docs | Medium | Debugging support matters |

---

## Decisions

---

### 1. Language Standard
**Decision: C++17**

| Option | Notes |
|---|---|
| **C++17** ✓ | Broad compiler support, std::filesystem, structured bindings |
| C++20 | Modules/coroutines may complicate toolchain — deferred |

Rationale: Stability over cutting-edge features. All target compilers (MSVC, MinGW) have solid C++17 support.

---

### 2. Graphics API
**Decision: OpenGL 4.5 — core profile, Direct State Access (DSA) throughout**

| Option | Notes |
|---|---|
| **OpenGL 4.5** ✓ | Mature, cross-platform, DSA, well-documented for space rendering |
| Vulkan | More control, far more boilerplate — overkill for this team size |
| Direct3D | Windows-only, excluded |

Rationale: OpenGL 4.5 DSA removes the old bind-to-edit pattern and produces clean, modern code without Vulkan's setup overhead.

---

### 3. Window & Context Creation
**Decision: GLFW 3**

| Option | Notes |
|---|---|
| **GLFW 3** ✓ | Lightweight, excellent OpenGL context support, cross-platform |
| SDL2 | Heavier; audio/input included but unnecessary given miniaudio choice |
| SDL3 | Too new, smaller community Q&A |

Rationale: GLFW is the leanest path to an OpenGL window. Audio handled separately by miniaudio.

---

### 4. OpenGL Function Loading
**Decision: GLAD (generated for OpenGL 4.5 core profile)**

| Option | Notes |
|---|---|
| **GLAD** ✓ | Generated loader, header-only, widely used |
| GLEW | Older, less flexible, doesn't support DSA cleanly |

---

### 5. Mathematics
**Decision: GLM 0.9.9+ with double precision enabled**

| Option | Notes |
|---|---|
| **GLM** ✓ | Header-only, mirrors GLSL types, vectors/matrices/quaternions |
| Eigen | More general — available as fallback for complex orbital mechanics if needed |

Rationale: GLM maps directly to GLSL shader types, reducing coordinate-system bugs. Double precision required for interstellar-scale coordinates without floating-point drift.

---

### 6. Entity System
**Decision: EnTT**

| Option | Notes |
|---|---|
| **EnTT** ✓ | Header-only, MIT, one of the fastest ECS implementations, used in Minecraft Bedrock |
| Custom simple component system | Less powerful; would need to be rebuilt as scope grew |

Rationale: Spirit Space has heterogeneous entity types at large scale — 119,000 stars, planets, alien ships, projectiles, Spirit Realm structures. EnTT keeps all entities of the same component type in contiguous memory (cache-friendly), handles the full scale without restructuring, and adding new entity types requires no new classes — only new component combinations.

Component plan (initial):
- `CPosition` — world-space double-precision XYZ
- `CVelocity` — movement vector
- `CMesh` — GPU mesh handle (VAO, vertex count)
- `CSpectralClass` — star colour and luminosity
- `COrbitalData` — Keplerian elements for planets/moons
- `CEnergy` — pool, regen rate, allocations (Phase 2)
- `CShield` — level, absorption capacity (Phase 2)
- `CHull` — integrity % (Phase 2)
- `CFaction` — Federation / Aggressive / Player (Phase 2)
- `CSensor` — detection range (Phase 2)
- `CSpiritRealm` — flags entity as Spirit Realm layer (Phase 2)

---

### 7. UI / HUD
**Decision: Dear ImGui (docking branch)**

| Option | Notes |
|---|---|
| **Dear ImGui** ✓ | Immediate-mode, OpenGL + GLFW backend, fast for HUD and dev tools |
| Qt | Heavy, LGPL licence complications |
| Custom | Too much work |

Rationale: ImGui handles all 2D HUD overlays, menus, galaxy map panels, and tech tree UI. The docking branch allows floating/docked panels for the minimap and alien contact panel.

---

### 8. 3D Model Loading
**Decision: Assimp**

| Option | Notes |
|---|---|
| **Assimp** ✓ | Handles OBJ, FBX, GLTF, DAE, 40+ formats — single unified API |
| tinyobjloader | Header-only but OBJ only — too limiting for AI tool exports + Blender GLTF workflow |

Rationale: AI model generation tools (Meshy.ai, Tripo3D) export OBJ today. Blender cleanup exports GLTF. Future tools may use FBX. Assimp loads all of them with identical code — one `importer.ReadFile()` call regardless of format. Eliminates the need to write or maintain any format parser. Installed via vcpkg.

---

### 9. Data Parsing
**Decision: nlohmann/json + fast-cpp-csv-parser + libcurl + tinyxml2**

| Library | Format | Notes |
|---|---|---|
| **nlohmann/json** ✓ | JSON | Header-only, excellent ergonomics — config files, save files |
| **fast-cpp-csv-parser** ✓ | CSV | Header-only — HYG and NASA Exoplanet Archive datasets |
| **libcurl** ✓ | HTTP | NASA/JPL Horizons API calls |
| **tinyxml2** ✓ | XML | Fallback — some agency datasets use XML |

---

### 10. Texture & Image Loading
**Decision: stb_image**

| Option | Notes |
|---|---|
| **stb_image** ✓ | Header-only, PNG/JPG/TGA — sufficient for all planet and ship textures |
| DevIL | Heavier, older |

---

### 11. Procedural Noise
**Decision: FastNoise2**

| Option | Notes |
|---|---|
| **FastNoise2** ✓ | Rich noise types (Perlin, Simplex, Cellular, Domain Warp), SIMD-accelerated, MIT |
| stb_perlin | Too limited for Spirit Realm structures and domain-warped nebulae |

Rationale: Used in two places — nebula/dust GLSL shader (skybox layer) and Spirit Realm structure generation. Domain warping produces organic-looking energy structures. FastNoise2's SIMD acceleration means procedural generation stays off the critical render path.

---

### 12. Audio
**Decision: miniaudio**

| Option | Notes |
|---|---|
| **miniaudio** ✓ | Single-header, MIT, zero dependencies, cross-platform, handles playback and mixing |
| OpenAL | More complex setup; overkill for current audio requirements |
| SDL2 audio | Would require replacing GLFW with SDL2 — unnecessary coupling |

Rationale: miniaudio is a single `.h` file drop-in. Handles ambient soundscapes, looping engine hum, one-shot SFX, and volume mixing. If 3D positional audio becomes necessary (enemy ships approaching from a direction), miniaudio supports it natively.

---

### 13. Build System & Package Manager
**Decision: CMake 3.20+ with vcpkg**

| Option | Notes |
|---|---|
| **CMake 3.20+** ✓ | Industry standard, vcpkg/conan compatible, cross-platform |
| **vcpkg** ✓ | Microsoft's package manager — manifest mode, integrates with CMake |
| Premake / Meson | Less universal ecosystem |

All libraries installed via `vcpkg.json` manifest. No manual DLL copying.

---

### 14. Testing
**Decision: Google Test (unit) + Python 3 (data validation) + custom QA checklists**

| Type | Tool | Notes |
|---|---|---|
| Unit tests | **Google Test** ✓ | CMake FetchContent integration, mature, CI-friendly |
| Data validation | **Python 3 scripts** ✓ | Schema checks, null checks, outlier detection on downloaded datasets |
| QA audit | **Markdown checklist** ✓ | Human-reviewed by owner at each sprint boundary |

---

### 15. Shader Tooling
**Decision: GLSL 450 + glslangValidator + hot-reload file watcher**

| Tool | Purpose |
|---|---|
| **GLSL 450** ✓ | All shaders — vertex, fragment, geometry |
| **glslangValidator** ✓ | Offline validation in CI — catches shader errors before runtime |
| **Hot-reload file watcher** ✓ | Custom ~50-line file timestamp check — reloads changed shaders without restart |
| spirv-cross | Optional — kept available if Vulkan migration ever considered |

Rationale: Hot-reload is essential during Spirit Realm visual development. Tweaking a nebula shader without restarting saves significant iteration time.

---

### 16. Version Control & CI
**Decision: Git + GitHub + GitHub Actions**

| Tool | Purpose |
|---|---|
| **Git** ✓ | Source control |
| **GitHub** ✓ | Remote repository, issue tracking |
| **GitHub Actions** ✓ | CI: build + unit tests + shader validation on every push to main |

---

## Full Dependency Summary

```
Language:
  C++17

Build & Packages:
  CMake 3.20+
  vcpkg (manifest mode)

Graphics:
  OpenGL 4.5 (core profile, DSA)
  GLFW 3
  GLAD (generated, GL 4.5 core)
  GLM 0.9.9+ (double precision)

Entity System:
  EnTT (header-only ECS)

3D Models:
  Assimp

UI / HUD:
  Dear ImGui (docking branch, OpenGL3 + GLFW backend)

Data:
  nlohmann/json
  fast-cpp-csv-parser
  libcurl
  tinyxml2
  stb_image

Procedural Generation:
  FastNoise2

Audio:
  miniaudio (single header)

Testing & Validation:
  Google Test
  Python 3
  glslangValidator

Version Control & CI:
  Git
  GitHub
  GitHub Actions
```

---

## Architecture Notes

- **EnTT** is the backbone of all world objects — stars, planets, ships, projectiles, Spirit Realm structures all live as entities with components. No class inheritance hierarchy for game objects.
- **Assimp** is used exclusively for loading external model files. All procedural geometry (star points, orbital rings, shield bubble, Spirit Realm overlays) is generated in code.
- **FastNoise2** runs CPU-side for Spirit Realm structure generation. Noise values are uploaded to GPU as textures or uniforms for the GLSL shaders to sample.
- **miniaudio** runs on its own thread — audio never touches the render loop.
- **Dear ImGui** renders after the 3D scene each frame (deferred to the end of the render pass).

---

## Resolved — No Open Decisions Remaining

All subsystem decisions are locked. Tech stack is ready for EP-01-S01 (build system setup).
