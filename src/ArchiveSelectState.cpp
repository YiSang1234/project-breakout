#include "ArchiveSelectState.h"
#include "Game.h"
#include "StateMachine.h"
#include "GameCore.h"      // 新增
#include "Ball.h"          // 新增（如果需要）
#include "Brick.h"         // 新增
#include "SaveManager.h"   // 如果需要
ArchiveSelectState::ArchiveSelectState(Game* game) : IState(game), selectedSlot(1), isSaveMode(false) {}

void ArchiveSelectState::OnEnter() {
    if (!game->GetSaveManager()) {
        game->GetStateMachine()->SwitchTo(GameStateType::MENU);
        return;
    }
    selectedSlot = 1;
    game->GetSaveManager()->RefreshSlotInfos();
}

void ArchiveSelectState::Update(float) {
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN)) {
        selectedSlot = (selectedSlot % 3) + 1;
    }
    if (IsKeyPressed(KEY_ENTER)) {
        if (isSaveMode) {
            game->GetSaveManager()->SaveToSlot(selectedSlot,
                *game->GetCore(), *game->GetLevelManager(),
                *game->GetPowerUpManager(), *game->GetParticleManager(),
                game->GetCore()->GetMultiBalls(), game->GetCore()->GetBricks());
        } else {
            if (game->GetSaveManager()->LoadFromSlot(selectedSlot,
                *game->GetCore(), *game->GetLevelManager(),
                *game->GetPowerUpManager(), *game->GetParticleManager(),
                const_cast<std::vector<Ball>&>(game->GetCore()->GetMultiBalls()), // 注意const_cast
                game->GetCore()->GetBricks())) {
                game->GetStateMachine()->SwitchTo(GameStateType::PLAYING);
                return;
            }
        }
        game->GetStateMachine()->SwitchTo(GameStateType::MENU);
    }
    if (IsKeyPressed(KEY_Q)) {
    game->GetStateMachine()->SwitchTo(GameStateType::MENU);
}
}

void ArchiveSelectState::Draw() {
    ClearBackground(DARKGRAY);
    DrawText("SELECT SAVE SLOT", GetScreenWidth()/2 - 180, 50, 40, YELLOW);   // 字体40
    for (int i = 1; i <= 3; ++i) {
        int y = 130 + (i-1) * 70;
        Color color = (selectedSlot == i) ? SKYBLUE : WHITE;
        std::string info = game->GetSaveManager()->GetSlotInfoText(i);
        DrawText(info.c_str(), GetScreenWidth()/2 - 220, y, 28, color);        // 字体28
        if (selectedSlot == i) {
            DrawRectangleLines(GetScreenWidth()/2 - 240, y - 5, 480, 38, SKYBLUE);
        }
    }
    const char* action = isSaveMode ? "SAVE" : "LOAD";
    DrawText(TextFormat("ENTER: %s | Q: CANCEL", action),
             GetScreenWidth()/2 - 220, GetScreenHeight() - 60, 26, LIGHTGRAY);  // 字体26
}

void ArchiveSelectState::OnExit() {}