#pragma once
#include "IState.h"

class DebugState : public IState {
private:
    int selectedOption;
    int brickLife;
    float ballSpeedX, ballSpeedY, paddleSpeed;
    int lives, brickRows;
public:
    explicit DebugState(Game* game);
    void OnEnter() override;
    void Update(float dt) override;
    void Draw() override;
    void OnExit() override;
};