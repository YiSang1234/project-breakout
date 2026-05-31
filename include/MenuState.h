#pragma once
#include "IState.h"

class MenuState : public IState {
private:
    int selectedOption;  // 0:开始游戏, 1:关卡选择, 2:加载存档, 3:编辑器, 4:调试模式
public:
    explicit MenuState(Game* game);
    void OnEnter() override;
    void Update(float dt) override;
    void Draw() override;
    void OnExit() override;
};