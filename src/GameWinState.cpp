#include "GameWinState.h"
#include "Game.h"
#include "StateMachine.h"
#include "GameCore.h"
#include "LevelManager.h"

GameWinState::GameWinState(Game* game) : IState(game) {}

void GameWinState::OnEnter() {}

void GameWinState::Update(float) {
    if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
        game->ResetGame();
        game->GetStateMachine()->SwitchTo(GameStateType::PLAYING);
        return;
    }
    if (IsKeyPressed(KEY_Q)) {
        game->ResetGame();
        game->GetStateMachine()->SwitchTo(GameStateType::MENU);
    }
}

void GameWinState::Draw() {
    int w = game->GetScreenWidth();
    int h = game->GetScreenHeight();
    // 金色渐变背景
    DrawText(TextFormat("LIVES REMAINING: %d", game->GetCore()->GetPlayerLife()), w/2 - 150, h/2 + 10, 30, WHITE);
    DrawText(TextFormat("LEVEL: %d / %d", game->GetCurrentLevel(), LevelManager::MAX_LEVEL), w/2 - 120, h/2 + 50, 30, WHITE);
    for (int i = 0; i < h; ++i) {
        float t = (float)i / h;
        Color col = { (unsigned char)(255 * t), (unsigned char)(200 * t), (unsigned char)(50 * t), 255 };
        DrawLine(0, i, w, i, col);
    }
    DrawText("VICTORY!", w/2 - 100, h/2 - 100, 60, DARKBLUE);
    DrawText(TextFormat("SCORE: %d", game->GetScore()), w/2 - 100, h/2 - 30, 30, WHITE);
    DrawText(TextFormat("LIVES REMAINING: %d", game->GetCore()->GetPlayerLife()), w/2 - 150, h/2 + 10, 30, WHITE);
    DrawText(TextFormat("LEVEL: %d / %d", game->GetCurrentLevel(), 5), w/2 - 120, h/2 + 50, 30, WHITE);
    DrawText("PRESS SPACE TO PLAY AGAIN   OR   Q TO MENU", w/2 - 300, h - 80, 24, YELLOW);
}

void GameWinState::OnExit() {}