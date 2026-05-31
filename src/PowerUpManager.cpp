#include "PowerUpManager.h"
#include "GameCore.h"
#include <cstdlib>
#include <algorithm>

PowerUpManager::PowerUpManager(GameCore* c) : paddleLongerTime(0), slowBallTime(0), core(c) {}

void PowerUpManager::Update(float dt, const Rectangle& paddleRect) {
    for (auto& pu : powerUps) {
        if (!pu.IsActive()) continue;
        pu.Update();
        if (pu.GetPosition().y > 800) pu.Deactivate();
        if (pu.CheckCollision(paddleRect)) {
            ApplyPowerUp(pu.GetType());
            pu.Deactivate();
        }
    }
    powerUps.erase(std::remove_if(powerUps.begin(), powerUps.end(),
        [](const PowerUp& pu) { return !pu.IsActive(); }), powerUps.end());
}

void PowerUpManager::Draw() const {
    for (const auto& pu : powerUps) pu.Draw();
}

void PowerUpManager::SpawnPowerUp(Vector2 pos) {
    if (rand() % 100 < 30) {
        PowerUpType type = (PowerUpType)(rand() % 3);
        powerUps.emplace_back(pos, type);
    }
}

void PowerUpManager::ApplyPowerUp(PowerUpType type) {
    if (!core) return;
    switch (type) {
        case PADDLE_LONGER:
            paddleLongerTime = POWERUP_DURATION;
            core->SetPaddleLonger(true);
            break;
        case MULTI_BALL:
            core->SpawnMultiBalls();
            break;
        case SLOW_BALL:
            slowBallTime = POWERUP_DURATION;
            core->SetSlowBall(true);
            break;
    }
}

void PowerUpManager::ClearAllEffects() {
    paddleLongerTime = 0;
    slowBallTime = 0;
    core->SetPaddleLonger(false);
    core->SetSlowBall(false);
}

void PowerUpManager::UpdateTimers(float dt) {
    if (paddleLongerTime > 0) {
        paddleLongerTime -= dt;
        if (paddleLongerTime <= 0) core->SetPaddleLonger(false);
    }
    if (slowBallTime > 0) {
        slowBallTime -= dt;
        if (slowBallTime <= 0) core->SetSlowBall(false);
    }
}