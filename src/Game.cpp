#include "Game.h"
#include "GameCore.h"
#include "LevelManager.h"
#include "PowerUpManager.h"
#include "ParticleManager.h"
#include "SaveManager.h"
#include "EditorManager.h"
#include "AsyncLoadManager.h"
#include "MenuState.h"
#include "PlayingState.h"
#include "PausedState.h"
#include "GameOverState.h"
#include "LevelScoreState.h"
#include "GameWinState.h"
#include "EditorState.h"
#include "ArchiveSelectState.h"
#include "DebugState.h"
#include <cstdlib>
#include <ctime>

// 避免 Windows.h 冲突（在包含任何 Windows 头文件前定义）
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOUSER
#include <windows.h>
#endif

Game::Game() : screenWidth(800), screenHeight(600), currentBgIndex(0), twoPlayerMode(false) {
    InitWindow(screenWidth, screenHeight, "Breakout | Complete Edition");
    InitAudioDevice();
    SetTargetFPS(60);
    srand((unsigned)time(nullptr));

#ifdef _WIN32
    system("mkdir saves 2>nul");
    system("mkdir levels 2>nul");
#else
    system("mkdir -p saves");
    system("mkdir -p levels");
#endif

    // 加载资源（容错）
    bgTextures[0] = LoadTexture("assets/bg1.png");
    bgTextures[1] = LoadTexture("assets/bg2.png");
    bgTextures[2] = LoadTexture("assets/bg3.png");
    for (int i = 1; i < 3; ++i)
        if (bgTextures[i].id == 0) bgTextures[i] = bgTextures[0];
    logoTexture = LoadTexture("assets/logo.png");
    bgMusic = LoadMusicStream("assets/bg_music.wav");
    hitSound = LoadSound("assets/hit.wav");
    powerUpSound = LoadSound("assets/powerup.wav");
    gameOverSound = LoadSound("assets/gameover.wav");
    PlayMusicStream(bgMusic);
    SetMusicVolume(bgMusic, 0.4f);

    levelMgr = std::make_unique<LevelManager>();
    particleMgr = std::make_unique<ParticleManager>();
    powerUpMgr = std::make_unique<PowerUpManager>(nullptr);
    core = std::make_unique<GameCore>(powerUpMgr.get(), particleMgr.get(), levelMgr.get(), this, twoPlayerMode);
    powerUpMgr->SetCore(core.get());
    saveMgr = std::make_unique<SaveManager>();
    editorMgr = std::make_unique<EditorManager>();
    asyncLoadMgr = std::make_unique<AsyncLoadManager>();

    stateMachine = std::make_unique<StateMachine>(this);
    stateMachine->RegisterState(GameStateType::MENU,           new MenuState(this));
    stateMachine->RegisterState(GameStateType::PLAYING,        new PlayingState(this));
    stateMachine->RegisterState(GameStateType::PAUSED,         new PausedState(this));
    stateMachine->RegisterState(GameStateType::GAME_OVER,      new GameOverState(this));
    stateMachine->RegisterState(GameStateType::LEVEL_SCORE,    new LevelScoreState(this));
    stateMachine->RegisterState(GameStateType::GAME_WIN,       new GameWinState(this));
    stateMachine->RegisterState(GameStateType::EDITOR,         new EditorState(this));
    stateMachine->RegisterState(GameStateType::ARCHIVE_SELECT, new ArchiveSelectState(this));
    stateMachine->RegisterState(GameStateType::DEBUG,          new DebugState(this));

    stateMachine->SwitchTo(GameStateType::MENU);
}

Game::~Game() {
    for (auto& tex : bgTextures) UnloadTexture(tex);
    UnloadTexture(logoTexture);
    UnloadMusicStream(bgMusic);
    UnloadSound(hitSound);
    UnloadSound(powerUpSound);
    UnloadSound(gameOverSound);
    CloseAudioDevice();
    CloseWindow();
}

void Game::Run() {
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        UpdateMusicStream(bgMusic);
        asyncLoadMgr->Update();

        BeginDrawing();
        ClearBackground(BLACK);
        stateMachine->Update(dt);
        stateMachine->Draw();
        DrawFPS(10, 10);
        EndDrawing();
    }
}

