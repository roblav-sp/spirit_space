#include "engine/core/GameStateMachine.h"
#include "engine/core/IGameState.h"

#include <gtest/gtest.h>

TEST(EmptyStack, UpdateNoCrash) {
    engine::GameStateMachine gsm;
    EXPECT_NO_FATAL_FAILURE(gsm.update(0.016));
}

TEST(EmptyStack, TopReturnsNullptr) {
    engine::GameStateMachine gsm;
    EXPECT_EQ(gsm.top(), nullptr);
}

TEST(EmptyStack, IsEmptyTrue) {
    engine::GameStateMachine gsm;
    EXPECT_TRUE(gsm.isEmpty());
}

TEST(EmptyStack, SizeZero) {
    engine::GameStateMachine gsm;
    EXPECT_EQ(gsm.size(), std::size_t{0});
}

TEST(EmptyStack, PopNoCrash) {
    engine::GameStateMachine gsm;
    EXPECT_NO_FATAL_FAILURE(gsm.pop());
}
