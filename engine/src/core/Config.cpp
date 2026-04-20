#include "engine/core/Config.h"

#include <fstream>
#include <sstream>
#include <vector>

namespace engine {

// ── Helpers ───────────────────────────────────────────────────────────────────

namespace {

/// Split a dot-notation key into its path segments.
std::vector<std::string> splitKey(const std::string& key) {
    std::vector<std::string> parts;
    std::stringstream ss(key);
    std::string segment;
    while (std::getline(ss, segment, '.')) {
        if (!segment.empty()) {
            parts.push_back(segment);
        }
    }
    return parts;
}

} // namespace

// ── Singleton ─────────────────────────────────────────────────────────────────

Config& Config::instance() {
    static Config s_instance;
    return s_instance;
}

// ── load / save ───────────────────────────────────────────────────────────────

void Config::load(const std::string& path) {
    m_loaded = false;
    m_path   = path;
    m_data   = nlohmann::json{};

    try {
        std::ifstream file(path);
        if (!file.is_open()) {
            return;
        }
        m_data   = nlohmann::json::parse(file, /*cb=*/nullptr,
                                         /*allow_exceptions=*/true,
                                         /*ignore_comments=*/true);
        m_loaded = true;
    } catch (...) {
        // Corrupt or invalid JSON — reset to empty, leave m_loaded = false.
        m_data = nlohmann::json{};
    }
}

void Config::save() const {
    if (m_path.empty()) {
        return;
    }
    std::ofstream file(m_path);
    if (!file.is_open()) {
        return;
    }
    file << m_data.dump(4);
}

// ── Key traversal ─────────────────────────────────────────────────────────────

const nlohmann::json* Config::traverse(const std::string& key) const {
    const auto parts = splitKey(key);
    if (parts.empty()) {
        return nullptr;
    }

    const nlohmann::json* node = &m_data;
    for (const auto& part : parts) {
        if (!node->is_object() || !node->contains(part)) {
            return nullptr;
        }
        node = &(*node)[part];
    }
    return node;
}

nlohmann::json* Config::traverseOrCreate(const std::string& key) {
    const auto parts = splitKey(key);
    if (parts.empty()) {
        return nullptr;
    }

    nlohmann::json* node = &m_data;
    for (const auto& part : parts) {
        // Ensure the current node is an object so we can descend into it.
        if (!node->is_object()) {
            *node = nlohmann::json::object();
        }
        node = &(*node)[part];
    }
    return node;
}

} // namespace engine
