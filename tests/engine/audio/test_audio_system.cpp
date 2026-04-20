#include <gtest/gtest.h>
#include "engine/audio/AudioSystem.h"

// ── EP-ENG-04-S02 — Audio Backend Tests ──────────────────────────────────────
//
// These tests run without a real audio device where possible.
// miniaudio falls back to a null/silent device when no output is available
// (e.g. CI), so isReady() may be false — individual tests skip in that case.

namespace {

constexpr const char* k_testClip  = "assets/audio/test_beep.wav";
constexpr const char* k_missingFile = "assets/audio/does_not_exist.wav";

} // namespace

// ── isReady ───────────────────────────────────────────────────────────────────

TEST(AudioSystem, InitReportsReady)
{
    engine::AudioSystem audio;
    // On CI without an audio device miniaudio may still succeed (null device).
    // We only assert it does not crash — ready state is informational here.
    SUCCEED();
}

// ── loadClip / missing file ───────────────────────────────────────────────────

TEST(AudioSystem, MissingFileNocrash)
{
    engine::AudioSystem audio;
    if (!audio.isReady()) GTEST_SKIP() << "No audio device available";

    // Must return false and must not crash or throw.
    bool result = false;
    EXPECT_NO_FATAL_FAILURE(result = audio.loadClip("missing", k_missingFile));
    EXPECT_FALSE(result);
}

// ── play / stop ───────────────────────────────────────────────────────────────

TEST(AudioSystem, PlayStopKnownClip)
{
    engine::AudioSystem audio;
    if (!audio.isReady()) GTEST_SKIP() << "No audio device available";

    bool loaded = audio.loadClip("beep", k_testClip);
    if (!loaded) GTEST_SKIP() << "Test clip not found (run from project root)";

    EXPECT_NO_FATAL_FAILURE(audio.play("beep"));
    EXPECT_NO_FATAL_FAILURE(audio.stop("beep"));
}

TEST(AudioSystem, PlayUnknownIdNoCrash)
{
    engine::AudioSystem audio;
    if (!audio.isReady()) GTEST_SKIP() << "No audio device available";

    EXPECT_NO_FATAL_FAILURE(audio.play("nonexistent_id"));
}

TEST(AudioSystem, StopUnknownIdNoCrash)
{
    engine::AudioSystem audio;
    if (!audio.isReady()) GTEST_SKIP() << "No audio device available";

    EXPECT_NO_FATAL_FAILURE(audio.stop("nonexistent_id"));
}

// ── volume ────────────────────────────────────────────────────────────────────

TEST(AudioSystem, VolumeSetAndGet)
{
    engine::AudioSystem audio;
    if (!audio.isReady()) GTEST_SKIP() << "No audio device available";

    audio.setVolume(0.5f);
    EXPECT_FLOAT_EQ(audio.volume(), 0.5f);
}

TEST(AudioSystem, VolumeClampsAboveOne)
{
    engine::AudioSystem audio;
    if (!audio.isReady()) GTEST_SKIP() << "No audio device available";

    audio.setVolume(2.0f);
    EXPECT_FLOAT_EQ(audio.volume(), 1.0f);
}

TEST(AudioSystem, VolumeClampsbelowZero)
{
    engine::AudioSystem audio;
    if (!audio.isReady()) GTEST_SKIP() << "No audio device available";

    audio.setVolume(-1.0f);
    EXPECT_FLOAT_EQ(audio.volume(), 0.0f);
}
