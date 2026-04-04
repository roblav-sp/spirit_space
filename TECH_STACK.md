# Spirit Space — Technology Stack

> Status: **Draft — pending final selection**
> Last updated: 2026-04-04

This document captures the candidate technologies, evaluation criteria, and final decisions for the Spirit Space tech stack. Decisions are made per subsystem.

---

## Evaluation Criteria

| Criterion | Weight | Notes |
|---|---|---|
| C++ compatibility | High | Must integrate cleanly with C++17/20 |
| Active maintenance | High | No abandoned libraries |
| License | High | MIT/Apache/BSD preferred |
| Performance | High | Real-time rendering target 60fps |
| Ease of integration | Medium | CMake-friendly, header-only preferred |
| Community / docs | Medium | Debugging support |

---

## Subsystems & Candidates

---

### 1. Language Standard

| Option | Notes |
|---|---|
| **C++17** | Broad compiler support, std::filesystem, structured bindings |
| **C++20** | Modules, coroutines, ranges — may complicate toolchain |

**Recommendation**: C++17 for stability; opt-in C++20 features where supported.

---

### 2. Graphics API

| Option | Notes |
|---|---|
| **OpenGL 4.5+** | Mature, cross-platform, DSA support, well-documented for space rendering |
| Vulkan | More control, far more boilerplate — overkill for solo/small team |
| Direct3D | Windows-only, excluded |

**Decision**: **OpenGL 4.5** with core profile. Use Direct State Access (DSA) throughout.

---

### 3. Window / Context Creation

| Option | Notes |
|---|---|
| **GLFW 3** | Lightweight, excellent OpenGL context support, cross-platform |
| SDL2 | Heavier; also handles audio/input — useful if we add audio later |
| SDL3 | New, less community Q&A |

**Recommendation**: **GLFW 3** initially. Revisit if audio integration becomes complex.

---

### 4. OpenGL Loading

| Option | Notes |
|---|---|
| **GLAD** | Generated loader, header-only, widely used |
| GLEW | Older, less flexible |

**Decision**: **GLAD** (OpenGL 4.5 core profile).

---

### 5. Mathematics

| Option | Notes |
|---|---|
| **GLM** | Header-only, mirrors GLSL types, vectors/matrices/quaternions |
| Eigen | More general linear algebra, heavier |

**Decision**: **GLM 0.9.9+**. Eigen available if needed for orbital mechanics.

---

### 6. UI / HUD

| Option | Notes |
|---|---|
| **Dear ImGui** | Immediate-mode, integrates directly with OpenGL/GLFW, fast for dev |
| Qt | Heavy, license concerns |
| Custom | Too much work for a HUD |

**Decision**: **Dear ImGui** with OpenGL3 + GLFW backend.

---

### 7. Data Parsing

| Library | Format | Notes |
|---|---|---|
| **nlohmann/json** | JSON | Header-only, excellent ergonomics |
| **fast-cpp-csv-parser** | CSV | Header-only, used for HYG/NASA CSV datasets |
| **libcurl** | HTTP | For optional live API calls to NASA Horizons |
| **tinyxml2** | XML | Fallback if any datasets use XML |

**Decision**: All four as needed. All header-only or small footprint.

---

### 8. Asset / Texture Loading

| Option | Notes |
|---|---|
| **stb_image** | Header-only, handles PNG/JPG/TGA — sufficient for planet textures |
| DevIL | Heavier, older |

**Decision**: **stb_image**.

---

### 9. Build System

| Option | Notes |
|---|---|
| **CMake 3.20+** | Industry standard, vcpkg/conan compatible, cross-platform |
| Premake | Less universal |
| Meson | Growing but smaller ecosystem |

**Decision**: **CMake 3.20+** with **vcpkg** for dependency management.

---

### 10. Testing

| Type | Library | Notes |
|---|---|---|
| Unit Tests | **Google Test (GTest)** | Mature, CMake integration via FetchContent |
| QA Audit | Custom checklist | Human-reviewed per sprint |
| Data validation | Custom scripts (Python 3) | Validate downloaded datasets before ingestion |

**Decision**: GTest for all unit tests. QA audit is a structured Markdown checklist reviewed by the project owner at each sprint boundary.

---

### 11. Shader Tools

| Tool | Purpose |
|---|---|
| GLSL (built-in) | All shaders written in GLSL 450 |
| **spirv-cross** (optional) | If we ever need SPIR-V for Vulkan migration |
| **glslangValidator** | Offline shader validation in CI |

---

### 12. Version Control & CI

| Tool | Purpose |
|---|---|
| **Git** | Source control |
| **GitHub** | Remote repository, issue tracking |
| **GitHub Actions** | CI: build + unit tests on push |

---

## Dependency Summary (Final Candidates)

```
Core:
  - C++17
  - CMake 3.20+
  - vcpkg

Graphics:
  - OpenGL 4.5 (core profile)
  - GLFW 3
  - GLAD (generated for GL 4.5)
  - GLM

UI:
  - Dear ImGui (docking branch)

Data:
  - nlohmann/json
  - fast-cpp-csv-parser
  - libcurl
  - stb_image

Testing:
  - Google Test

Validation:
  - Python 3 (data scripts)
  - glslangValidator
```

---

## Open Decisions

- [ ] SDL2 vs GLFW — confirm no audio needed in Sprint 0–3
- [ ] Shader hot-reload strategy for development iteration
- [ ] Procedural planet texture approach (noise lib: FastNoise2 or stb_perlin)
- [ ] Audio library for future sprints (OpenAL / miniaudio)
- [ ] Exoplanet API strategy: bulk download vs. live API

---

*All decisions subject to revision after Sprint 0 prototype.*
