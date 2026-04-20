#include "engine/core/GameStateMachine.h"

#include <iostream>

namespace engine {

void GameStateMachine::push(std::unique_ptr<IGameState> state) {
    if (!m_stack.empty()) {
        m_stack.back()->onPause();
    }
    m_stack.push_back(std::move(state));
    m_stack.back()->onEnter();
}

void GameStateMachine::pop() {
    if (m_stack.empty()) {
        std::cerr << "[GameStateMachine] pop() called on empty stack — ignored\n";
        return;
    }
    m_stack.back()->onExit();
    m_stack.pop_back();
    if (!m_stack.empty()) {
        m_stack.back()->onResume();
    }
}

void GameStateMachine::replace(std::unique_ptr<IGameState> state) {
    if (!m_stack.empty()) {
        m_stack.back()->onExit();
        m_stack.pop_back();
    }
    m_stack.push_back(std::move(state));
    m_stack.back()->onEnter();
}

void GameStateMachine::update(double dt) {
    if (m_stack.empty()) {
        return;
    }
    m_stack.back()->onUpdate(dt);
}

bool GameStateMachine::isEmpty() const {
    return m_stack.empty();
}

std::size_t GameStateMachine::size() const {
    return m_stack.size();
}

IGameState* GameStateMachine::top() const {
    if (m_stack.empty()) {
        return nullptr;
    }
    return m_stack.back().get();
}

} // namespace engine
