#include "EditorState.h"
#include "Game.h"
#include "StateMachine.h"
#include "EditorManager.h"

EditorState::EditorState(Game* game) : IState(game) {}

void EditorState::OnEnter() {
    int level = game->GetUnlockedLevel();
    game->GetEditorManager()->SetEditingLevel(level);
    game->GetEditorManager()->LoadLevelForEdit(level);
}

void EditorState::Update(float dt) {
    game->GetEditorManager()->Update(dt);
    if (IsKeyPressed(KEY_Q)) {
        game->GetEditorManager()->SaveCurrentLevel();
        game->GetStateMachine()->SwitchTo(GameStateType::MENU);
    }
    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_U)) {
        game->GetStateMachine()->SwitchTo(GameStateType::MENU);
    }
}

void EditorState::Draw() {
    game->GetEditorManager()->Draw();
}

void EditorState::OnExit() {}