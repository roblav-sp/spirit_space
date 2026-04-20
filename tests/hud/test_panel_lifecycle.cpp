#include <gtest/gtest.h>

// glad MUST precede GLFW — see engine/src/core/Window.cpp for explanation.
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine/core/Window.h"
#include "engine/hud/HudSystem.h"

// ── EP-ENG-04-S01 — Panel Lifecycle Tests ────────────────────────────────────
//
// Tests that require an OpenGL context call GTEST_SKIP() when the context
// cannot be created (expected on CI runners without a compatible GPU driver).

namespace {

engine::Window::Config hiddenConfig()
{
    engine::Window::Config cfg;
    cfg.width   = 800;
    cfg.height  = 600;
    cfg.title   = "hud-panel-test-window";
    cfg.visible = false;
    return cfg;
}

} // namespace

// ── PanelLifecycle.RegisterPanel ─────────────────────────────────────────────

TEST(PanelLifecycle, RegisterPanel)
{
    engine::Window window(hiddenConfig());

    if (!window.isContextValid()) {
        GTEST_SKIP() << "OpenGL 4.5 context unavailable "
                        "(no compatible GPU driver — expected on CI)";
    }

    engine::HudSystem hud(window.handle());

    hud.registerPanel("test", []() {});

    EXPECT_TRUE(hud.hasPanel("test"));
    EXPECT_EQ(hud.panelCount(), static_cast<std::size_t>(1));
}

// ── PanelLifecycle.UnregisterPanel ───────────────────────────────────────────

TEST(PanelLifecycle, UnregisterPanel)
{
    engine::Window window(hiddenConfig());

    if (!window.isContextValid()) {
        GTEST_SKIP() << "OpenGL 4.5 context unavailable "
                        "(no compatible GPU driver — expected on CI)";
    }

    engine::HudSystem hud(window.handle());

    hud.registerPanel("test", []() {});
    hud.unregisterPanel("test");

    EXPECT_FALSE(hud.hasPanel("test"));
    EXPECT_EQ(hud.panelCount(), static_cast<std::size_t>(0));
}

// ── PanelLifecycle.RenderPanelCalled ─────────────────────────────────────────

TEST(PanelLifecycle, RenderPanelCalled)
{
    engine::Window window(hiddenConfig());

    if (!window.isContextValid()) {
        GTEST_SKIP() << "OpenGL 4.5 context unavailable "
                        "(no compatible GPU driver — expected on CI)";
    }

    engine::HudSystem hud(window.handle());

    bool called = false;
    hud.registerPanel("flag-panel", [&called]() {
        called = true;
    });

    hud.beginFrame();
    hud.renderPanels();
    hud.endFrame();

    EXPECT_TRUE(called);
}

// ── PanelLifecycle.DuplicateRegisterOverwrites ────────────────────────────────

TEST(PanelLifecycle, DuplicateRegisterOverwrites)
{
    engine::Window window(hiddenConfig());

    if (!window.isContextValid()) {
        GTEST_SKIP() << "OpenGL 4.5 context unavailable "
                        "(no compatible GPU driver — expected on CI)";
    }

    engine::HudSystem hud(window.handle());

    hud.registerPanel("test", []() {});
    hud.registerPanel("test", []() {});  // second registration — same name

    EXPECT_TRUE(hud.hasPanel("test"));
    EXPECT_EQ(hud.panelCount(), static_cast<std::size_t>(1));
}
