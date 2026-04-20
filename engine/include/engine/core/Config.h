#pragma once

#include <nlohmann/json.hpp>
#include <string>

namespace engine {

/// Singleton JSON config system.
///
/// Loads a JSON file from disk, provides typed get/set with dot-notation
/// key traversal, and can write the current state back to the same file.
///
/// Usage:
///   Config::instance().load("config/game.json");
///   int w = Config::instance().get<int>("graphics.width", 1920);
///   Config::instance().set<int>("graphics.width", 2560);
///   Config::instance().save();
///
/// Key notation: dot-separated path segments, e.g. "graphics.width".
/// Missing path segments always return the supplied default — never throws.
class Config {
public:
    /// Meyer's singleton — thread-safe initialisation in C++11+.
    static Config& instance();

    // Non-copyable, non-movable.
    Config(const Config&)            = delete;
    Config& operator=(const Config&) = delete;
    Config(Config&&)                 = delete;
    Config& operator=(Config&&)      = delete;

    /// Load JSON from file.
    /// On parse error or missing file: m_loaded is set to false, internal
    /// state is reset to empty, and no exception is propagated.
    void load(const std::string& path);

    /// Write current state back to the path supplied to load().
    /// No-op if load() was never successfully called.
    void save() const;

    /// True if the last load() call succeeded.
    [[nodiscard]] bool isLoaded() const { return m_loaded; }

    // ── Typed accessors ───────────────────────────────────────────────────────

    /// Return the value at dot-notation key, or defaultVal if not found.
    /// Never throws — a corrupt/missing key silently returns the default.
    template <typename T>
    T get(const std::string& key, const T& defaultVal) const {
        try {
            const nlohmann::json* node = traverse(key);
            if (node == nullptr) {
                return defaultVal;
            }
            return node->get<T>();
        } catch (...) {
            return defaultVal;
        }
    }

    /// Set value at dot-notation key, creating intermediate objects as needed.
    /// Does NOT auto-save — caller must call save() explicitly.
    template <typename T>
    void set(const std::string& key, const T& value) {
        nlohmann::json* node = traverseOrCreate(key);
        if (node != nullptr) {
            *node = value;
        }
    }

private:
    Config() = default;

    /// Walk the JSON tree following dot-split segments of key.
    /// Returns pointer to the target node, or nullptr if any segment is missing.
    [[nodiscard]] const nlohmann::json* traverse(const std::string& key) const;

    /// Walk the JSON tree following dot-split segments of key, creating
    /// intermediate objects as needed. Returns pointer to the target node.
    nlohmann::json* traverseOrCreate(const std::string& key);

    nlohmann::json m_data;
    std::string    m_path;
    bool           m_loaded = false;
};

} // namespace engine
