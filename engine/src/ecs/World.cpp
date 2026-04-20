#include "engine/ecs/World.h"

namespace engine {

entt::entity World::createEntity() {
    return m_registry.create();
}

void World::destroyEntity(entt::entity entity) {
    m_registry.destroy(entity);
}

std::size_t World::entityCount() const {
    // EnTT 3.x removed registry::alive(). The entity storage's free_list()
    // tracks the number of live (non-recycled) entity slots, which equals the
    // alive entity count.
    // The const overload of storage<T>() returns a pointer; dereference it.
    return m_registry.storage<entt::entity>()->free_list();
}

void World::clear() {
    m_registry.clear();
}

} // namespace engine
