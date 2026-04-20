// miniaudio implementation — defined exactly once in the project.
// All other translation units include miniaudio.h WITHOUT this define.
#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

#include "engine/audio/AudioSystem.h"

#include <algorithm>
#include <iostream>

namespace engine {

AudioSystem::AudioSystem()
    : m_engine(std::make_unique<ma_engine>())
{
    ma_result result = ma_engine_init(nullptr, m_engine.get());
    if (result != MA_SUCCESS) {
        std::cerr << "[Audio] ma_engine_init failed: " << result << '\n';
        m_engine.reset();
        return;
    }
    m_ready = true;
}

AudioSystem::~AudioSystem()
{
    for (auto& [id, entry] : m_clips) {
        if (entry.loaded) {
            ma_sound_uninit(entry.sound.get());
        }
    }
    if (m_ready) {
        ma_engine_uninit(m_engine.get());
    }
}

bool AudioSystem::loadClip(const std::string& id, const std::string& path)
{
    if (!m_ready) return false;

    auto sound = std::make_unique<ma_sound>();
    ma_result result = ma_sound_init_from_file(
        m_engine.get(),
        path.c_str(),
        MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC,
        nullptr, nullptr,
        sound.get()
    );

    if (result != MA_SUCCESS) {
        std::cerr << "[Audio] Could not load '" << path << "': " << result << '\n';
        return false;
    }

    // If a clip with this id already exists, uninit the old one first.
    auto it = m_clips.find(id);
    if (it != m_clips.end() && it->second.loaded) {
        ma_sound_uninit(it->second.sound.get());
    }

    m_clips[id] = { std::move(sound), true };
    return true;
}

void AudioSystem::play(const std::string& id)
{
    if (!m_ready) return;
    auto it = m_clips.find(id);
    if (it == m_clips.end() || !it->second.loaded) return;

    // Rewind so repeated play() calls always start from the beginning.
    ma_sound_seek_to_pcm_frame(it->second.sound.get(), 0);
    ma_sound_start(it->second.sound.get());
}

void AudioSystem::stop(const std::string& id)
{
    if (!m_ready) return;
    auto it = m_clips.find(id);
    if (it == m_clips.end() || !it->second.loaded) return;
    ma_sound_stop(it->second.sound.get());
}

void AudioSystem::setVolume(float volume)
{
    m_volume = std::clamp(volume, 0.0f, 1.0f);
    if (m_ready) {
        ma_engine_set_volume(m_engine.get(), m_volume);
    }
}

// ── Positional stubs ──────────────────────────────────────────────────────────

void AudioSystem::setListenerPosition(double /*x*/, double /*y*/, double /*z*/)
{
    // Phase 2: ma_engine_listener_set_position()
}

void AudioSystem::setSoundPosition(const std::string& /*id*/,
                                   double /*x*/, double /*y*/, double /*z*/)
{
    // Phase 2: ma_sound_set_position()
}

} // namespace engine
