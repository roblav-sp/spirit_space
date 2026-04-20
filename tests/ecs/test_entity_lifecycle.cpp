/// tests/ecs/test_entity_lifecycle.cpp
/// Tests entity creation and destruction through engine::World.
/// Component convention: all component types are prefixed with C (e.g. CPosition).

#include <gtest/gtest.h>
#include "engine/ecs/World.h"

// ---------------------------------------------------------------------------
// EntityLifecycle.CreateAndDestroy
// Create 10 000 entities then destroy them all; entityCount must return 0.
// ---------------------------------------------------------------------------
TEST(EntityLifecycle, CreateAndDestroy) {
    engine::World world;

    constexpr int kEntityCount = 10'000;

    std::vector<entt::entity> entities;
    entities.reserve(kEntityCount);

    for (int i = 0; i < kEntityCount; ++i) {
        entities.push_back(world.createEntity());
    }

    EXPECT_EQ(world.entityCount(), static_cast<std::size_t>(kEntityCount));

    for (auto e : entities) {
        world.destroyEntity(e);
    }

    EXPECT_EQ(world.entityCount(), 0u);
}

// ---------------------------------------------------------------------------
// EntityLifecycle.ClearResetsCount
// clear() should destroy all entities and bring entityCount to 0.
// ---------------------------------------------------------------------------
TEST(EntityLifecycle, ClearResetsCount) {
    engine::World world;

    for (int i = 0; i < 100; ++i) {
        world.createEntity();
    }

    EXPECT_EQ(world.entityCount(), 100u);
    world.clear();
    EXPECT_EQ(world.entityCount(), 0u);
}

// ---------------------------------------------------------------------------
// EntityLifecycle.DestroyNonexistentIsSafe
// NOTE: EnTT asserts (debug) or has undefined behaviour (release) when you
// destroy an entity that was never valid or has already been destroyed.
// This sprint does NOT test that path; callers are responsible for only
// destroying live entities.  The behaviour is documented here so future
// engineers are not surprised.
// ---------------------------------------------------------------------------
TEST(EntityLifecycle, DocumentedBehaviourDestroyOnlyLiveEntities) {
    // Intentionally empty — see comment above.
    SUCCEED() << "Destroying non-existent entities is undefined in EnTT; "
                 "callers must only destroy entities returned by createEntity().";
}
