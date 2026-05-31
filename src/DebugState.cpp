#include "DebugState.h"
#include "Game.h"
#include "StateMachine.h"
#include "GameCore.h"
#include "LevelManager.h"
#include <algorithm>

DebugState::DebugState(Game* game) : IState(game), selectedOption(0),
    ballSpeedY(-4.0f), paddleSpeed(8.0f), lives(3), brickRows(5) {}

void DebugState::OnEnter() {
    ballSpeedY = game->GetCore()->GetBallSpeedY();
    paddleSpeed = game->GetCore()->GetPaddleSpeed();
    lives = game->GetCore()->GetStartLives();
    brickRows = game->GetLevelManager()->GetBrickRows();
    selectedOption = 0;
}

void DebugState::Update(float) {
    int numOptions = 4;
    if (IsKeyPressed(KEY_DOWN)) selectedOption = (selectedOption + 1) % numOptions;
    if (IsKeyPressed(KEY_UP)) selectedOption = (selectedOption - 1 + numOptions) % numOptions;

    if (selectedOption == 0) { // 球速 Y
        if (IsKeyPressed(KEY_RIGHT)) ballSpeedY = std::min(-2.0f, ballSpeedY + 0.5f);
        if (IsKeyPressed(KEY_LEFT))  ballSpeedY = std::max(-8.0f, ballSpeedY - 0.5f);
    } else if (selectedOption == 1) { // 挡板速度
        if (IsKeyPressed(KEY_RIGHT)) paddleSpeed = std::min(15.0f, paddleSpeed + 0.5f);
        if (IsKeyPressed(KEY_LEFT))  paddleSpeed = std::max(4.0f, paddleSpeed - 0.5f);
    } else if (selectedOption == 2) { // 生命数
        if (IsKeyPressed(KEY_RIGHT)) lives = std::min(9, lives + 1);
        if (IsKeyPressed(KEY_LEFT))  lives = std::max(1, lives - 1);
    } else if (selectedOption == 3) { // 砖块行数
        if (IsKeyPressed(KEY_RIGHT)) brickRows = std::min(8, brickRows + 1);
        if (IsKeyPressed(KEY_LEFT))  brickRows = std::max(3, brickRows - 1);
    }

    if (IsKeyPressed(KEY_Q)) {
        game->GetCore()->SetBallSpeedY(ballSpeedY);
        game->GetCore()->SetPaddleSpeed(paddleSpeed);
        game->GetCore()->SetStartLives(lives);
        game->GetLevelManager()->SetDefaultBrickRows(brickRows);
        game->GetStateMachine()->SwitchTo(GameStateType::MENU);
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        game->GetStateMachine()->SwitchTo(GameStateType::MENU);
    }
}

void DebugState::Draw() {
    int w = game->GetScreenWidth();
    int h = game->GetScreenHeight();
    DrawRectangle(0, 0, w, h, Color{0,0,0,200});
    DrawText("DEBUG MODE - ADJUST SETTINGS", w/2 - 230, 40, 34, YELLOW);

    const char* options[] = {
        TextFormat("BALL SPEED Y: %.1f", ballSpeedY),
        TextFormat("PADDLE SPEED: %.1f", paddleSpeed),
        TextFormat("LIVES: %d", lives),
        TextFormat("BRICK ROWS: %d", brickRows)
    };
    for (int i = 0; i < 4; ++i) {
        Color color = (selectedOption == i) ? RED : WHITE;
        DrawText(options[i], w/2 - 200, 120 + i * 55, 32, color);
    }
    DrawText("UP/DOWN | LEFT/RIGHT | Q SAVE & EXIT | ESC CANCEL", w/2 - 320, h - 60, 24, LIGHTGRAY);
}

void DebugState::OnExit() {}