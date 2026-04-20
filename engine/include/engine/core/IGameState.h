#pragma once

namespace engine {

class IGameState {
public:
    virtual ~IGameState() = default;

    virtual void onEnter() = 0;
    virtual void onExit()  = 0;
    virtual void onUpdate(double dt) = 0;

    virtual void onPause()  {}
    virtual void onResume() {}
};

} // namespace engine
