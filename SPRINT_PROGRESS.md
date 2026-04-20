# Sprint Progress — Spirit Space Phase 1

---

## EP-ENG-01 — Foundation  ⏳ IN PROGRESS

### S01 — Build System & Repository

| Criterion | Status | Notes |
|---|---|---|
| Build system works | ✅ | Migrated from CMake → Makefile (`make` builds `spirit_space.exe`) |
| vcpkg manifest present | ✅ | `vcpkg.json` with glad, glfw3, glm, gtest, miniaudio |
| GTest integrated | ✅ | `build/bin/spirit_space_tests.exe` builds and runs |
| CI pipeline exists | ✅ | `.github/workflows/ci.yml` updated for MinGW/Makefile |
| CI green on push | ⏳ | Needs a push to confirm — ci.yml was just updated |
| QA Gate: owner builds on own machine | ✅ | `make` confirmed working in WSL |

### S02 — Window & OpenGL Context

| Criterion | Status | Notes |
|---|---|---|
| `engine/src/core/Window.cpp` delivered | ✅ | Done |
| GLFW + GLAD via vcpkg | ✅ | `x64-mingw-static` triplet |
| 1920×1080 window opens | ⏳ | Binary built — needs run on Windows |
| Red triangle renders | ⏳ | Binary built — needs run on Windows |
| Fullscreen toggle works (F11) | ⏳ | Binary built — needs run on Windows |
| Closes cleanly (ESC) | ⏳ | Binary built — needs run on Windows |
| `test_window_context` passes | ⏳ | `engine_tests.exe` built — needs run on Windows |
| `test_fullscreen_toggle` no crash | ⏳ | `engine_tests.exe` built — needs run on Windows |
| QA Gate: owner launches app and sees window | ⏳ | Pending owner sign-off |

---

## EP-ENG-04-S02 — Audio Backend  ⏳ AWAITING QA SIGN-OFF

### Deliverables

| Item | Status |
|---|---|
| `vcpkg.json` — miniaudio added | ✅ |
| `engine/include/engine/audio/AudioSystem.h` | ✅ |
| `engine/src/audio/AudioSystem.cpp` | ✅ |
| `tests/engine/audio/test_audio_system.cpp` | ✅ |
| `Makefile` — audio sources + audio test target | ✅ |
| `src/main.cpp` — P key plays/stops beep | ✅ |
| `assets/audio/test_beep.wav` — 440Hz test tone | ✅ |
| All binaries build cleanly | ✅ |

### Acceptance Criteria

| Criterion | Status | Notes |
|---|---|---|
| Audio file plays | ⏳ | Needs run on Windows |
| Volume control works | ✅ | `setVolume` + clamping tested in unit tests |
| No crash on missing file | ✅ | `loadClip` returns false, no throw |
| API generic (no game audio types) | ✅ | Only `std::string` id + `float` volume |
| QA Gate: owner hears beep on P keypress | ⏳ | Pending owner sign-off |

---

## How to run QA

```bash
# Copy to Windows and run:
build/bin/spirit_space.exe

# Expected:
# - 1920x1080 window opens with dark-navy background and red triangle
# - Console prints OpenGL version and "Audio: ready | P: play/stop beep"
# - P key plays a 440Hz beep; press again to stop
# - F11 toggles fullscreen
# - ESC closes the window cleanly
```
