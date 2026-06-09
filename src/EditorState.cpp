#include "EditorState.h"
#include "Game.h"
#include "StateMachine.h"
#include "EditorManager.h"
#include "LevelManager.h"

EditorState::EditorState(Game* game) : IState(game) {}

void EditorState::OnEnter() {
    int total = LevelManager::MAX_LEVEL;   // 总关卡数 5
    game->GetEditorManager()->SetMaxUnlockedLevel(total);
    // 默认编辑第一关（也可以从当前关卡读取）
    game->GetEditorManager()->SetEditingLevel(1);
    game->GetEditorManager()->LoadLevelForEdit(1);
}

void EditorState::Update(float dt) {
    auto* mgr = game->GetEditorManager();
    mgr->Update(dt);

    // A/D 切换关卡（已由 EditorManager 处理）
    // Q 退出
    if (IsKeyPressed(KEY_Q) || IsKeyPressed(KEY_ESCAPE)) {
        game->GetStateMachine()->SwitchTo(GameStateType::MENU);
    }
}

void EditorState::Draw() {
    game->GetEditorManager()->Draw();
}

void EditorState::OnExit() {}