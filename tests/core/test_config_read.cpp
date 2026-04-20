#include "engine/core/Config.h"

#include <gtest/gtest.h>
#include <fstream>
#include <string>

// Each test uses a unique temp file path to avoid singleton state conflicts.

namespace {

void writeTempJson(const std::string& path, const std::string& content) {
    std::ofstream f(path);
    f << content;
}

} // namespace

// ── ConfigRead ────────────────────────────────────────────────────────────────

TEST(ConfigRead, ReadString) {
    const std::string path = "/tmp/config_read_string.json";
    writeTempJson(path, R"({ "name": "Spirit Space" })");

    engine::Config& cfg = engine::Config::instance();
    cfg.load(path);

    EXPECT_TRUE(cfg.isLoaded());
    EXPECT_EQ(cfg.get<std::string>("name", ""), "Spirit Space");
}

TEST(ConfigRead, ReadInt) {
    const std::string path = "/tmp/config_read_int.json";
    writeTempJson(path, R"({ "count": 42 })");

    engine::Config& cfg = engine::Config::instance();
    cfg.load(path);

    EXPECT_TRUE(cfg.isLoaded());
    EXPECT_EQ(cfg.get<int>("count", 0), 42);
}

TEST(ConfigRead, ReadDouble) {
    const std::string path = "/tmp/config_read_double.json";
    writeTempJson(path, R"({ "ratio": 3.14 })");

    engine::Config& cfg = engine::Config::instance();
    cfg.load(path);

    EXPECT_TRUE(cfg.isLoaded());
    EXPECT_DOUBLE_EQ(cfg.get<double>("ratio", 0.0), 3.14);
}

TEST(ConfigRead, ReadNested) {
    const std::string path = "/tmp/config_read_nested.json";
    writeTempJson(path, R"({ "graphics": { "width": 1920 } })");

    engine::Config& cfg = engine::Config::instance();
    cfg.load(path);

    EXPECT_TRUE(cfg.isLoaded());
    EXPECT_EQ(cfg.get<int>("graphics.width", 0), 1920);
}
