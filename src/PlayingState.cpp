#include "PlayingState.h"
#include "Game.h"
#include "StateMachine.h"
#include "LevelManager.h"

PlayingState::PlayingState(Game* game) : IState(game) {}

void PlayingState::OnEnter() {}

void PlayingState::Update(float dt) {
    if (IsKeyPressed(KEY_P)) {
        game->GetStateMachine()->SwitchTo(GameStateType::PAUSED);
        return;
    }
    if (IsKeyPressed(KEY_S)) {
        game->OpenArchiveSaveMode();
        return;
    }
    if (IsKeyPressed(KEY_K)) {
        game->StartBallAsyncLoad();
    }
    if (IsKeyPressed(KEY_J)) {
        game->StartBrickAsyncLoad();
    }

    game->UpdateGamePlay(dt);
    game->UpdateAsyncLoad();

    if (game->IsBallAsyncComplete()) {
        game->ApplyRandomBallColors();
        game->ResetBallAsync();
    }
    if (game->IsBrickAsyncComplete()) {
        game->ApplyRandomBrickColors();
        game->ResetBrickAsync();
    }

    if (game->IsGameOver()) {
        game->GetStateMachine()->SwitchTo(GameStateType::GAME_OVER);
    } else if (game->IsGameWin()) {
        game->GetStateMachine()->SwitchTo(GameStateType::GAME_WIN);
    }
}

void PlayingState::Draw() {
    game->DrawGamePlay();
    if (game->IsAnyAsyncLoading()) {
        DrawRectangle(0, 0, game->GetScreenWidth(), game->GetScreenHeight(), Color{0,0,0,180});
        const char* text = "LOADING...";
        int tw = MeasureText(text, 40);
        DrawText(text, game->GetScreenWidth()/2 - tw/2, game->GetScreenHeight()/2 - 20, 40, WHITE);
    }
}

void PlayingState::OnExit() {}