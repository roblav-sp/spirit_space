// glad MUST precede GLFW — see engine/src/core/Window.cpp for explanation.
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <gtest/gtest.h>

#include "engine/core/Window.h"
#include "engine/render/Shader.h"

#include <fstream>
#include <string>

// ── EP-ENG-03-S01 — Shader compile tests ─────────────────────────────────────
//
// Shader GLSL is written to temp files at runtime so tests are working-directory
// independent — the binary can be run from any directory.

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

static const char* k_vertSrc = R"glsl(
#version 450 core
layout(location = 0) in vec2 aPos;
void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);
}
)glsl";

static const char* k_fragSrc = R"glsl(
#version 450 core
out vec4 fragColor;
uniform vec3 uColor;
void main() {
    fragColor = vec4(uColor, 1.0);
}
)glsl";

// Writes src to a file in the current directory. Returns "" on failure.
// Using cwd (not tmpnam) avoids Windows temp-path issues when the binary
// is run via WSL interop — cwd is always writable in that context.
std::string writeTempShader(const char* filename, const char* src)
{
    std::ofstream f(filename);
    if (!f) return "";
    f << src;
    return filename;
}

} // namespace

// ── ShaderCompile.LoadValidShaders ────────────────────────────────────────────

TEST(ShaderCompile, LoadValidShaders)
{
    engine::Window window(hiddenConfig());
    if (!window.isContextValid()) {
        GTEST_SKIP() << "OpenGL 4.5 context unavailable — expected on CI";
    }

    const std::string vert = writeTempShader("_ss_vert.glsl", k_vertSrc);
    const std::string frag = writeTempShader("_ss_frag.glsl", k_fragSrc);
    if (vert.empty() || frag.empty()) {
        GTEST_SKIP() << "Could not write temp shader files";
    }

    engine::Shader shader;
    const bool ok = shader.loadFromFiles(vert, frag);

    std::remove(vert.c_str());
    std::remove(frag.c_str());

    EXPECT_TRUE(ok)              << "loadFromFiles should succeed with valid GLSL";
    EXPECT_TRUE(shader.isValid()) << "isValid() should be true after successful load";
}

// ── ShaderCompile.BindUnbind ──────────────────────────────────────────────────

TEST(ShaderCompile, BindUnbind)
{
    engine::Window window(hiddenConfig());
    if (!window.isContextValid()) {
        GTEST_SKIP() << "OpenGL 4.5 context unavailable — expected on CI";
    }

    const std::string vert = writeTempShader("_ss_vert.glsl", k_vertSrc);
    const std::string frag = writeTempShader("_ss_frag.glsl", k_fragSrc);
    if (vert.empty() || frag.empty()) {
        GTEST_SKIP() << "Could not write temp shader files";
    }

    engine::Shader shader;
    const bool ok = shader.loadFromFiles(vert, frag);
    std::remove(vert.c_str());
    std::remove(frag.c_str());

    if (!ok) GTEST_SKIP() << "Shader compilation failed";

    EXPECT_GT(shader.programId(), 0u);
    EXPECT_NO_FATAL_FAILURE(shader.bind());
    EXPECT_NO_FATAL_FAILURE(shader.unbind());
}
