#pragma once
#include "raylib.h"

class Game;

// 状态基类
class IState {
protected:
    Game* game;
public:
    explicit IState(Game* g) : game(g) {}
    virtual ~IState() = default;
    virtual void OnEnter() = 0;
    virtual void Update(float dt) = 0;
    virtual void Draw() = 0;
    virtual void OnExit() = 0;
};