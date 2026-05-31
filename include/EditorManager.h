#pragma once
#include "raylib.h"
#include <vector>

class EditorManager {
private:
    int editingLevel;
    std::vector<std::vector<int>> layout;
    int brickRows, brickCols;
    float brickW, brickH, brickSpace;
    Color selectedColor;
    Vector2 mousePos;

public:
    EditorManager();
    void LoadLevelForEdit(int level);
    void SaveCurrentLevel();
    void Update(float dt);
    void Draw() const;
    int GetEditingLevel() const { return editingLevel; }
    void SetEditingLevel(int level) { editingLevel = level; }
};