#include "GameOverState.h"
#include "Game.h"
#include "StateMachine.h"

GameOverState::GameOverState(Game* game) : IState(game) {}

void GameOverState::OnEnter() { game->PlayGameOverSound(); }

void GameOverState::Update(float) {
    if (IsKeyPressed(KEY_SPACE)) {
        game->ResetGame();
        game->GetStateMachine()->SwitchTo(GameStateType::PLAYING);
    }
    if (IsKeyPressed(KEY_S)) {
        game->OpenArchiveSaveMode();
    }
}

void GameOverState::Draw() {
    int w = game->GetScreenWidth();
    int h = game->GetScreenHeight();
    DrawRectangle(0, 0, w, h, Color{0,0,0,180});
    DrawText("GAME OVER", w/2 - 120, h/2 - 50, 56, RED);     // 字体56
    DrawText("PRESS SPACE TO PLAY AGAIN", w/2 - 200, h/2 + 30, 28, LIGHTGRAY);
    DrawText("PRESS S TO SAVE", w/2 - 110, h/2 + 80, 24, LIGHTGRAY);
    DrawText("PRESS Q TO MENU", w/2 - 100, h/2 + 120, 24, LIGHTGRAY);
}

void GameOverState::OnExit() {}