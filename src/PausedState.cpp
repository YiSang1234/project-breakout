#include "PausedState.h"
#include "Game.h"
#include "StateMachine.h"

PausedState::PausedState(Game* game) : IState(game) {}

void PausedState::OnEnter() {}

void PausedState::Update(float) {
    if (IsKeyPressed(KEY_P)) {
        game->GetStateMachine()->SwitchTo(GameStateType::PLAYING);
    }
    if (IsKeyPressed(KEY_R)) {
        game->ResetGame();
        game->GetStateMachine()->SwitchTo(GameStateType::PLAYING);
    }
    if (IsKeyPressed(KEY_U)) {
        game->GetStateMachine()->SwitchTo(GameStateType::MENU);
    }
}

void PausedState::Draw() {
    game->DrawGamePlay();
    DrawRectangle(0, 0, game->GetScreenWidth(), game->GetScreenHeight(), Color{0,0,0,150});
    int w = 400, h = 180;  // 面板稍大
    int x = game->GetScreenWidth()/2 - w/2;
    int y = game->GetScreenHeight()/2 - h/2;
    DrawRectangle(x, y, w, h, DARKGRAY);
    DrawRectangleLines(x, y, w, h, YELLOW);
    DrawText("PAUSED", x + 150, y + 30, 40, ORANGE);        // 字体40
    DrawText("P: RESUME", x + 50, y + 90, 28, WHITE);       // 字体28
    DrawText("R: RESTART", x + 200, y + 90, 28, WHITE);
    DrawText("U: MENU", x + 150, y + 130, 28, LIGHTGRAY);
}

void PausedState::OnExit() {}