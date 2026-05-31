#pragma once
#include "IState.h"

class GameOverState : public IState {
public:
    explicit GameOverState(Game* game);
    void OnEnter() override;
    void Update(float dt) override;
    void Draw() override;
    void OnExit() override;
};