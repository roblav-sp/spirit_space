# DONE — EP-ENG-03-S01: Shader System & DSA Helpers

## Files Created

### Engine headers
- `engine/include/engine/render/Shader.h`
- `engine/include/engine/render/GpuBuffer.h`

### Engine implementation
- `engine/src/render/Shader.cpp`
- `engine/src/render/GpuBuffer.cpp`

### Test shaders
- `src/shaders/test_quad.vert`
- `src/shaders/test_quad.frag`

### Unit tests
- `tests/render/test_shader_compile.cpp`
- `tests/render/test_shader_missing_file_error.cpp`
- `tests/render/test_dsa_buffer_upload.cpp`

## Tests Written

| Test suite | Test name | Description |
|---|---|---|
| ShaderCompile | LoadValidShaders | Load test_quad.vert + test_quad.frag; isValid() == true |
| ShaderCompile | BindUnbind | bind() / unbind() no crash; programId() > 0 |
| ShaderMissing | MissingVertFile | nonexistent paths return false / isValid() false (no context needed) |
| ShaderMissing | NoCrashOnInvalidShader | Broken GLSL temp file returns false without crash |
| DsaBuffer | UploadAndId | 12 floats uploaded; id() > 0; isValid() true |
| DsaBuffer | VertexArrayValid | VertexArray constructed; id() > 0; isValid() true |
| DsaBuffer | DestructorCleans | Buffer + VAO destroyed in nested scope; no crash |

All context-requiring tests call `GTEST_SKIP()` if `window.isContextValid()` is false.

## Compilation Status

All files compiled individually with:
```
x86_64-w64-mingw32-g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror
```

| File | Status |
|---|---|
| engine/src/render/Shader.cpp | OK |
| engine/src/render/GpuBuffer.cpp | OK |
| tests/render/test_shader_compile.cpp | OK |
| tests/render/test_shader_missing_file_error.cpp | OK |
| tests/render/test_dsa_buffer_upload.cpp | OK |

Full `make build/bin/render_tests.exe` link also succeeded.

## DSA Compliance

All buffer and VAO operations use DSA exclusively:
- `glCreateBuffers` / `glNamedBufferData` (no glGenBuffers / glBindBuffer for ops)
- `glCreateVertexArrays` / `glVertexArrayVertexBuffer` / `glEnableVertexArrayAttrib` / `glVertexArrayAttribFormat` / `glVertexArrayAttribBinding`
- No legacy bind-then-set pattern anywhere in engine code

## Issues / Notes

- `EXPECT_NO_FATAL_FAILURE` does not accept multi-statement blocks under MinGW gtest; replaced with a plain nested scope + `SUCCEED()` macro.
- `MissingVertFile` does not require a GL context (fails at std::ifstream), so it runs on CI without skipping.
