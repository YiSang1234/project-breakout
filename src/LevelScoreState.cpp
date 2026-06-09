#include "LevelScoreState.h"
#include "Game.h"
#include "StateMachine.h"
#include "LevelManager.h"

LevelScoreState::LevelScoreState(Game* game) : IState(game), previewLevel(1) {}

void LevelScoreState::OnEnter() {
    previewLevel = game->GetCurrentLevel();   // 可选：保留当前进度
    if (previewLevel < 1) previewLevel = 1;
    if (previewLevel > LevelManager::MAX_LEVEL) previewLevel = LevelManager::MAX_LEVEL;
}

void LevelScoreState::Update(float) {
    int total = LevelManager::MAX_LEVEL;
    if (IsKeyPressed(KEY_A) && previewLevel > 1) previewLevel--;
    if (IsKeyPressed(KEY_D) && previewLevel < total) previewLevel++;

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
    DrawText(TextFormat("SELECTED: %d / %d", previewLevel, LevelManager::MAX_LEVEL), w/2 - 150, 110, 30, WHITE);

    // 简单预览砖块布局
    auto layout = game->GetLevelManager()->GetLayout(previewLevel);
    int rows = (int)layout.size();
    int cols = (rows > 0) ? (int)layout[0].size() : 0;
    float cellW = 30, cellH = 15;
    float startX = w/2 - cols*cellW/2;
    float startY = 200;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (layout[r][c] == 1)
                DrawRectangle(startX + c*cellW, startY + r*cellH, cellW-1, cellH-1, RED);
            else
                DrawRectangle(startX + c*cellW, startY + r*cellH, cellW-1, cellH-1, DARKGRAY);
        }
    }
    DrawText("A / D : CHANGE LEVEL", w/2 - 150, h - 80, 24, LIGHTGRAY);
    DrawText("ENTER : START | Q : BACK", w/2 - 150, h - 40, 24, LIGHTGRAY);
}

void LevelScoreState::OnExit() {}