// 委托方法
void Game::UpdateGamePlay(float dt) { core->UpdateGamePlay(dt); }
void Game::DrawGamePlay() { core->DrawGamePlay(); }
bool Game::IsGameOver() { return core->IsGameOver(); }
bool Game::IsGameWin() { return core->IsGameWin(); }
void Game::ResetGame() { core->ResetGame(); }
void Game::NextLevel() { core->NextLevel(); }
void Game::GotoLevel(int level) { core->GotoLevel(level); }

void Game::SetMenuConfig(float bs, float ps, int lives, int rows) {
    core->SetBallSpeedX(bs);
    core->SetBallSpeedY(-bs);
    core->SetPaddleSpeed(ps);
    core->SetStartLives(lives);
    levelMgr->SetDefaultBrickRows(rows);
}

void Game::SetPaddleLonger(bool enable) { core->SetPaddleLonger(enable); }
void Game::SetSlowBall(bool enable) { core->SetSlowBall(enable); }
void Game::SpawnMultiBalls() { core->SpawnMultiBalls(); }

int Game::GetScore() const { return core->GetScore(); }
int Game::GetHighScore() const { return core->GetHighScore(); }
int Game::GetCurrentLevel() const { return core->GetCurrentLevel(); }
int Game::GetUnlockedLevel() const { return core->GetUnlockedLevel(); }

void Game::OpenArchiveSaveMode() {
    stateMachine->SwitchTo(GameStateType::ARCHIVE_SELECT);
    if (auto* s = dynamic_cast<ArchiveSelectState*>(stateMachine->GetCurrentState()))
        s->SetMode(true);
}

void Game::OpenArchiveLoadMode() {
    stateMachine->SwitchTo(GameStateType::ARCHIVE_SELECT);
    if (auto* s = dynamic_cast<ArchiveSelectState*>(stateMachine->GetCurrentState()))
        s->SetMode(false);
}

void Game::StartEditMode() {
    int level = core->GetUnlockedLevel();
    editorMgr->SetEditingLevel(level);
    editorMgr->LoadLevelForEdit(level);
    stateMachine->SwitchTo(GameStateType::EDITOR);
}

void Game::SetTwoPlayerMode(bool enable) {
    twoPlayerMode = enable;
    core->SetTwoPlayerMode(enable);
}

// 异步加载小球
void Game::StartBallAsyncLoad() { asyncLoadMgr->StartBallAsyncLoad(); }
bool Game::IsBallAsyncComplete() const { return asyncLoadMgr->IsBallComplete(); }
void Game::ResetBallAsync() { asyncLoadMgr->ResetBall(); }
void Game::ApplyRandomBallColors() {
    auto randColor = []() { return Color{(unsigned char)(rand()%256), (unsigned char)(rand()%256), (unsigned char)(rand()%256), 255}; };
    core->GetBall().SetColor(randColor());
    for (auto& mb : core->GetMultiBalls()) mb.SetColor(randColor());
}

// 异步加载砖块
void Game::StartBrickAsyncLoad() { asyncLoadMgr->StartBrickAsyncLoad(); }
bool Game::IsBrickAsyncComplete() const { return asyncLoadMgr->IsBrickComplete(); }
void Game::ResetBrickAsync() { asyncLoadMgr->ResetBrick(); }
void Game::ApplyRandomBrickColors() {
    auto randColor = []() { return Color{(unsigned char)(rand()%256), (unsigned char)(rand()%256), (unsigned char)(rand()%256), 255}; };
    for (auto& brick : core->GetBricks()) if (brick.IsActive()) brick.SetColor(randColor());
}

bool Game::IsAnyAsyncLoading() const {
    return asyncLoadMgr->IsBallLoading() || asyncLoadMgr->IsBrickLoading();
}

void Game::UpdateAsyncLoad() { asyncLoadMgr->Update(); }

int Game::GetLevelHighScore(int level) const {
    return 0; // 暂未实现
}