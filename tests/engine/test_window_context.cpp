#include <gtest/gtest.h>

// glad MUST precede GLFW — see engine/src/core/Window.cpp for explanation.
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine/core/Window.h"

// ── EP-ENG-01-S02 — Window & Context Tests ───────────────────────────────────
//
// Tests that require an OpenGL 4.5 driver call GTEST_SKIP() when the context
// cannot be created.  This is the expected behaviour on CI runners (windows-latest)
// which use the Microsoft software renderer and do not support GL 4.5 core.
//
// On a developer machine with a discrete GPU the tests run in full.
// "SKIPPED" is not a failure — ctest exits 0.

namespace {

engine::Window::Config hiddenConfig()
{
    engine::Window::Config cfg;
    cfg.width   = 800;
    cfg.height  = 600;
    cfg.title   = "engine-test-window";
    cfg.visible = false;  // headless — no OS window shown
    return cfg;
}

} // namespace

// ── Test: GL version ≥ 4.5 ───────────────────────────────────────────────────

TEST(WindowContext, GlVersion45)
{
    engine::Window window(hiddenConfig());

    if (!window.isContextValid()) {
        GTEST_SKIP() << "OpenGL 4.5 context unavailable "
                        "(no compatible GPU driver — expected on CI)";
    }

    // GLVersion is a global struct populated by gladLoadGLLoader.
    // It always reflects the version negotiated during context creation.
    const int major = GLVersion.major;
    const int minor = GLVersion.minor;

    EXPECT_GE(major, 4)
        << "Expected OpenGL major version >= 4, got " << major;

    if (major == 4) {
        EXPECT_GE(minor, 5)
            << "Expected OpenGL minor version >= 5 (when major == 4), got "
            << minor;
    }
}

// ── Test: fullscreen toggle does not crash ────────────────────────────────────

TEST(WindowContext, FullscreenToggleNoCrash)
{
    engine::Window window(hiddenConfig());

    if (!window.isContextValid()) {
        GTEST_SKIP() << "OpenGL 4.5 context unavailable "
                        "(no compatible GPU driver — expected on CI)";
    }

    EXPECT_TRUE(window.isOpen());

    // Toggle into fullscreen and immediately back — must not crash or assert.
    // A hidden window can still call glfwSetWindowMonitor safely on Windows.
    EXPECT_NO_FATAL_FAILURE(window.toggleFullscreen());
    EXPECT_NO_FATAL_FAILURE(window.toggleFullscreen());

    EXPECT_TRUE(window.isOpen());
}
