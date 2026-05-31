#pragma once
#include "IState.h"

class LevelScoreState : public IState {
private:
    int previewLevel;
public:
    explicit LevelScoreState(Game* game);
    void OnEnter() override;
    void Update(float dt) override;
    void Draw() override;
    void OnExit() override;
};