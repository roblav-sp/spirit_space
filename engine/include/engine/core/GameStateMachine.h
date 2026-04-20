#pragma once

#include "engine/core/IGameState.h"

#include <cstddef>
#include <memory>
#include <vector>

namespace engine {

class GameStateMachine {
public:
    GameStateMachine()  = default;
    ~GameStateMachine() = default;

    GameStateMachine(const GameStateMachine&)            = delete;
    GameStateMachine& operator=(const GameStateMachine&) = delete;
    GameStateMachine(GameStateMachine&&)                 = delete;
    GameStateMachine& operator=(GameStateMachine&&)      = delete;

    // Push a new state on top: pauses current top (if any), then enters new state.
    void push(std::unique_ptr<IGameState> state);

    // Pop the top state: exits it, then resumes the new top (if any).
    // No-op (with warning) if the stack is empty.
    void pop();

    // Replace the top state: exits current top, enters new state.
    // No pause/resume involved. No-op pop guard applies if stack is empty.
    void replace(std::unique_ptr<IGameState> state);

    // Call onUpdate(dt) on the top state only. No-op if stack is empty.
    void update(double dt);

    bool        isEmpty() const;
    std::size_t size()    const;

    // Raw pointer to the top state, or nullptr if the stack is empty.
    IGameState* top() const;

private:
    std::vector<std::unique_ptr<IGameState>> m_stack;
};

} // namespace engine
