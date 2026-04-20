# EP-ENG-02-S02 — Config System — Done

## Files Created

| File | Role |
|---|---|
| `engine/include/engine/core/Config.h` | Singleton Config class declaration with templated get/set |
| `engine/src/core/Config.cpp` | load/save/traverse implementation |
| `tests/core/test_config_read.cpp` | ConfigRead test suite (4 tests) |
| `tests/core/test_config_missing_key_default.cpp` | ConfigMissing test suite (3 tests) |
| `tests/core/test_config_write_persist.cpp` | ConfigWrite test suite (3 tests) |

## Tests Written

### ConfigRead (test_config_read.cpp)
- `ReadString` — get<std::string> returns correct string value
- `ReadInt` — get<int> returns correct integer
- `ReadDouble` — get<double> returns correct double
- `ReadNested` — dot-notation "graphics.width" traversal returns correct value

### ConfigMissing (test_config_missing_key_default.cpp)
- `MissingKeyReturnsDefault` — get<int>("nonexistent", 42) returns 42
- `NestedMissingReturnsDefault` — get<float>("a.b.c", 1.0f) returns 1.0f
- `CorruptFileNoCrash` — invalid JSON sets isLoaded()==false, get still returns default

### ConfigWrite (test_config_write_persist.cpp)
- `SetAndGet` — set value, get it back in-memory without saving
- `WriteAndReload` — set, save, reload, verify value persists
- `SetNestedAndPersist` — dot-notation set, save, reload, verify persists

## Compilation Status

All four .cpp files compiled with zero warnings under:
```
x86_64-w64-mingw32-g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror
```

- `engine/src/core/Config.cpp` → PASS
- `tests/core/test_config_read.cpp` → PASS
- `tests/core/test_config_missing_key_default.cpp` → PASS
- `tests/core/test_config_write_persist.cpp` → PASS

## Design Notes

- Meyer's singleton: `static Config& instance()` — thread-safe in C++17.
- `load()` wraps `json::parse` in try/catch; corrupt/missing file leaves `m_loaded = false`, no exception escapes.
- `get<T>` wraps the full traversal in try/catch — a type mismatch at the leaf also returns the default, never throws.
- `set<T>` creates intermediate JSON objects as needed via `traverseOrCreate`.
- `save()` writes with `json::dump(4)` (4-space indent). No-op if path is empty.
- Tests use unique `/tmp/` file paths per test case to work safely with the singleton across all test cases.

## Issues

None.
