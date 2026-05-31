#include "EditorManager.h"
#include <fstream>
#include "json.hpp"
using json = nlohmann::json;

EditorManager::EditorManager() : editingLevel(1), brickRows(5), brickCols(10),
    brickW(70), brickH(25), brickSpace(5), selectedColor(RED), mousePos{0,0} {}

void EditorManager::LoadLevelForEdit(int level) {
    editingLevel = level;
    std::string filename = "levels/level_" + std::to_string(level) + ".json";
    std::ifstream file(filename);
    if (!file.is_open()) {
        layout.assign(5, std::vector<int>(10, 1));
        brickRows = 5; brickCols = 10;
        return;
    }
    json config;
    try { file >> config; } catch(...) { layout.assign(5, std::vector<int>(10, 1)); return; }
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
        int rows = config.value("brick_rows", 5);
        int cols = config.value("brick_cols", 10);
        layout.assign(rows, std::vector<int>(cols, 1));
        brickRows = rows; brickCols = cols;
    }
    brickW = config.value("brick_width", 70.0f);
    brickH = config.value("brick_height", 25.0f);
    brickSpace = config.value("brick_spacing", 5.0f);
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

void EditorManager::Update(float) {
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
}

void EditorManager::Draw() const {
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
    int row = (int)((mousePos.y - 60) / (brickH + brickSpace));
    int col = (int)((mousePos.x - startX) / (brickW + brickSpace));
    if (row >= 0 && row < brickRows && col >= 0 && col < brickCols) {
        float x = startX + col * (brickW + brickSpace);
        float y = 60 + row * (brickH + brickSpace);
        DrawRectangleLinesEx({x, y, brickW, brickH}, 3, YELLOW);
    }
    DrawText(TextFormat("EDIT MODE - Level %d", editingLevel), 20, 20, 24, WHITE);
    int brickCount = 0;
    for (auto& row : layout) for (int v : row) if (v) brickCount++;
    DrawText(TextFormat("BRICKS: %d", brickCount), GetScreenWidth() - 150, 22, 20, YELLOW);
    DrawText("LMB ADD | RMB DEL | S SAVE | U EXIT", 20, GetScreenHeight() - 30, 18, LIGHTGRAY);
}