#include "engine/core/Config.h"

#include <gtest/gtest.h>
#include <fstream>
#include <string>

namespace {

void writeTempJson(const std::string& path, const std::string& content) {
    std::ofstream f(path);
    f << content;
}

} // namespace

// ── ConfigMissing ─────────────────────────────────────────────────────────────

TEST(ConfigMissing, MissingKeyReturnsDefault) {
    const std::string path = "/tmp/config_missing_flat.json";
    writeTempJson(path, R"({ "present": 1 })");

    engine::Config& cfg = engine::Config::instance();
    cfg.load(path);

    EXPECT_TRUE(cfg.isLoaded());
    EXPECT_EQ(cfg.get<int>("nonexistent", 42), 42);
}

TEST(ConfigMissing, NestedMissingReturnsDefault) {
    const std::string path = "/tmp/config_missing_nested.json";
    writeTempJson(path, R"({ "a": {} })");

    engine::Config& cfg = engine::Config::instance();
    cfg.load(path);

    EXPECT_TRUE(cfg.isLoaded());
    EXPECT_FLOAT_EQ(cfg.get<float>("a.b.c", 1.0f), 1.0f);
}

TEST(ConfigMissing, CorruptFileNoCrash) {
    const std::string path = "/tmp/config_corrupt.json";
    writeTempJson(path, R"({ this is not valid json !! )");

    engine::Config& cfg = engine::Config::instance();
    cfg.load(path);

    EXPECT_FALSE(cfg.isLoaded());
    // Verify get still works gracefully and returns the default.
    EXPECT_EQ(cfg.get<int>("any.key", 99), 99);
}
