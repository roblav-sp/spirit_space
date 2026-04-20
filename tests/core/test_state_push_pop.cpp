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

TEST(PushPop, PushOne) {
    engine::GameStateMachine gsm;
    gsm.push(std::make_unique<MockState>());
    EXPECT_EQ(gsm.size(), std::size_t{1});
    EXPECT_NE(gsm.top(), nullptr);
}

TEST(PushPop, PopOne) {
    engine::GameStateMachine gsm;
    gsm.push(std::make_unique<MockState>());
    gsm.pop();
    EXPECT_EQ(gsm.size(), std::size_t{0});
    EXPECT_TRUE(gsm.isEmpty());
}

TEST(PushPop, PushTwo) {
    engine::GameStateMachine gsm;
    gsm.push(std::make_unique<MockState>());
    gsm.push(std::make_unique<MockState>());
    EXPECT_EQ(gsm.size(), std::size_t{2});
}

TEST(PushPop, PopToFirst) {
    engine::GameStateMachine gsm;
    auto* first = new MockState();
    gsm.push(std::unique_ptr<MockState>(first));
    gsm.push(std::make_unique<MockState>());
    gsm.pop();
    EXPECT_EQ(gsm.top(), first);
}

TEST(PushPop, PopEmptyIsSafe) {
    engine::GameStateMachine gsm;
    EXPECT_NO_FATAL_FAILURE(gsm.pop());
}
