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

// ── ConfigWrite ───────────────────────────────────────────────────────────────

TEST(ConfigWrite, SetAndGet) {
    const std::string path = "/tmp/config_write_setget.json";
    writeTempJson(path, R"({ "score": 0 })");

    engine::Config& cfg = engine::Config::instance();
    cfg.load(path);

    cfg.set<int>("score", 999);
    EXPECT_EQ(cfg.get<int>("score", 0), 999);
}

TEST(ConfigWrite, WriteAndReload) {
    const std::string path = "/tmp/config_write_persist.json";
    writeTempJson(path, R"({ "volume": 0.5 })");

    // Set a new value and persist it.
    {
        engine::Config& cfg = engine::Config::instance();
        cfg.load(path);
        cfg.set<double>("volume", 0.75);
        cfg.save();
    }

    // Reload from the same file and verify the written value survives.
    {
        engine::Config& cfg = engine::Config::instance();
        cfg.load(path);
        EXPECT_TRUE(cfg.isLoaded());
        EXPECT_DOUBLE_EQ(cfg.get<double>("volume", 0.0), 0.75);
    }
}

TEST(ConfigWrite, SetNestedAndPersist) {
    const std::string path = "/tmp/config_write_nested_persist.json";
    writeTempJson(path, R"({ "graphics": { "width": 1280 } })");

    {
        engine::Config& cfg = engine::Config::instance();
        cfg.load(path);
        cfg.set<int>("graphics.width", 2560);
        cfg.save();
    }

    {
        engine::Config& cfg = engine::Config::instance();
        cfg.load(path);
        EXPECT_EQ(cfg.get<int>("graphics.width", 0), 2560);
    }
}
