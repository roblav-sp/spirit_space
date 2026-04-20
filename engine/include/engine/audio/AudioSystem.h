#pragma once

#include <string>
#include <unordered_map>
#include <memory>

// Forward-declare the miniaudio engine struct so consumers of this header
// do not need to include miniaudio.h (which defines MA_NO_* macros globally).
struct ma_engine;
struct ma_sound;

namespace engine {

/// Generic audio backend built on miniaudio.
/// Loads named clips, plays/stops them, and controls master volume.
/// Positional audio setters are stubbed — implementation deferred to Phase 2.
///
/// Usage:
///   AudioSystem audio;
///   audio.loadClip("beep", "assets/audio/beep.wav");
///   audio.play("beep");
///   audio.setVolume(0.5f);
///   audio.stop("beep");
class AudioSystem {
public:
    AudioSystem();
    ~AudioSystem();

    AudioSystem(const AudioSystem&)            = delete;
    AudioSystem& operator=(const AudioSystem&) = delete;
    AudioSystem(AudioSystem&&)                 = delete;
    AudioSystem& operator=(AudioSystem&&)      = delete;

    /// Returns true if the miniaudio engine initialised successfully.
    [[nodiscard]] bool isReady() const { return m_ready; }

    /// Load an audio file and register it under 'id'.
    /// Returns false and does nothing if the file does not exist.
    bool loadClip(const std::string& id, const std::string& path);

    /// Start playback of a previously loaded clip.
    /// No-op if id is unknown or the system is not ready.
    void play(const std::string& id);

    /// Stop playback of a previously loaded clip.
    /// No-op if id is unknown or the clip is not playing.
    void stop(const std::string& id);

    /// Master volume: 0.0 (silent) – 1.0 (full). Clamped automatically.
    void setVolume(float volume);

    [[nodiscard]] float volume() const { return m_volume; }

    // ── Positional audio stubs (Phase 2) ─────────────────────────────────────
    void setListenerPosition(double x, double y, double z);
    void setSoundPosition(const std::string& id, double x, double y, double z);

private:
    struct SoundEntry {
        std::unique_ptr<ma_sound> sound;
        bool loaded = false;
    };

    std::unique_ptr<ma_engine>                         m_engine;
    std::unordered_map<std::string, SoundEntry>        m_clips;
    float                                              m_volume = 1.0f;
    bool                                               m_ready  = false;
};

} // namespace engine
