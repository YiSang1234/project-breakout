#include "GameCore.h"
#include "PowerUpManager.h"
#include "ParticleManager.h"
#include "LevelManager.h"
#include "Game.h"
#include <cmath>
#include <algorithm>

GameCore::GameCore(PowerUpManager* pum, ParticleManager* pm, LevelManager* lm, Game* g, bool twoPlayer)
    : ball({400,530}, {0,0}, 10.0f),
      paddle1(350, 550, 100, 20, 8.0f, SKYBLUE),
      paddle2(350, 530, 100, 20, 8.0f, GREEN),
      twoPlayerMode(twoPlayer),
      score(0), highScore(0), playerLife(3),
      currentLevel(1), unlockedLevel(1),
      gameTime(0), ballServed(false),
      showLevelText(true), levelStartTimer(1.5f),
      ballSpeedX(4.0f), ballSpeedY(-4.0f), paddleSpeed(8.0f),
      startLives(3), maxGameTime(120), defaultBrickHitPoints(1),
      screenWidth(800), screenHeight(600),
      powerUpMgr(pum), particleMgr(pm), levelMgr(lm), game(g)
{
}

void GameCore::UpdateGamePlay(float dt) {
    if (showLevelText) {
        levelStartTimer -= dt;
        if (levelStartTimer <= 0) showLevelText = false;
        return;
    }

    gameTime += dt;
    // 移动挡板
    if (!twoPlayerMode) {
        // 单人模式：只用左右键控制 paddle1
        if (IsKeyDown(KEY_LEFT)) paddle1.MoveLeft(screenWidth);
        if (IsKeyDown(KEY_RIGHT)) paddle1.MoveRight(screenWidth);
    } else {
        // 双人模式：paddle1 左右键，paddle2 A/D 键
        if (IsKeyDown(KEY_LEFT)) paddle1.MoveLeft(screenWidth);
        if (IsKeyDown(KEY_RIGHT)) paddle1.MoveRight(screenWidth);
        if (IsKeyDown(KEY_A)) paddle2.MoveLeft(screenWidth);
        if (IsKeyDown(KEY_D)) paddle2.MoveRight(screenWidth);
    }

    powerUpMgr->Update(dt, paddle1.GetRect());
    if (twoPlayerMode) powerUpMgr->Update(dt, paddle2.GetRect());
    powerUpMgr->UpdateTimers(dt);
    particleMgr->Update(dt);

    if (!ballServed) {
        float bx = (twoPlayerMode ? paddle2.GetRect().x : paddle1.GetRect().x) + paddle1.GetRect().width / 2;
        float by = (twoPlayerMode ? paddle2.GetRect().y : paddle1.GetRect().y) - 10;
        ball.Reset({bx, by}, {0,0});
        if (IsKeyPressed(KEY_SPACE)) {
            ball.Reset({bx, by}, {ballSpeedX, ballSpeedY});
            ballServed = true;
        }
    } else {
        float mul = powerUpMgr->IsSlowBall() ? 0.7f : 1.0f;
        ball.Move(mul);
        ball.BounceEdge(screenWidth, screenHeight);
        // 碰撞检测（双人模式两个挡板）
        bool hitPaddle = ball.CheckPaddleCollision(paddle1.GetRect());
        if (!hitPaddle && twoPlayerMode) {
            hitPaddle = ball.CheckPaddleCollision(paddle2.GetRect());
        }
        if (hitPaddle) {
            particleMgr->SpawnParticles(ball.GetPosition(), SKYBLUE, 12, PARTICLE_SPARK);
            particleMgr->SpawnDirectionalParticle(ball.GetPosition(), {0,-1}, PARTICLE_SPEED_LINE);
            PlayHitSound();
        }
        HandleBallBrickCollision(ball);
        if (ball.CheckBottomDeath(screenHeight)) {
            playerLife--;
            ballServed = false;
            multiBalls.clear();
            powerUpMgr->ClearAllEffects();
        }
    }

    for (auto& mb : multiBalls) {
        float mul = powerUpMgr->IsSlowBall() ? 0.7f : 1.0f;
        mb.Move(mul);
        mb.BounceEdge(screenWidth, screenHeight);
        bool hitPaddle = mb.CheckPaddleCollision(paddle1.GetRect());
        if (!hitPaddle && twoPlayerMode) hitPaddle = mb.CheckPaddleCollision(paddle2.GetRect());
        if (hitPaddle) {
            particleMgr->SpawnParticles(mb.GetPosition(), SKYBLUE, 6, PARTICLE_SPARK);
        }
        HandleBallBrickCollision(mb);
    }
    multiBalls.erase(std::remove_if(multiBalls.begin(), multiBalls.end(),
        [this](const Ball& b) { return b.GetPosition().y - 10 > screenHeight; }),
        multiBalls.end());

    int activeBricks = 0;
    for (auto& b : bricks) if (b.IsActive()) activeBricks++;
    if (activeBricks == 0) NextLevel();
}

