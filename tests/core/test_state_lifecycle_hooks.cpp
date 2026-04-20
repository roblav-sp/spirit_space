#include "engine/core/GameStateMachine.h"
#include "engine/core/IGameState.h"

#include <gtest/gtest.h>
#include <memory>

namespace {

struct MockState : engine::IGameState {
    int enterCount  = 0;
    int exitCount   = 0;
    int updateCount = 0;
    int pauseCount  = 0;
    int resumeCount = 0;

    void onEnter()           override { ++enterCount; }
    void onExit()            override { ++exitCount; }
    void onUpdate(double)    override { ++updateCount; }
    void onPause()           override { ++pauseCount; }
    void onResume()          override { ++resumeCount; }
};

} // namespace

TEST(Lifecycle, EnterOnPush) {
    engine::GameStateMachine gsm;
    auto* s = new MockState();
    gsm.push(std::unique_ptr<MockState>(s));
    EXPECT_EQ(s->enterCount, 1);
}

TEST(Lifecycle, ExitOnPop) {
    engine::GameStateMachine gsm;
    auto* s = new MockState();
    gsm.push(std::unique_ptr<MockState>(s));
    gsm.pop();
    EXPECT_EQ(s->exitCount, 1);
}

TEST(Lifecycle, EnterExitExactlyOnce) {
    engine::GameStateMachine gsm;
    auto* s = new MockState();
    gsm.push(std::unique_ptr<MockState>(s));
    gsm.update(0.016);
    gsm.update(0.016);
    gsm.update(0.016);
    gsm.pop();
    EXPECT_EQ(s->enterCount,  1);
    EXPECT_EQ(s->exitCount,   1);
    EXPECT_EQ(s->updateCount, 3);
}

TEST(Lifecycle, PauseOnSecondPush) {
    engine::GameStateMachine gsm;
    auto* a = new MockState();
    gsm.push(std::unique_ptr<MockState>(a));
    gsm.push(std::make_unique<MockState>());
    EXPECT_EQ(a->pauseCount, 1);
}

TEST(Lifecycle, ResumeOnPop) {
    engine::GameStateMachine gsm;
    auto* a = new MockState();
    gsm.push(std::unique_ptr<MockState>(a));
    gsm.push(std::make_unique<MockState>());
    gsm.pop();
    EXPECT_EQ(a->resumeCount, 1);
}

TEST(Lifecycle, UpdateOnlyTop) {
    engine::GameStateMachine gsm;
    auto* a = new MockState();
    auto* b = new MockState();
    gsm.push(std::unique_ptr<MockState>(a));
    gsm.push(std::unique_ptr<MockState>(b));
    gsm.update(0.016);
    EXPECT_EQ(b->updateCount, 1);
    EXPECT_EQ(a->updateCount, 0);
}

TEST(Lifecycle, ReplaceExitEnter) {
    engine::GameStateMachine gsm;
    auto* a = new MockState();
    gsm.push(std::unique_ptr<MockState>(a));
    auto* b = new MockState();
    gsm.replace(std::unique_ptr<MockState>(b));
    EXPECT_EQ(a->exitCount,  1);
    EXPECT_EQ(b->enterCount, 1);
    EXPECT_EQ(a->pauseCount, 0);
}
