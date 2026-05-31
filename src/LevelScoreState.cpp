#include "LevelScoreState.h"
#include "Game.h"
#include "StateMachine.h"
#include "LevelManager.h"

LevelScoreState::LevelScoreState(Game* game) : IState(game), previewLevel(1) {}

void LevelScoreState::OnEnter() {
    previewLevel = game->GetCurrentLevel();
}

void LevelScoreState::Update(float) {
    int unlocked = game->GetUnlockedLevel();
    if (IsKeyPressed(KEY_A) && previewLevel > 1) previewLevel--;
    if (IsKeyPressed(KEY_D) && previewLevel < unlocked) previewLevel++;
    if (IsKeyPressed(KEY_ENTER)) {
        game->GotoLevel(previewLevel);
        game->ResetGame();
        game->GetStateMachine()->SwitchTo(GameStateType::PLAYING);
    }
    if (IsKeyPressed(KEY_Q) || IsKeyPressed(KEY_ESCAPE)) {
        game->GetStateMachine()->SwitchTo(GameStateType::MENU);
    }
}

void LevelScoreState::Draw() {
    int w = game->GetScreenWidth();
    int h = game->GetScreenHeight();
    DrawRectangle(0, 0, w, h, Color{0,0,80,255});
    DrawText("LEVEL SELECT", w/2 - 150, 40, 50, YELLOW);
    DrawText(TextFormat("SELECTED: %d / %d", previewLevel, game->GetUnlockedLevel()), w/2 - 150, 110, 30, WHITE);

    auto layout = game->GetLevelManager()->GetLayout(previewLevel);
    int rows = (int)layout.size();
    int cols = (rows > 0) ? (int)layout[0].size() : 0;
    float cellW = 30, cellH = 15;
    float startX = w/2 - cols*cellW/2;
    float startY = 200;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (layout[r][c] == 1) {
                DrawRectangle(startX + c*cellW, startY + r*cellH, cellW-1, cellH-1, RED);
            } else {
                DrawRectangle(startX + c*cellW, startY + r*cellH, cellW-1, cellH-1, DARKGRAY);
            }
        }
    }
    DrawText("LEVEL HIGH SCORES:", w/2 - 150, startY + rows*cellH + 30, 24, LIGHTGRAY);
    for (int i = 1; i <= game->GetUnlockedLevel(); ++i) {
        int score = game->GetLevelHighScore(i);
        DrawText(TextFormat("Level %d: %d", i, score), w/2 - 100, startY + rows*cellH + 60 + i*25, 20, (i == previewLevel) ? YELLOW : WHITE);
    }
    DrawText("A / D : CHANGE LEVEL", w/2 - 150, h - 80, 24, LIGHTGRAY);
    DrawText("ENTER : START | Q : BACK", w/2 - 150, h - 40, 24, LIGHTGRAY);
}

void LevelScoreState::OnExit() {}