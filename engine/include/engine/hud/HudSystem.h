#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include <cstddef>

// Forward-declare GLFWwindow so consumers of this header do not need
// to include <GLFW/glfw3.h> themselves.
struct GLFWwindow;

namespace engine {

/// Manages the Dear ImGui context (docking branch) and a registry of named
/// render panels.
///
/// Lifecycle:
///   - Constructor: initialises ImGui context with docking enabled.
///   - Call beginFrame() at the start of each frame, renderPanels() to invoke
///     all registered callbacks, then endFrame() to flush draw data to the GPU.
///   - Destructor: tears down ImGui backends and destroys the context.
///
/// Non-copyable, non-movable — owns the global ImGui context.
class HudSystem {
public:
    /// Initialises the ImGui context and OpenGL3 + GLFW backends.
    /// @param window  Active GLFW window with a current OpenGL context.
    explicit HudSystem(GLFWwindow* window);

    /// Shuts down ImGui backends and destroys the context.
    ~HudSystem();

    // Non-copyable, non-movable.
    HudSystem(const HudSystem&)            = delete;
    HudSystem& operator=(const HudSystem&) = delete;
    HudSystem(HudSystem&&)                 = delete;
    HudSystem& operator=(HudSystem&&)      = delete;

    // ── Panel registry ────────────────────────────────────────────────────────

    /// Register (or overwrite) a named panel.
    /// @param name     Unique panel identifier.
    /// @param renderFn Callback invoked between beginFrame/endFrame to render
    ///                 one ImGui window.  May be empty to clear a slot.
    void registerPanel(const std::string& name, std::function<void()> renderFn);

    /// Remove a panel from the registry.  No-op if the name is not found.
    void unregisterPanel(const std::string& name);

    /// Returns true if a panel with the given name is registered.
    [[nodiscard]] bool hasPanel(const std::string& name) const;

    /// Returns the number of currently registered panels.
    [[nodiscard]] std::size_t panelCount() const;

    // ── Frame interface ───────────────────────────────────────────────────────

    /// Begin a new ImGui frame and submit a fullscreen dockspace.
    /// Must be called before renderPanels() and endFrame().
    void beginFrame();

    /// Invoke every registered panel callback.
    /// Call between beginFrame() and endFrame().
    void renderPanels();

    /// Finalise the ImGui frame and flush draw data to the GPU.
    void endFrame();

    // ── State queries ─────────────────────────────────────────────────────────

    /// True after the constructor completed successfully.
    [[nodiscard]] bool isReady() const;

private:
    std::unordered_map<std::string, std::function<void()>> m_panels;
    bool m_ready = false;
};

} // namespace engine
