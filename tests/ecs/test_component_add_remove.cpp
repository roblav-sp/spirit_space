/// tests/ecs/test_component_add_remove.cpp
/// Tests component add/remove operations through engine::World.
/// Component convention: all component types are prefixed with C.

#include <gtest/gtest.h>
#include "engine/ecs/World.h"

// ---------------------------------------------------------------------------
// Test components — C-prefix convention enforced.
// Spatial coordinates use double precision as required by the project.
// ---------------------------------------------------------------------------

struct CPosition {
    double x{0.0};
    double y{0.0};
    double z{0.0};
};

struct CHealth {
    float current{0.0f};
    float max{0.0f};
};

struct CVelocity {
    double vx{0.0};
    double vy{0.0};
    double vz{0.0};
};

// ---------------------------------------------------------------------------
// ComponentAddRemove.AddAndGet
// Add CPosition to an entity and verify the stored values are correct.
// ---------------------------------------------------------------------------
TEST(ComponentAddRemove, AddAndGet) {
    engine::World world;
    auto entity = world.createEntity();

    auto& pos = world.addComponent<CPosition>(entity, CPosition{1.0, 2.0, 3.0});

    EXPECT_DOUBLE_EQ(pos.x, 1.0);
    EXPECT_DOUBLE_EQ(pos.y, 2.0);
    EXPECT_DOUBLE_EQ(pos.z, 3.0);

    // getComponent should return the same data
    auto& posRef = world.getComponent<CPosition>(entity);
    EXPECT_DOUBLE_EQ(posRef.x, 1.0);
    EXPECT_DOUBLE_EQ(posRef.y, 2.0);
    EXPECT_DOUBLE_EQ(posRef.z, 3.0);
}

// ---------------------------------------------------------------------------
// ComponentAddRemove.HasComponent
// Add CHealth, check has returns true; remove, check has returns false.
// ---------------------------------------------------------------------------
TEST(ComponentAddRemove, HasComponent) {
    engine::World world;
    auto entity = world.createEntity();

    EXPECT_FALSE(world.hasComponent<CHealth>(entity));

    world.addComponent<CHealth>(entity, CHealth{80.0f, 100.0f});
    EXPECT_TRUE(world.hasComponent<CHealth>(entity));

    world.removeComponent<CHealth>(entity);
    EXPECT_FALSE(world.hasComponent<CHealth>(entity));
}

// ---------------------------------------------------------------------------
// ComponentAddRemove.MultipleComponents
// Add CPosition, CHealth, and CVelocity to one entity; verify each is
// accessible and holds correct values.
// ---------------------------------------------------------------------------
TEST(ComponentAddRemove, MultipleComponents) {
    engine::World world;
    auto entity = world.createEntity();

    world.addComponent<CPosition>(entity, CPosition{10.0, 20.0, 30.0});
    world.addComponent<CHealth>(entity, CHealth{50.0f, 100.0f});
    world.addComponent<CVelocity>(entity, CVelocity{0.1, 0.2, 0.3});

    EXPECT_TRUE(world.hasComponent<CPosition>(entity));
    EXPECT_TRUE(world.hasComponent<CHealth>(entity));
    EXPECT_TRUE(world.hasComponent<CVelocity>(entity));

    auto& pos = world.getComponent<CPosition>(entity);
    EXPECT_DOUBLE_EQ(pos.x, 10.0);
    EXPECT_DOUBLE_EQ(pos.y, 20.0);
    EXPECT_DOUBLE_EQ(pos.z, 30.0);

    auto& hp = world.getComponent<CHealth>(entity);
    EXPECT_FLOAT_EQ(hp.current, 50.0f);
    EXPECT_FLOAT_EQ(hp.max, 100.0f);

    auto& vel = world.getComponent<CVelocity>(entity);
    EXPECT_DOUBLE_EQ(vel.vx, 0.1);
    EXPECT_DOUBLE_EQ(vel.vy, 0.2);
    EXPECT_DOUBLE_EQ(vel.vz, 0.3);
}

// ---------------------------------------------------------------------------
// ComponentAddRemove.AddComponentReplacesExisting
// addComponent on an already-present component updates the value (emplace_or_replace).
// ---------------------------------------------------------------------------
TEST(ComponentAddRemove, AddComponentReplacesExisting) {
    engine::World world;
    auto entity = world.createEntity();

    world.addComponent<CPosition>(entity, CPosition{1.0, 2.0, 3.0});
    world.addComponent<CPosition>(entity, CPosition{9.0, 8.0, 7.0});

    auto& pos = world.getComponent<CPosition>(entity);
    EXPECT_DOUBLE_EQ(pos.x, 9.0);
    EXPECT_DOUBLE_EQ(pos.y, 8.0);
    EXPECT_DOUBLE_EQ(pos.z, 7.0);
}
