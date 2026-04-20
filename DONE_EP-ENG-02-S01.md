# EP-ENG-02-S01: ECS Layer — Completion Report

## Files Created

| File | Purpose |
|---|---|
| `engine/include/engine/ecs/World.h` | World class header — thin EnTT registry wrapper |
| `engine/src/ecs/World.cpp` | World implementation (createEntity, destroyEntity, entityCount, clear) |
| `tests/ecs/test_entity_lifecycle.cpp` | Entity create/destroy/clear lifecycle tests |
| `tests/ecs/test_component_add_remove.cpp` | Component add/get/has/remove tests |
| `tests/ecs/test_view_query.cpp` | View query tests (single, multi-component, empty, post-clear) |

## Tests Written

### test_entity_lifecycle.cpp
- `EntityLifecycle.CreateAndDestroy` — 10 000 entities created then destroyed; entityCount == 0
- `EntityLifecycle.ClearResetsCount` — clear() brings entityCount to 0
- `EntityLifecycle.DocumentedBehaviourDestroyOnlyLiveEntities` — documents that EnTT asserts on invalid destroy; no code tests it

### test_component_add_remove.cpp
Components: `CPosition {double x,y,z}`, `CHealth {float current,max}`, `CVelocity {double vx,vy,vz}`
- `ComponentAddRemove.AddAndGet` — add CPosition, verify stored values via getComponent
- `ComponentAddRemove.HasComponent` — add CHealth → has=true; remove → has=false
- `ComponentAddRemove.MultipleComponents` — all 3 components on one entity, each accessible
- `ComponentAddRemove.AddComponentReplacesExisting` — addComponent on existing component updates value (emplace_or_replace)

### test_view_query.cpp
- `ViewQuery.ViewSingleComponent` — 100 entities with CPosition → view iterates exactly 100
- `ViewQuery.ViewMultipleComponents` — 50 with CPosition+CVelocity, 50 with only CPosition → view<CPosition,CVelocity> yields 50
- `ViewQuery.ViewEmpty` — no entities → view yields 0, no crash
- `ViewQuery.ViewAfterClear` — after clear(), view yields 0

## Compilation Status

All files compiled with:
```
x86_64-w64-mingw32-g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror
```

| File | Result |
|---|---|
| `engine/src/ecs/World.cpp` | PASS |
| `tests/ecs/test_entity_lifecycle.cpp` | PASS |
| `tests/ecs/test_component_add_remove.cpp` | PASS |
| `tests/ecs/test_view_query.cpp` | PASS |

## Issues / Notes

- `entt::registry::alive()` does not exist in this EnTT version (3.x). Entity count is obtained via `registry.storage<entt::entity>()->free_list()`, which returns the number of live entity slots.
- The const overload of `storage<T>()` returns a pointer (not a reference); the non-const overload returns a reference. `entityCount()` is const, so `->` is required.
- All spatial component fields use `double` precision per project convention.
- No Phase 2 or Phase 3 domain types appear in any engine header.
