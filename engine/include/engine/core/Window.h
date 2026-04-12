#pragma once

#include <string>

// Forward-declare GLFWwindow so consumers of this header do not need
// to include <GLFW/glfw3.h> themselves.
struct GLFWwindow;

namespace engine {

/// Manages a GLFW window and its OpenGL 4.5 core context.
///
/// Lifecycle:
///   - Constructor initialises GLFW, creates the window, loads GL via GLAD.
///   - On success: isContextValid() == true, isOpen() == true.
///   - On failure (no GL 4.5 driver): isContextValid() == false; caller must
///     check before calling any GL or rendering functions.
///   - Destructor always destroys the window and terminates GLFW.
///
/// Built-in key bindings (registered automatically):
///   ESC  → marks window should-close
///   F11  → toggleFullscreen()
class Window {
public:
    struct Config {
        int         width      = 1920;
        int         height     = 1080;
        std::string title      = "Spirit Space";
        bool        fullscreen = false;
        /// Set false to create a hidden (off-screen) window — used for tests
        /// running in CI environments without a display or GPU driver.
        bool        visible    = true;
    };

    explicit Window(const Config& cfg);
    ~Window();

    // Non-copyable, non-movable — owns a GLFW window handle.
    Window(const Window&)            = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&&)                 = delete;
    Window& operator=(Window&&)      = delete;

    // ── Frame-loop interface ──────────────────────────────────────────────────

    /// Returns true while the window has not been asked to close.
    [[nodiscard]] bool isOpen() const;

    /// Swap front/back buffers and process pending OS events.
    /// Call once per frame, at the end of the render block.
    void swapAndPoll();

    // ── State queries ─────────────────────────────────────────────────────────

    /// True if an OpenGL 4.5 core context was successfully created and GLAD
    /// loaded all function pointers. Always check this before rendering.
    [[nodiscard]] bool isContextValid() const { return m_contextValid; }

    /// Raw GLFW window handle — use when calling GL functions directly (e.g.
    /// setting additional callbacks from the application layer).
    [[nodiscard]] GLFWwindow* handle() const { return m_window; }

    // ── Window control ────────────────────────────────────────────────────────

    /// Toggle between windowed and borderless-fullscreen on the primary monitor.
    /// Windowed dimensions are restored from the Config passed at construction.
    void toggleFullscreen();

private:
    static void errorCallback(int error, const char* description);
    static void keyCallback(GLFWwindow* win, int key, int scancode,
                            int action, int mods);

    GLFWwindow* m_window       = nullptr;
    bool        m_fullscreen   = false;
    bool        m_contextValid = false;
    Config      m_cfg;  // saved for windowed restore after fullscreen
};

} // namespace engine
