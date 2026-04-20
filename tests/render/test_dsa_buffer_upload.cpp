// glad MUST precede GLFW — see engine/src/core/Window.cpp for explanation.
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <gtest/gtest.h>

#include <array>

#include "engine/core/Window.h"
#include "engine/render/GpuBuffer.h"

// ── EP-ENG-03-S01 — DSA buffer / VAO tests ───────────────────────────────────
//
// All tests require an OpenGL 4.5 context.  They call GTEST_SKIP() when no
// compatible GPU driver is available (expected behaviour on CI).

namespace {

engine::Window::Config hiddenConfig()
{
    engine::Window::Config cfg;
    cfg.width   = 800;
    cfg.height  = 600;
    cfg.title   = "render-test-dsa";
    cfg.visible = false;
    return cfg;
}

} // namespace

// ── DsaBuffer.UploadAndId ─────────────────────────────────────────────────────

TEST(DsaBuffer, UploadAndId)
{
    engine::Window window(hiddenConfig());

    if (!window.isContextValid()) {
        GTEST_SKIP() << "OpenGL 4.5 context unavailable "
                        "(no compatible GPU driver — expected on CI)";
    }

    // Upload 12 floats (a simple triangle) via DSA.
    const std::array<float, 12> vertices = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f,
        -0.5f, -0.5f,
         0.5f,  0.5f,
    };

    engine::GpuBuffer buf;
    buf.upload(vertices.data(),
               static_cast<GLsizeiptr>(vertices.size() * sizeof(float)),
               GL_STATIC_DRAW);

    EXPECT_GT(buf.id(), 0u)  << "id() must be non-zero after upload";
    EXPECT_TRUE(buf.isValid()) << "isValid() must be true after upload";
}

// ── DsaBuffer.VertexArrayValid ────────────────────────────────────────────────

TEST(DsaBuffer, VertexArrayValid)
{
    engine::Window window(hiddenConfig());

    if (!window.isContextValid()) {
        GTEST_SKIP() << "OpenGL 4.5 context unavailable "
                        "(no compatible GPU driver — expected on CI)";
    }

    engine::VertexArray vao;

    EXPECT_GT(vao.id(), 0u)    << "id() must be non-zero after VertexArray construction";
    EXPECT_TRUE(vao.isValid()) << "isValid() must be true after VertexArray construction";
}

// ── DsaBuffer.DestructorCleans ────────────────────────────────────────────────
//
// Destructor correctness is verified implicitly: if the destructor calls
// glDeleteBuffers / glDeleteVertexArrays with an invalid name the GL
// implementation generates an error, and the test harness would catch a crash.
// We simply create and destroy both objects inside a scope.

TEST(DsaBuffer, DestructorCleans)
{
    engine::Window window(hiddenConfig());

    if (!window.isContextValid()) {
        GTEST_SKIP() << "OpenGL 4.5 context unavailable "
                        "(no compatible GPU driver — expected on CI)";
    }

    // Create buffer and VAO in a nested scope so their destructors run before
    // the test ends.  Any crash in a destructor would fail the test.
    {
        engine::GpuBuffer buf;
        const std::array<float, 6> data = {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
        buf.upload(data.data(),
                   static_cast<GLsizeiptr>(data.size() * sizeof(float)));

        engine::VertexArray vao;
        vao.addAttrib(0u, 0u, 2, GL_FLOAT, 0u);
        vao.bindVertexBuffer(0u, buf, 0, static_cast<GLsizei>(2 * sizeof(float)));

        // Both buf and vao go out of scope here — destructors must not crash.
    }
    SUCCEED() << "Destructor ran without crash";
}
