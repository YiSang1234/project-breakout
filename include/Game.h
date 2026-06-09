#pragma once
#include "raylib.h"
#include "StateMachine.h"
#include <memory>

// 前向声明
class GameCore;
class LevelManager;
class PowerUpManager;
class ParticleManager;
class SaveManager;
class EditorManager;
class AsyncLoadManager;

class Game {
private:
    int screenWidth, screenHeight;
    Texture2D bgTextures[3];
    int currentBgIndex;
    Texture2D logoTexture;
    Music bgMusic;
    Sound hitSound, powerUpSound, gameOverSound;

    std::unique_ptr<GameCore> core;
    std::unique_ptr<LevelManager> levelMgr;
    std::unique_ptr<PowerUpManager> powerUpMgr;
    std::unique_ptr<ParticleManager> particleMgr;
    std::unique_ptr<SaveManager> saveMgr;
    std::unique_ptr<EditorManager> editorMgr;
    std::unique_ptr<AsyncLoadManager> asyncLoadMgr;
    std::unique_ptr<StateMachine> stateMachine;

    float menuBallSpeed, menuPaddleSpeed;
    int menuLives, menuBrickRows;
    bool twoPlayerMode;

public:
    Game();
    ~Game();
    void Run();

    // 核心逻辑委托
    void UpdateGamePlay(float dt);
    void DrawGamePlay();
    bool IsGameOver();
    bool IsGameWin();
    void ResetGame();
    void NextLevel();
    void GotoLevel(int level);
    void SetMenuConfig(float ballSpeed, float paddleSpeed, int lives, int brickRows);

    // 道具效果
    void SetPaddleLonger(bool enable);
    void SetSlowBall(bool enable);
    void SpawnMultiBalls();

    // 双人模式
    void SetTwoPlayerMode(bool enable);
    bool IsTwoPlayerMode() const { return twoPlayerMode; }

    // 存档与编辑器
    void OpenArchiveSaveMode();
    void OpenArchiveLoadMode();
    void StartEditMode();

    // 异步加载
    void StartBallAsyncLoad();
    void StartBrickAsyncLoad();
    void UpdateAsyncLoad();
    bool IsAnyAsyncLoading() const;
    bool IsBallAsyncComplete() const;
    bool IsBrickAsyncComplete() const;
    void ResetBallAsync();
    void ResetBrickAsync();
    void ApplyRandomBallColors();
    void ApplyRandomBrickColors();

    // 删档重置
    void ResetAllProgress();

    // Getter
    int GetScreenWidth() const { return screenWidth; }
    int GetScreenHeight() const { return screenHeight; }
    int GetScore() const;
    int GetHighScore() const;
    int GetCurrentLevel() const;
    int GetUnlockedLevel() const;
    int GetLevelHighScore(int level) const;

    StateMachine* GetStateMachine() { return stateMachine.get(); }
    GameCore* GetCore() { return core.get(); }
    LevelManager* GetLevelManager() { return levelMgr.get(); }
    PowerUpManager* GetPowerUpManager() { return powerUpMgr.get(); }
    ParticleManager* GetParticleManager() { return particleMgr.get(); }
    SaveManager* GetSaveManager() { return saveMgr.get(); }
    EditorManager* GetEditorManager() { return editorMgr.get(); }

    void PlayHitSound() { PlaySound(hitSound); }
    void PlayPowerUpSound() { PlaySound(powerUpSound); }
    void PlayGameOverSound() { PlaySound(gameOverSound); }
};