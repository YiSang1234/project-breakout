#pragma once
#include "IState.h"

class GameWinState : public IState {
public:
    explicit GameWinState(Game* game);
    void OnEnter() override;
    void Update(float dt) override;
    void Draw() override;
    void OnExit() override;
};