#pragma once
#include "Ball.h"
#include "Paddle.h"
#include "Brick.h"
#include <vector>

class PowerUpManager;
class ParticleManager;
class LevelManager;
class Game;

class GameCore {
private:
    Ball ball;
    Paddle paddle1;
    Paddle paddle2;
    std::vector<Brick> bricks;
    std::vector<Ball> multiBalls;
    int score, highScore, playerLife;
    int currentLevel, unlockedLevel;
    float gameTime;
    bool ballServed;
    bool showLevelText;
    float levelStartTimer;
    float ballSpeedX, ballSpeedY, paddleSpeed;
    int startLives, maxGameTime;
    int defaultBrickHitPoints;
    int screenWidth, screenHeight;
    bool twoPlayerMode;
    PowerUpManager* powerUpMgr;
    ParticleManager* particleMgr;
    LevelManager* levelMgr;
    Game* game;

    void HandleBallBrickCollision(Ball& ball);

public:
    GameCore(PowerUpManager* pum, ParticleManager* pm, LevelManager* lm, Game* g, bool twoPlayer = false);
    void SetScreenDimensions(int w, int h) { screenWidth = w; screenHeight = h; }

    void UpdateGamePlay(float dt);
    void DrawGamePlay() const;
    void ResetGame();
    void NextLevel();
    void GotoLevel(int level);
    bool IsGameOver() const;
    bool IsGameWin() const;

    void SetPaddleLonger(bool enable);
    void SetSlowBall(bool enable);
    void SpawnMultiBalls();

    // 访问器
    Ball& GetBall() { return ball; }
    const Ball& GetBall() const { return ball; }
    Paddle& GetPaddle1() { return paddle1; }
    const Paddle& GetPaddle1() const { return paddle1; }
    Paddle& GetPaddle2() { return paddle2; }
    const Paddle& GetPaddle2() const { return paddle2; }
    std::vector<Brick>& GetBricks() { return bricks; }
    const std::vector<Brick>& GetBricks() const { return bricks; }
    std::vector<Ball>& GetMultiBalls() { return multiBalls; }
    const std::vector<Ball>& GetMultiBalls() const { return multiBalls; }

    int GetScore() const { return score; }
    int GetHighScore() const { return highScore; }
    int GetPlayerLife() const { return playerLife; }
    int GetCurrentLevel() const { return currentLevel; }
    int GetUnlockedLevel() const { return unlockedLevel; }
    bool IsBallServed() const { return ballServed; }
    void SetBallServed(bool s) { ballServed = s; }
    void SetScore(int s) { score = s; }
    void AddScore(int add) { score += add; }
    void SetPlayerLife(int life) { playerLife = life; }
    void SetCurrentLevel(int lvl) { currentLevel = lvl; }
    void SetUnlockedLevel(int lvl) { unlockedLevel = lvl; }
    void SetGameTime(float t) { gameTime = t; }
    float GetGameTime() const { return gameTime; }
    void ClearMultiBalls() { multiBalls.clear(); }

    float GetBallSpeedX() const { return ballSpeedX; }
    float GetBallSpeedY() const { return ballSpeedY; }
    void SetBallSpeedX(float x) { ballSpeedX = x; }
    void SetBallSpeedY(float y) { ballSpeedY = y; }
    float GetPaddleSpeed() const { return paddleSpeed; }
    void SetPaddleSpeed(float s) { paddleSpeed = s; }
    int GetDefaultBrickHitPoints() const { return defaultBrickHitPoints; }
    void SetDefaultBrickHitPoints(int hp) { defaultBrickHitPoints = hp; }
    int GetStartLives() const { return startLives; }
    void SetStartLives(int lives) { startLives = lives; }
    void SetMaxGameTime(int t) { maxGameTime = t; }

    void SetTwoPlayerMode(bool enable) { twoPlayerMode = enable; }
    bool IsTwoPlayerMode() const { return twoPlayerMode; }

    void PlayHitSound() const;
};