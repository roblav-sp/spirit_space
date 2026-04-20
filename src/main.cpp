// EP-ENG-01-S02: Window & OpenGL Context
// EP-ENG-04-S02: Audio Backend demo (P = play/stop beep)
//
// Shader system deferred to EP-ENG-03-S01 (inline GLSL for now).

// glad MUST precede GLFW — see engine/src/core/Window.cpp for explanation.
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine/core/Window.h"
#include "engine/audio/AudioSystem.h"

#include <array>
#include <iostream>

// ── Inline GLSL (temporary — EP-ENG-03-S01 replaces with file-based system) ──

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
void main() {
    fragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
)glsl";

// ── Minimal shader helpers ────────────────────────────────────────────────────

static GLuint compileShader(GLenum type, const char* src)
{
    GLuint sh = glCreateShader(type);
    glShaderSource(sh, 1, &src, nullptr);
    glCompileShader(sh);

    GLint ok = GL_FALSE;
    glGetShaderiv(sh, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[512] = {};
        glGetShaderInfoLog(sh, static_cast<GLsizei>(sizeof(log)), nullptr, log);
        std::cerr << "[Shader] Compile error:\n" << log << '\n';
        glDeleteShader(sh);
        return 0;
    }
    return sh;
}

static GLuint buildProgram()
{
    const GLuint vert = compileShader(GL_VERTEX_SHADER,   k_vertSrc);
    const GLuint frag = compileShader(GL_FRAGMENT_SHADER, k_fragSrc);
    if (!vert || !frag) {
        glDeleteShader(vert);
        glDeleteShader(frag);
        return 0;
    }

    GLuint prog = glCreateProgram();
    glAttachShader(prog, vert);
    glAttachShader(prog, frag);
    glLinkProgram(prog);

    // Shaders detach+delete after linking — the program owns them.
    glDetachShader(prog, vert);
    glDetachShader(prog, frag);
    glDeleteShader(vert);
    glDeleteShader(frag);

    GLint ok = GL_FALSE;
    glGetProgramiv(prog, GL_LINK_STATUS, &ok);
    if (!ok) {
        char log[512] = {};
        glGetProgramInfoLog(prog, static_cast<GLsizei>(sizeof(log)), nullptr, log);
        std::cerr << "[Shader] Link error:\n" << log << '\n';
        glDeleteProgram(prog);
        return 0;
    }
    return prog;
}

// ── Entry point ───────────────────────────────────────────────────────────────

int main()
{
    engine::Window::Config cfg;
    cfg.title      = "Spirit Space v0.1.0  |  ESC: quit  |  F11: fullscreen";
    cfg.width      = 1920;
    cfg.height     = 1080;
    cfg.fullscreen = false;
    cfg.visible    = true;

    engine::Window window(cfg);

    if (!window.isContextValid()) {
        std::cerr << "Fatal: could not create an OpenGL 4.5 core context.\n";
        return 1;
    }

    std::cout << "Spirit Space v0.1.0\n";
    std::cout << "Phase 1 — Game Engine\n";
    std::cout << "OpenGL:  " << glGetString(GL_VERSION)  << '\n';
    std::cout << "Vendor:  " << glGetString(GL_VENDOR)   << '\n';
    std::cout << "GLSL:    " << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';

    // ── Audio backend (EP-ENG-04-S02) ────────────────────────────────────────
    engine::AudioSystem audio;
    if (audio.isReady()) {
        audio.loadClip("beep", "assets/audio/test_beep.wav");
        std::cout << "Audio:   ready  |  P: play/stop beep\n";
    } else {
        std::cout << "Audio:   unavailable (no device)\n";
    }
    bool audioPlaying = false;

    // ── Red triangle — NDC coordinates, OpenGL 4.5 DSA ───────────────────────
    static constexpr std::array<float, 6> vertices = {
        -0.5f, -0.5f,   // bottom-left
         0.5f, -0.5f,   // bottom-right
         0.0f,  0.5f,   // top-centre
    };

    GLuint vao = 0, vbo = 0;
    glCreateVertexArrays(1, &vao);
    glCreateBuffers(1, &vbo);

    // Immutable storage — vertices never change after upload.
    glNamedBufferStorage(
        vbo,
        static_cast<GLsizeiptr>(vertices.size() * sizeof(float)),
        vertices.data(),
        0  // no GL_DYNAMIC_STORAGE_BIT needed
    );

    // Bind VBO to vertex binding point 0 of the VAO; stride = 2 floats.
    glVertexArrayVertexBuffer(vao, 0, vbo, 0,
                              static_cast<GLsizei>(2 * sizeof(float)));

    // Attribute 0: 2 × float, not normalised, offset 0 within the stride.
    glEnableVertexArrayAttrib(vao, 0);
    glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao, 0, 0);

    // ── Shader program ────────────────────────────────────────────────────────
    const GLuint prog = buildProgram();
    if (!prog) return 1;

    // ── Render loop ───────────────────────────────────────────────────────────
    while (window.isOpen()) {
        // P key: toggle beep play/stop
        if (glfwGetKey(window.handle(), GLFW_KEY_P) == GLFW_PRESS && audio.isReady()) {
            if (audioPlaying) { audio.stop("beep"); audioPlaying = false; }
            else              { audio.play("beep"); audioPlaying = true;  }
        }

        glClearColor(0.05f, 0.05f, 0.10f, 1.0f);  // dark-navy background
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(prog);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        window.swapAndPoll();
    }

    // ── Cleanup ───────────────────────────────────────────────────────────────
    glDeleteProgram(prog);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    // window destructor: glfwDestroyWindow + glfwTerminate

    return 0;
}
