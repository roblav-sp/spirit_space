// glad MUST precede GLFW — see engine/src/core/Window.cpp for explanation.
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <gtest/gtest.h>

#include <cstdio>
#include <fstream>
#include <string>

#include "engine/core/Window.h"
#include "engine/render/Shader.h"

// ── EP-ENG-03-S01 — Shader error path tests ──────────────────────────────────
//
// MissingVertFile does NOT need a GL context — the failure happens at file I/O.
// NoCrashOnInvalidShader DOES need a context to attempt compilation.

namespace {

engine::Window::Config hiddenConfig()
{
    engine::Window::Config cfg;
    cfg.width   = 800;
    cfg.height  = 600;
    cfg.title   = "render-test-missing";
    cfg.visible = false;
    return cfg;
}

/// Write text to a temporary file and return its path.
/// The caller is responsible for deleting the file after use.
std::string writeTempFile(const std::string& content)
{
    // Use a fixed name in the system temp dir to avoid any platform weirdness
    // with tmpnam under MinGW.
    static int counter = 0;
    const std::string path = std::string("/tmp/spirit_bad_shader_")
                             + std::to_string(counter++) + ".glsl";
    std::ofstream f(path);
    f << content;
    return path;
}

} // namespace

// ── ShaderMissing.MissingVertFile ─────────────────────────────────────────────
//
// No GL context required — failure is at std::ifstream open.

TEST(ShaderMissing, MissingVertFile)
{
    engine::Shader shader;
    const bool ok = shader.loadFromFiles("nonexistent.vert", "nonexistent.frag");

    EXPECT_FALSE(ok)               << "loadFromFiles should return false for missing files";
    EXPECT_FALSE(shader.isValid()) << "isValid() must be false after failed load";
}

// ── ShaderMissing.NoCrashOnInvalidShader ──────────────────────────────────────
//
// Write intentionally broken GLSL to temp files and verify that loadFromFiles
// returns false without crashing.

TEST(ShaderMissing, NoCrashOnInvalidShader)
{
    engine::Window window(hiddenConfig());

    if (!window.isContextValid()) {
        GTEST_SKIP() << "OpenGL 4.5 context unavailable "
                        "(no compatible GPU driver — expected on CI)";
    }

    // Broken vertex shader — syntax error.
    const std::string badVertPath = writeTempFile(
        "#version 450 core\n"
        "THIS IS NOT VALID GLSL AT ALL @@@ !!!\n"
    );
    // Minimal valid fragment shader (the vert error should abort before this).
    const std::string okFragPath = writeTempFile(
        "#version 450 core\n"
        "out vec4 fragColor;\n"
        "void main() { fragColor = vec4(1.0); }\n"
    );

    engine::Shader shader;
    const bool ok = shader.loadFromFiles(badVertPath, okFragPath);

    // Clean up temp files.
    std::remove(badVertPath.c_str());
    std::remove(okFragPath.c_str());

    EXPECT_FALSE(ok)               << "loadFromFiles should return false for broken GLSL";
    EXPECT_FALSE(shader.isValid()) << "isValid() must be false after failed compile";
}
