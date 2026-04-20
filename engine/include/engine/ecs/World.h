#pragma once

#include <entt/entt.hpp>
#include <cstddef>

namespace engine {

/// Thin wrapper around entt::registry providing the engine's entity/component API.
/// All component types must follow the C-prefix convention (e.g. CPosition, CHealth).
/// No game-domain types (CStar, CShip, etc.) may appear in this header.
class World {
public:
    World() = default;
    ~World() = default;

    // Non-copyable, movable
    World(const World&) = delete;
    World& operator=(const World&) = delete;
    World(World&&) = default;
    World& operator=(World&&) = default;

    /// Create a new entity and return its handle.
    entt::entity createEntity();

    /// Destroy an entity and all its components.
    void destroyEntity(entt::entity entity);

    /// Add (or replace) a component on an entity.
    template<typename T, typename... Args>
    T& addComponent(entt::entity entity, Args&&... args) {
        return m_registry.emplace_or_replace<T>(entity, std::forward<Args>(args)...);
    }

    /// Remove a component from an entity. No-op if the component is not present.
    template<typename T>
    void removeComponent(entt::entity entity) {
        m_registry.remove<T>(entity);
    }

    /// Get a reference to an existing component. Undefined behaviour if not present.
    template<typename T>
    T& getComponent(entt::entity entity) {
        return m_registry.get<T>(entity);
    }

    /// Get a const reference to an existing component.
    template<typename T>
    const T& getComponent(entt::entity entity) const {
        return m_registry.get<T>(entity);
    }

    /// Returns true if the entity has the given component.
    template<typename T>
    bool hasComponent(entt::entity entity) const {
        return m_registry.all_of<T>(entity);
    }

    /// Return a view over all entities that have all listed components.
    template<typename... Components>
    auto view() {
        return m_registry.view<Components...>();
    }

    /// Return a const view over all entities that have all listed components.
    template<typename... Components>
    auto view() const {
        return m_registry.view<Components...>();
    }

    /// Number of currently alive entities.
    std::size_t entityCount() const;

    /// Destroy all entities and components.
    void clear();

private:
    entt::registry m_registry;
};

} // namespace engine
