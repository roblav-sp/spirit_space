// IMPORTANT: glad/glad.h MUST be included before GLFW/glfw3.h.
// glad defines platform macros (APIENTRY, etc.) that glfw3.h uses
// conditionally. Reversing the order causes macro redefinition errors
// under MSVC and undefined behaviour on other toolchains.
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "engine/hud/HudSystem.h"

namespace engine {

// ── Constructor ───────────────────────────────────────────────────────────────

HudSystem::HudSystem(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, /*install_callbacks=*/true);
    ImGui_ImplOpenGL3_Init("#version 450");

    m_ready = true;
}

// ── Destructor ────────────────────────────────────────────────────────────────

HudSystem::~HudSystem()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

// ── Panel registry ────────────────────────────────────────────────────────────

void HudSystem::registerPanel(const std::string& name,
                               std::function<void()> renderFn)
{
    m_panels[name] = std::move(renderFn);
}

void HudSystem::unregisterPanel(const std::string& name)
{
    m_panels.erase(name);
}

bool HudSystem::hasPanel(const std::string& name) const
{
    return m_panels.count(name) != 0;
}

std::size_t HudSystem::panelCount() const
{
    return m_panels.size();
}

// ── Frame interface ───────────────────────────────────────────────────────────

void HudSystem::beginFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Fullscreen dockspace — covers the entire viewport.
    // Pass 0 as the dockspace ID to let ImGui auto-assign one, and pass the
    // main viewport explicitly so the dockspace covers the full window area.
    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
}

void HudSystem::renderPanels()
{
    for (auto& [name, fn] : m_panels) {
        if (fn) {
            fn();
        }
    }
}

void HudSystem::endFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// ── State queries ─────────────────────────────────────────────────────────────

bool HudSystem::isReady() const
{
    return m_ready;
}

} // namespace engine
