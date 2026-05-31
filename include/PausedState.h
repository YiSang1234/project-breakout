#pragma once
#include "IState.h"

class PausedState : public IState {
public:
    explicit PausedState(Game* game);
    void OnEnter() override;
    void Update(float dt) override;
    void Draw() override;
    void OnExit() override;
};