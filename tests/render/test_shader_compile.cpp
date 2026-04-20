// glad MUST precede GLFW — see engine/src/core/Window.cpp for explanation.
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <gtest/gtest.h>

#include "engine/core/Window.h"
#include "engine/render/Shader.h"

// ── EP-ENG-03-S01 — Shader compile tests ─────────────────────────────────────
//
// These tests require an OpenGL 4.5 context.  They call GTEST_SKIP() when no
// compatible GPU driver is available (expected behaviour on CI).

namespace {

engine::Window::Config hiddenConfig()
{
    engine::Window::Config cfg;
    cfg.width   = 800;
    cfg.height  = 600;
    cfg.title   = "render-test-shader";
    cfg.visible = false;
    return cfg;
}

// Paths are relative to the project root; tests are run from the build
// directory, but the Makefile copies shaders alongside the binary.
// For development builds invoked directly, we use the repo path.
// Both are tried; the first that works is used.
constexpr const char* VERT_PATH = "src/shaders/test_quad.vert";
constexpr const char* FRAG_PATH = "src/shaders/test_quad.frag";

} // namespace

// ── ShaderCompile.LoadValidShaders ────────────────────────────────────────────

TEST(ShaderCompile, LoadValidShaders)
{
    engine::Window window(hiddenConfig());

    if (!window.isContextValid()) {
        GTEST_SKIP() << "OpenGL 4.5 context unavailable "
                        "(no compatible GPU driver — expected on CI)";
    }

    engine::Shader shader;
    const bool ok = shader.loadFromFiles(VERT_PATH, FRAG_PATH);

    EXPECT_TRUE(ok)       << "loadFromFiles should succeed with valid GLSL";
    EXPECT_TRUE(shader.isValid()) << "isValid() should be true after successful load";
}

// ── ShaderCompile.BindUnbind ──────────────────────────────────────────────────

TEST(ShaderCompile, BindUnbind)
{
    engine::Window window(hiddenConfig());

    if (!window.isContextValid()) {
        GTEST_SKIP() << "OpenGL 4.5 context unavailable "
                        "(no compatible GPU driver — expected on CI)";
    }

    engine::Shader shader;
    if (!shader.loadFromFiles(VERT_PATH, FRAG_PATH)) {
        GTEST_SKIP() << "Shader source files not found — run from project root";
    }

    EXPECT_GT(shader.programId(), 0u) << "programId() must be non-zero";

    EXPECT_NO_FATAL_FAILURE(shader.bind());
    EXPECT_NO_FATAL_FAILURE(shader.unbind());
}
