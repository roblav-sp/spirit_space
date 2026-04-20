#include <gtest/gtest.h>

// glad MUST precede GLFW — see engine/src/core/Window.cpp for explanation.
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine/core/Window.h"
#include "engine/hud/HudSystem.h"

// ── EP-ENG-04-S01 — HUD ImGui Init Tests ─────────────────────────────────────
//
// Tests that require an OpenGL context call GTEST_SKIP() when the context
// cannot be created (expected on CI runners without a compatible GPU driver).

namespace {

engine::Window::Config hiddenConfig()
{
    engine::Window::Config cfg;
    cfg.width   = 800;
    cfg.height  = 600;
    cfg.title   = "hud-test-window";
    cfg.visible = false;
    return cfg;
}

} // namespace

// ── HudInit.CreatesContext ────────────────────────────────────────────────────

TEST(HudInit, CreatesContext)
{
    engine::Window window(hiddenConfig());

    if (!window.isContextValid()) {
        GTEST_SKIP() << "OpenGL 4.5 context unavailable "
                        "(no compatible GPU driver — expected on CI)";
    }

    engine::HudSystem hud(window.handle());
    EXPECT_TRUE(hud.isReady());
}

// ── HudInit.DestructorCleans ──────────────────────────────────────────────────

TEST(HudInit, DestructorCleans)
{
    engine::Window window(hiddenConfig());

    if (!window.isContextValid()) {
        GTEST_SKIP() << "OpenGL 4.5 context unavailable "
                        "(no compatible GPU driver — expected on CI)";
    }

    // HudSystem goes out of scope here — must not crash or assert.
    EXPECT_NO_FATAL_FAILURE({
        engine::HudSystem hud(window.handle());
        static_cast<void>(hud.isReady());
    });
}
