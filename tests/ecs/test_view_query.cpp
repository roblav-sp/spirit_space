/// tests/ecs/test_view_query.cpp
/// Tests EnTT view queries through engine::World.
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

struct CVelocity {
    double vx{0.0};
    double vy{0.0};
    double vz{0.0};
};

// ---------------------------------------------------------------------------
// ViewQuery.ViewSingleComponent
// Create 100 entities each with CPosition; the view must iterate exactly 100.
// ---------------------------------------------------------------------------
TEST(ViewQuery, ViewSingleComponent) {
    engine::World world;

    constexpr int kCount = 100;
    for (int i = 0; i < kCount; ++i) {
        auto e = world.createEntity();
        world.addComponent<CPosition>(e, CPosition{static_cast<double>(i), 0.0, 0.0});
    }

    int count = 0;
    for ([[maybe_unused]] auto entity : world.view<CPosition>()) {
        ++count;
    }

    EXPECT_EQ(count, kCount);
}

// ---------------------------------------------------------------------------
// ViewQuery.ViewMultipleComponents
// 50 entities get CPosition + CVelocity; another 50 get only CPosition.
// view<CPosition, CVelocity> must return exactly 50.
// ---------------------------------------------------------------------------
TEST(ViewQuery, ViewMultipleComponents) {
    engine::World world;

    constexpr int kBothCount = 50;
    constexpr int kPositionOnly = 50;

    for (int i = 0; i < kBothCount; ++i) {
        auto e = world.createEntity();
        world.addComponent<CPosition>(e, CPosition{static_cast<double>(i), 0.0, 0.0});
        world.addComponent<CVelocity>(e, CVelocity{1.0, 0.0, 0.0});
    }

    for (int i = 0; i < kPositionOnly; ++i) {
        auto e = world.createEntity();
        world.addComponent<CPosition>(e, CPosition{static_cast<double>(i), 1.0, 0.0});
        // No CVelocity
    }

    int count = 0;
    for ([[maybe_unused]] auto entity : world.view<CPosition, CVelocity>()) {
        ++count;
    }

    EXPECT_EQ(count, kBothCount);
}

// ---------------------------------------------------------------------------
// ViewQuery.ViewEmpty
// No entities exist; iterating a view must not crash and must yield zero items.
// ---------------------------------------------------------------------------
TEST(ViewQuery, ViewEmpty) {
    engine::World world;

    int count = 0;
    for ([[maybe_unused]] auto entity : world.view<CPosition>()) {
        ++count;
    }

    EXPECT_EQ(count, 0);
}

// ---------------------------------------------------------------------------
// ViewQuery.ViewAfterClear
// After world.clear(), a view must return no entities.
// ---------------------------------------------------------------------------
TEST(ViewQuery, ViewAfterClear) {
    engine::World world;

    for (int i = 0; i < 20; ++i) {
        auto e = world.createEntity();
        world.addComponent<CPosition>(e, CPosition{});
    }

    world.clear();

    int count = 0;
    for ([[maybe_unused]] auto entity : world.view<CPosition>()) {
        ++count;
    }

    EXPECT_EQ(count, 0);
}
