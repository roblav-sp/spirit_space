# Spirit Space — Claude Code Instructions

This file is loaded automatically at the start of every session.
Read it fully before doing any work. It replaces the need to re-read all project docs every session.

---

## Project in One Paragraph

Spirit Space is a C++17 / OpenGL 4.5 space simulation game using real astronomical data.
Built in **two phases**: Phase 1 = standalone space simulation (Sandbox Mode, no gameplay).
Phase 2 = game layer on top (Game Mode: aliens, combat, energy, Spirit Realm).
Phase 2 does not begin until Phase 1 passes owner QA sign-off.
Tech stack is **fully locked** — never ask about library choices.
All requirements trace to `REQUIREMENTS.md`. All sprint specs live in `EPICS.md`.

---

## Key Documents — What Lives Where

| Question | File |
|---|---|
| What are we building? | `PROJECT_SUMMARY.md` |
| What are the requirements? | `REQUIREMENTS.md` |
| What sprint are we on? What is the acceptance criteria? | `EPICS.md` |
| What libraries do we use and why? | `TECH_STACK.md` |
| Old sprint list (superseded by EPICS.md) | `SPRINTS.md` |

**Do not re-read all docs every session.** Use memory + these pointers. Only read a doc if the task directly requires it.

---

## Tech Stack — Locked, Never Ask Again

```
Language:      C++17
Build:         CMake 3.20+ + vcpkg
Graphics:      OpenGL 4.5 core (DSA) + GLFW 3 + GLAD + GLM (double precision)
Entities:      EnTT  (ECS — all world objects: stars, planets, ships, projectiles)
Models:        Assimp  (OBJ/FBX/GLTF loader)
UI/HUD:        Dear ImGui  (docking branch)
Noise:         FastNoise2
Audio:         miniaudio  (single header)
Data:          nlohmann/json, fast-cpp-csv-parser, libcurl, stb_image
Testing:       Google Test + Python 3 data scripts
CI:            GitHub Actions
```

---

## Conventions — Follow Without Being Asked

- All spatial math uses `double` precision (interstellar scale requires it)
- World origin = Sol `(0,0,0)`. Units: AU (solar system), LY (interstellar)
- All entity types use EnTT. No inheritance hierarchy for game objects
- Component names prefixed with `C`: `CPosition`, `CEnergy`, `CShield`, etc.
- Config is the source of truth for all tunable values — no magic numbers in code
- `config/game.json` = graphics, audio, controls
- `config/ships.json` = all ship definitions (data-driven, no recompile)
- Shaders in `src/shaders/`, named `<purpose>.vert` / `<purpose>.frag` / `<purpose>.glsl`
- Unit tests mirror source structure: `src/render/StarRenderer.cpp` → `tests/render/test_star_renderer.cpp`
- Phase 1 code must have zero Phase 2 includes/dependencies (clean separation)

---

## Sprint Workflow — Follow This Every Sprint

1. Read the sprint spec from `EPICS.md` (just that sprint — not the whole file)
2. Confirm acceptance criteria with owner before writing any code
3. Implement — write unit tests alongside code, not after
4. All unit tests pass on CI before marking sprint done
5. Present the sprint's QA audit checklist to the owner
6. **Wait for owner sign-off before starting the next sprint**
7. Commit with `/commit` skill

Never start the next sprint without explicit owner approval.
Never implement Phase 2 features during Phase 1 sprints.

---

## Token-Saving Rules

### Use Agents — Don't Search Manually

| Task | Use This |
|---|---|
| "Where is X implemented?" or "Find all uses of Y" | `Explore` agent (subagent_type=Explore) |
| "What's the best way to implement X in OpenGL/EnTT?" | `general-purpose` agent for research |
| "Plan how to implement this sprint" | `Plan` agent before writing code |
| "Review this code for quality" | `/simplify` skill |
| "Commit this work" | `/commit` skill |

**Never use Bash grep/find when Grep/Glob tools or an Explore agent will do.**
**Never research library APIs inline — spawn a general-purpose agent.**

### Read Minimally

- Use `offset` + `limit` on Read tool — never read a whole large file if you only need a section
- Use Grep to find the exact line before reading context around it
- `EPICS.md` is long — Grep for the sprint ID (e.g. `EP-02-S01`) then read ±30 lines

### Batch Tool Calls

- Always run independent tool calls in parallel in one message
- Example: reading a header + its implementation file → one message, two Read calls
- Example: checking 3 candidate files → one message, three Glob/Grep calls

### Avoid Repeating Context

- Tech stack decisions are locked — never explain them again unless owner asks
- Sprint requirements are in `EPICS.md` — never ask the owner to repeat them
- Architecture decisions (ECS, double precision, config-driven ships) are permanent — apply them silently

### Write Code Efficiently

- Write the full implementation in one pass — avoid "let me start with X then we'll add Y"
- Unit tests go in the same session as the code — don't leave them for a follow-up
- Don't add comments to unchanged code, don't add error handling for impossible cases
- Config values go in `config/` files — never hardcode them even temporarily

---

## Agent Usage Guide

### When to Spawn an Explore Agent
- Searching for a pattern across more than 3 files
- Finding all places a component type is used
- Answering "how does X work in the current codebase?"
- Any search where you're not sure exactly where to look

### When to Spawn a Plan Agent
- Before starting any sprint with more than 2 deliverables
- When a sprint involves a new subsystem (first time touching EnTT, first shader, first audio)
- When a requirement has ambiguous implementation paths

### When to Spawn a General-Purpose Agent
- Researching OpenGL techniques (instanced rendering, bloom pass, etc.)
- Looking up EnTT API usage patterns
- Researching Assimp loading patterns for a new format
- Any external knowledge needed that isn't in the codebase

### When NOT to Spawn an Agent
- Simple single-file reads → use Read directly
- Known file locations → use Glob/Grep directly
- Small targeted edits → use Edit directly
- The overhead of spawning costs more than doing it inline

---

## Phase Awareness

**Always know which phase we are in before writing any code.**

| Phase | Mode | Includes | Excludes |
|---|---|---|---|
| 1 | Sandbox | Rendering, flight, HUD, save position, time warp | Energy, shields, damage, aliens, tech tree, Spirit Realm |
| 2 | Game | Everything in Phase 1 + all gameplay | Nothing — full game |

If a Phase 1 sprint is active and a feature belongs to Phase 2, note it as a future hook and move on. Do not implement it.

---

## QA Gate Behaviour

At the end of every sprint, present the QA checklist from `EPICS.md` for that sprint.
Do not proceed to the next sprint until the owner responds with sign-off.
The Phase 1 QA Milestone is a hard gate — present the full Phase 1 checklist and wait for explicit approval before any Phase 2 work begins.

---

## What NOT to Do

- Do not ask about tech stack choices — they are locked
- Do not implement Phase 2 features in Phase 1 sprints
- Do not start a sprint without reading its spec from `EPICS.md`
- Do not start the next sprint without owner QA sign-off
- Do not add features beyond the sprint's acceptance criteria
- Do not add error handling for internal invariants that cannot fail
- Do not add docstrings or comments to code you didn't change
- Do not hardcode values that belong in config files
- Do not use `float` for world-space coordinates — always `double`
- Do not search files with Bash grep — use the Grep tool
