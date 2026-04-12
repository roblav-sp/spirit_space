// IMPORTANT: glad/glad.h MUST be included before GLFW/glfw3.h.
// glad defines platform macros (APIENTRY, etc.) that glfw3.h uses
// conditionally. Reversing the order causes macro redefinition errors
// under MSVC and undefined behaviour on other toolchains.
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine/core/Window.h"

#include <iostream>
#include <stdexcept>

namespace engine {

// ── Static callbacks ──────────────────────────────────────────────────────────

void Window::errorCallback(int error, const char* description)
{
    std::cerr << "[GLFW] Error " << error << ": " << description << '\n';
}

void Window::keyCallback(GLFWwindow* win, int key, int /*scancode*/,
                         int action, int /*mods*/)
{
    if (action != GLFW_PRESS) return;

    auto* self = static_cast<Window*>(glfwGetWindowUserPointer(win));
    if (!self) return;

    switch (key) {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(win, GLFW_TRUE);
            break;
        case GLFW_KEY_F11:
            self->toggleFullscreen();
            break;
        default:
            break;
    }
}

// ── Constructor ───────────────────────────────────────────────────────────────

Window::Window(const Config& cfg)
    : m_fullscreen(cfg.fullscreen)
    , m_contextValid(false)
    , m_cfg(cfg)
{
    glfwSetErrorCallback(errorCallback);

    if (!glfwInit()) {
        // glfwInit failure — cannot recover; this is always a fatal error.
        throw std::runtime_error("glfwInit() failed");
    }

    // Request an OpenGL 4.5 core profile context.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE,        GLFW_OPENGL_CORE_PROFILE);
    // FORWARD_COMPAT is required on macOS; on Windows it is a no-op / false.
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_FALSE);
    glfwWindowHint(GLFW_VISIBLE,
                   cfg.visible ? GLFW_TRUE : GLFW_FALSE);

    GLFWmonitor* monitor = cfg.fullscreen ? glfwGetPrimaryMonitor() : nullptr;

    m_window = glfwCreateWindow(
        cfg.width, cfg.height,
        cfg.title.c_str(),
        monitor,
        nullptr  // no shared context
    );

    if (!m_window) {
        // Window or context creation failed — not necessarily fatal for tests;
        // the caller checks isContextValid() before proceeding.
        // Do NOT call glfwTerminate here — the destructor owns that call.
        return;
    }

    glfwSetWindowUserPointer(m_window, this);
    glfwSetKeyCallback(m_window, keyCallback);
    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);  // vsync — 0 to disable

    // Load all OpenGL 4.5 function pointers via GLAD.
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cerr << "[GLAD] Failed to load OpenGL function pointers.\n";
        glfwDestroyWindow(m_window);
        m_window = nullptr;
        // Do NOT call glfwTerminate here — the destructor owns that call.
        return;
    }

    // Verify the driver provided at least OpenGL 4.5.
    // GLVersion is a global struct populated by gladLoadGLLoader.
    if (GLVersion.major < 4 || (GLVersion.major == 4 && GLVersion.minor < 5)) {
        std::cerr << "[GL] OpenGL 4.5 required; driver reports "
                  << GLVersion.major << '.' << GLVersion.minor << ".\n";
        glfwDestroyWindow(m_window);
        m_window = nullptr;
        // Do NOT call glfwTerminate here — the destructor owns that call.
        return;
    }

    m_contextValid = true;
}

// ── Destructor ────────────────────────────────────────────────────────────────

Window::~Window()
{
    if (m_window) {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
    // glfwTerminate is called exactly once per successful glfwInit.
    // The constructor only throws if glfwInit itself fails (before the library
    // is initialised), so if the destructor runs, glfwInit succeeded and
    // glfwTerminate must be called here — regardless of whether a window or
    // context was created.
    glfwTerminate();
}

// ── Frame-loop ────────────────────────────────────────────────────────────────

bool Window::isOpen() const
{
    return m_window && !glfwWindowShouldClose(m_window);
}

void Window::swapAndPoll()
{
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

// ── Fullscreen toggle ─────────────────────────────────────────────────────────

void Window::toggleFullscreen()
{
    m_fullscreen = !m_fullscreen;

    if (m_fullscreen) {
        GLFWmonitor*       monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode    = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(m_window, monitor,
                             0, 0,
                             mode->width, mode->height,
                             mode->refreshRate);
    } else {
        // Restore to the dimensions from the original Config.
        glfwSetWindowMonitor(m_window, nullptr,
                             100, 100,
                             m_cfg.width, m_cfg.height,
                             GLFW_DONT_CARE);
    }
}

} // namespace engine
