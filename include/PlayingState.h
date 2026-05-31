#pragma once
#include "IState.h"

class PlayingState : public IState {
public:
    explicit PlayingState(Game* game);
    void OnEnter() override;
    void Update(float dt) override;
    void Draw() override;
    void OnExit() override;
};