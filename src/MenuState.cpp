#include "MenuState.h"
#include "Game.h"
#include "StateMachine.h"

MenuState::MenuState(Game* game) : IState(game), selectedOption(0), waitingConfirm(false) {}

void MenuState::OnEnter() {
    selectedOption = 0;
    waitingConfirm = false;
}

void MenuState::Update(float) {
    int numOptions = 7;  // 增加重置选项
    if (waitingConfirm) {
        // 等待用户确认 Y/N
        if (IsKeyPressed(KEY_Y)) {
            game->ResetAllProgress();
            waitingConfirm = false;
        } else if (IsKeyPressed(KEY_N) || IsKeyPressed(KEY_ESCAPE)) {
            waitingConfirm = false;
        }
        return;
    }

    if (IsKeyPressed(KEY_DOWN)) selectedOption = (selectedOption + 1) % numOptions;
    if (IsKeyPressed(KEY_UP)) selectedOption = (selectedOption - 1 + numOptions) % numOptions;

    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        switch (selectedOption) {
            case 0: // 单人模式
                game->SetTwoPlayerMode(false);
                game->ResetGame();
                game->GetStateMachine()->SwitchTo(GameStateType::PLAYING);
                break;
            case 1: // 双人模式
                game->SetTwoPlayerMode(true);
                game->ResetGame();
                game->GetStateMachine()->SwitchTo(GameStateType::PLAYING);
                break;
            case 2: // 关卡选择
                game->GetStateMachine()->SwitchTo(GameStateType::LEVEL_SCORE);
                break;
            case 3: // 加载存档
                game->OpenArchiveLoadMode();
                break;
            case 4: // 编辑器
                game->StartEditMode();
                break;
            case 5: // 调试模式
                game->GetStateMachine()->SwitchTo(GameStateType::DEBUG);
                break;
            case 6: // 重置所有进度
                waitingConfirm = true;
                break;
        }
    }
}

void MenuState::Draw() {
    int w = game->GetScreenWidth();
    int h = game->GetScreenHeight();
    // 背景渐变
    for (int i = 0; i < h; ++i) {
        float t = (float)i / h;
        Color col = { (unsigned char)(20 + 30 * t), (unsigned char)(20 + 50 * t), (unsigned char)(70 + 100 * t), 255 };
        DrawLine(0, i, w, i, col);
    }
    DrawText("BREAKOUT", w/2 - 150, 60, 60, DARKGRAY);
    DrawText("BREAKOUT", w/2 - 152, 58, 60, YELLOW);
    DrawText(TextFormat("HIGH SCORE: %d", game->GetHighScore()), w - 240, 30, 28, LIGHTGRAY);

    const char* options[] = { "START (1P)", "START (2P)", "LEVEL SELECT", "LOAD SAVE", "EDITOR", "DEBUG MODE", "RESET ALL PROGRESS" };
    for (int i = 0; i < 7; ++i) {
        int y = 180 + i * 45;
        Color color = (selectedOption == i) ? ORANGE : WHITE;
        DrawText(options[i], w/2 - MeasureText(options[i], 30)/2, y, 30, color);
    }
    DrawText("UP/DOWN | ENTER/SPACE", w/2 - 150, h - 70, 24, GRAY);
    DrawText("HOST/JOIN coming soon", w/2 - 120, h - 40, 18, LIGHTGRAY);

    if (waitingConfirm) {
        DrawRectangle(0, 0, w, h, Color{0,0,0,200});
        DrawText("ARE YOU SURE? (Y/N)", w/2 - 150, h/2 - 20, 30, RED);
    }
}

void MenuState::OnExit() {}