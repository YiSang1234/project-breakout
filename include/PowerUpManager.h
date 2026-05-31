#pragma once
#include "PowerUp.h"
#include <vector>

class GameCore;

class PowerUpManager {
private:
    std::vector<PowerUp> powerUps;
    float paddleLongerTime;
    float slowBallTime;
    const float POWERUP_DURATION = 8.0f;
    GameCore* core;

public:
    explicit PowerUpManager(GameCore* c = nullptr);
    void SetCore(GameCore* c) { core = c; }
    void Update(float dt, const Rectangle& paddleRect);
    void Draw() const;
    void SpawnPowerUp(Vector2 pos);
    void ApplyPowerUp(PowerUpType type);
    void ClearAllEffects();
    void UpdateTimers(float dt);
    bool IsPaddleLonger() const { return paddleLongerTime > 0; }
    bool IsSlowBall() const { return slowBallTime > 0; }
    float GetPaddleLongerTime() const { return paddleLongerTime; }
    float GetSlowBallTime() const { return slowBallTime; }
};