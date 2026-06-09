 #pragma once
#include "IState.h"

class MenuState : public IState {
private:
    int selectedOption;
    bool waitingConfirm;
public:
    explicit MenuState(Game* game);
    void OnEnter() override;
    void Update(float dt) override;
    void Draw() override;
    void OnExit() override;
};