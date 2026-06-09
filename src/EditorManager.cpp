#include "EditorManager.h"
#include <fstream>
#include "json.hpp"
using json = nlohmann::json;

EditorManager::EditorManager() : editingLevel(1), brickRows(5), brickCols(10),
    brickW(70), brickH(25), brickSpace(5), selectedColor(RED), maxUnlockedLevel(1) {}

void EditorManager::LoadLevelForEdit(int level) {
    editingLevel = level;
    std::string filename = "levels/level_" + std::to_string(level) + ".json";
    std::ifstream file(filename);
    if (!file.is_open()) {
        layout.assign(5, std::vector<int>(10, 1));
        brickRows = 5;
        brickCols = 10;
        brickW = 70;
        brickH = 25;
        brickSpace = 5;
        return;
    }
    json config;
    try {
        file >> config;
        if (config.contains("layout") && config["layout"].is_array()) {
            layout.clear();
            for (auto& row : config["layout"]) {
                std::vector<int> rowData;
                for (auto& cell : row) rowData.push_back(cell.get<int>());
                layout.push_back(rowData);
            }
            brickRows = (int)layout.size();
            brickCols = (int)layout[0].size();
        } else {
            brickRows = config.value("brick_rows", 5);
            brickCols = config.value("brick_cols", 10);
            layout.assign(brickRows, std::vector<int>(brickCols, 1));
        }
        brickW = config.value("brick_width", 70.0f);
        brickH = config.value("brick_height", 25.0f);
        brickSpace = config.value("brick_spacing", 5.0f);
    } catch (...) {
        layout.assign(5, std::vector<int>(10, 1));
        brickRows = 5;
        brickCols = 10;
    }
}

void EditorManager::SaveCurrentLevel() {
    std::string filename = "levels/level_" + std::to_string(editingLevel) + ".json";
    json config;
    std::ifstream inFile(filename);
    if (inFile.is_open()) {
        try { inFile >> config; } catch(...) { config = json::object(); }
    }
    config["layout"] = layout;
    config["brick_rows"] = brickRows;
    config["brick_cols"] = brickCols;
    config["brick_width"] = brickW;
    config["brick_height"] = brickH;
    config["brick_spacing"] = brickSpace;
    if (!config.contains("ball_speed_x")) config["ball_speed_x"] = 4.0f;
    if (!config.contains("ball_speed_y")) config["ball_speed_y"] = -4.0f;
    if (!config.contains("row_colors")) {
        std::vector<std::string> colors = {"RED","ORANGE","YELLOW","GREEN","BLUE","PURPLE"};
        json colorsArr;
        for (int i = 0; i < brickRows; ++i)
            colorsArr.push_back(colors[i % colors.size()]);
        config["row_colors"] = colorsArr;
    }
    if (!config.contains("row_scores")) {
        json scoresArr;
        for (int i = 0; i < brickRows; ++i) scoresArr.push_back(10);
        config["row_scores"] = scoresArr;
    }
    std::ofstream outFile(filename);
    if (outFile.is_open()) outFile << config.dump(4);
}

void EditorManager::SwitchToLevel(int delta) {
    int newLevel = editingLevel + delta;
    if (newLevel < 1 || newLevel > maxUnlockedLevel) return;
    editingLevel = newLevel;
    LoadLevelForEdit(editingLevel);
}

void EditorManager::Update(float /*dt*/) {
    mousePos = GetMousePosition();
    float startX = (GetScreenWidth() - (brickCols * (brickW + brickSpace))) / 2.0f;
    int col = (int)((mousePos.x - startX) / (brickW + brickSpace));
    int row = (int)((mousePos.y - 60) / (brickH + brickSpace));

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (row >= 0 && row < brickRows && col >= 0 && col < brickCols)
            layout[row][col] = 1;
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        if (row >= 0 && row < brickRows && col >= 0 && col < brickCols)
            layout[row][col] = 0;
    }
    if (IsKeyPressed(KEY_S)) SaveCurrentLevel();
    if (IsKeyPressed(KEY_A)) SwitchToLevel(-1);
    if (IsKeyPressed(KEY_D)) SwitchToLevel(1);
}

void EditorManager::Draw() const {
    // 注意：不要在此处调用 ClearBackground，主循环已清屏
    float startX = (GetScreenWidth() - (brickCols * (brickW + brickSpace))) / 2.0f;
    for (int r = 0; r < brickRows; ++r) {
        for (int c = 0; c < brickCols; ++c) {
            if (layout[r][c] == 0) continue;
            float x = startX + c * (brickW + brickSpace);
            float y = 60 + r * (brickH + brickSpace);
            DrawRectangleRec({x, y, brickW, brickH}, selectedColor);
            DrawRectangleLinesEx({x, y, brickW, brickH}, 1, WHITE);
        }
    }
    // 高亮
    int row = (int)((mousePos.y - 60) / (brickH + brickSpace));
    int col = (int)((mousePos.x - startX) / (brickW + brickSpace));
    if (row >= 0 && row < brickRows && col >= 0 && col < brickCols) {
        float x = startX + col * (brickW + brickSpace);
        float y = 60 + row * (brickH + brickSpace);
        DrawRectangleLinesEx({x, y, brickW, brickH}, 3, YELLOW);
    }
    DrawText(TextFormat("EDIT MODE - Level %d / %d", editingLevel, maxUnlockedLevel), 20, 20, 28, WHITE);
    int brickCount = 0;
    for (auto& row : layout) for (int v : row) if (v) brickCount++;
    DrawText(TextFormat("BRICKS: %d", brickCount), GetScreenWidth() - 150, 22, 24, YELLOW);
    DrawText("LMB ADD | RMB DEL | S SAVE | A/D SWITCH LEVEL | Q EXIT", 20, GetScreenHeight() - 40, 24, LIGHTGRAY);
}