void GameCore::HandleBallBrickCollision(Ball& ball) {
    for (auto& brick : bricks) {
        if (!brick.IsActive()) continue;
        if (ball.CheckBrickCollision(brick.GetRect())) {
            brick.DecrementHitPoints();
            Vector2 center = { brick.GetRect().x + brick.GetRect().width/2,
                               brick.GetRect().y + brick.GetRect().height/2 };
            if (!brick.IsActive()) {
                particleMgr->SpawnParticles(center, brick.GetColor(), 15, PARTICLE_DEFAULT);
                particleMgr->SpawnParticles(center, DARKGRAY, 8, PARTICLE_SMOKE);
                for (int i = 0; i < 12; ++i) {
                    float angle = (rand() % 360) * DEG2RAD;
                    Vector2 dir = { cosf(angle), sinf(angle) };
                    particleMgr->SpawnDirectionalParticle(center, dir, PARTICLE_SPEED_LINE);
                }
                score += brick.GetScoreValue();
                powerUpMgr->SpawnPowerUp(center);
            } else {
                particleMgr->SpawnParticles(center, brick.GetColor(), 5, PARTICLE_SPARK);
            }
            break;
        }
    }
}

void GameCore::DrawGamePlay() const {
    ball.DrawWithTrail();
    paddle1.Draw();
    if (twoPlayerMode) paddle2.Draw();
    for (auto& b : bricks) b.Draw();
    for (auto& mb : multiBalls) mb.DrawAsClone();
    powerUpMgr->Draw();
    particleMgr->Draw();

    DrawText(TextFormat("❤ %d", playerLife), 10, 8, 20, RED);
    DrawText(TextFormat("SCORE: %d", score), screenWidth/2 - 60, 8, 20, YELLOW);
    DrawText(TextFormat("BEST: %d", highScore), screenWidth - 130, 8, 20, GREEN);
    DrawText(TextFormat("LV: %d", currentLevel), screenWidth - 50, 8, 18, SKYBLUE);

    if (powerUpMgr->IsPaddleLonger())
        DrawText(TextFormat("LONGER: %.0fs", powerUpMgr->GetPaddleLongerTime()), 10, 50, 18, ORANGE);
    if (powerUpMgr->IsSlowBall())
        DrawText(TextFormat("SLOW: %.0fs", powerUpMgr->GetSlowBallTime()), 10, 70, 18, SKYBLUE);

    if (!ballServed && !showLevelText)
        DrawText("PRESS SPACE", screenWidth/2 - 70, screenHeight - 30, 20, LIGHTGRAY);
    if (showLevelText) {
        const char* txt = TextFormat("LEVEL %d", currentLevel);
        DrawText(txt, screenWidth/2 - MeasureText(txt, 40)/2, screenHeight/2 - 20, 40, YELLOW);
    }
    DrawText("P:PAUSE  L:MENU  K:LOAD  J:BRICKS", 20, screenHeight - 25, 16, LIGHTGRAY);
}

void GameCore::ResetGame() {
    playerLife = startLives;
    score = 0;
    gameTime = 0;
    ballServed = false;
    showLevelText = true;
    levelStartTimer = 1.5f;
    multiBalls.clear();
    powerUpMgr->ClearAllEffects();
    paddle1.SetScale(1.0f);
    paddle2.SetScale(1.0f);
    levelMgr->LoadLevel(currentLevel);
    bricks = levelMgr->GenerateBricks(screenWidth);
    float bx = (twoPlayerMode ? paddle2.GetRect().x : paddle1.GetRect().x) + paddle1.GetRect().width/2;
    float by = (twoPlayerMode ? paddle2.GetRect().y : paddle1.GetRect().y) - 10;
    ball.Reset({bx, by}, {0,0});
}

void GameCore::NextLevel() {
    currentLevel++;
    if (currentLevel > 5) {  // 最大5关
        return;
    }
    if (!levelMgr->LoadLevel(currentLevel)) {
        levelMgr->LoadLevel(currentLevel); // 生成默认
    }
    bricks = levelMgr->GenerateBricks(screenWidth);
    playerLife++;
    ballServed = false;
    showLevelText = true;
    levelStartTimer = 1.5f;
    multiBalls.clear();
    powerUpMgr->ClearAllEffects();
    paddle1.SetScale(1.0f);
    paddle2.SetScale(1.0f);
}

void GameCore::GotoLevel(int level) {
    currentLevel = level;
    ResetGame();
}

bool GameCore::IsGameOver() const {
    return playerLife <= 0 || gameTime >= maxGameTime;
}

bool GameCore::IsGameWin() const {
    return currentLevel > 5;
}

void GameCore::SetPaddleLonger(bool enable) {
    paddle1.SetScale(enable ? 1.6f : 1.0f);
    if (twoPlayerMode) paddle2.SetScale(enable ? 1.6f : 1.0f);
}

void GameCore::SetSlowBall(bool enable) { }

void GameCore::SpawnMultiBalls() {
    Vector2 p = ball.GetPosition();
    Color randColor = { (unsigned char)(rand()%256), (unsigned char)(rand()%256), (unsigned char)(rand()%256), 255 };
    multiBalls.emplace_back(p, Vector2{-3, -5}, 10.0f, randColor);
    multiBalls.emplace_back(p, Vector2{3, -5}, 10.0f, randColor);
}

void GameCore::PlayHitSound() const {
    if (game) game->PlayHitSound();
}

