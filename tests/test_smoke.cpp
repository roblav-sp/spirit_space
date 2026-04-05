#include <gtest/gtest.h>
#include <filesystem>
#include <string>
#include <cstdint>

// ── EP-01-S01 Smoke Tests ─────────────────────────────────────────────────────
// Acceptance: build artifact exists and test executable runs.
// These tests verify the build system, C++17 features, and
// GTest integration are all working correctly.

TEST(Smoke, BuildSucceeds)
{
    // If this test runs, the build succeeded.
    SUCCEED();
}

TEST(Smoke, Cpp17FeaturesAvailable)
{
    // Structured bindings (C++17)
    auto [a, b] = std::pair<int, int>{1, 2};
    EXPECT_EQ(a, 1);
    EXPECT_EQ(b, 2);

    // std::string_view (C++17)
    std::string_view sv = "Spirit Space";
    EXPECT_EQ(sv.size(), 12u);

    // if constexpr (C++17)
    constexpr bool is_64bit = sizeof(void*) == 8;
    if constexpr (is_64bit) {
        EXPECT_EQ(sizeof(void*), 8u);
    }
}

TEST(Smoke, DoublePrecisionAvailable)
{
    // Verify double precision (required for interstellar coordinates).
    // A float cannot represent distances > ~16M AU without loss.
    // A double can represent distances to ~100 LY with sub-AU precision.
    double au_per_ly = 63241.077;
    double distance_100ly = 100.0 * au_per_ly;
    EXPECT_NEAR(distance_100ly, 6324107.7, 0.1);
}

TEST(Smoke, StdFilesystemAvailable)
{
    // std::filesystem is a key C++17 feature used throughout the project.
    namespace fs = std::filesystem;
    fs::path config_path = "config/game.json";
    EXPECT_EQ(config_path.filename().string(), "game.json");
    EXPECT_EQ(config_path.parent_path().string(), "config");
}
