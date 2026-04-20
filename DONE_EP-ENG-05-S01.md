# EP-ENG-05-S01 — Game State Machine — DONE

## Files Created

### Engine headers
- `engine/include/engine/core/IGameState.h`
- `engine/include/engine/core/GameStateMachine.h`

### Engine implementation
- `engine/src/core/GameStateMachine.cpp`

### Unit tests
- `tests/core/test_state_push_pop.cpp`   (5 tests: PushOne, PopOne, PushTwo, PopToFirst, PopEmptyIsSafe)
- `tests/core/test_state_lifecycle_hooks.cpp` (7 tests: EnterOnPush, ExitOnPop, EnterExitExactlyOnce, PauseOnSecondPush, ResumeOnPop, UpdateOnlyTop, ReplaceExitEnter)
- `tests/core/test_empty_stack_safe.cpp` (5 tests: UpdateNoCrash, TopReturnsNullptr, IsEmptyTrue, SizeZero, PopNoCrash)

## Compilation Status

All four compilation units passed with `-std=c++17 -Wall -Wextra -Wpedantic -Werror` targeting `x86_64-w64-mingw32-g++`:

| File | Result |
|---|---|
| `engine/src/core/GameStateMachine.cpp` | OK |
| `tests/core/test_state_push_pop.cpp` | OK |
| `tests/core/test_state_lifecycle_hooks.cpp` | OK |
| `tests/core/test_empty_stack_safe.cpp` | OK |

## Issues
None.
