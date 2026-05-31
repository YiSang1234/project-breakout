#include "LevelManager.h"
#include "json.hpp"
#include <fstream>
#include <iostream>
#include "LevelManager.h"
#include <unordered_map>
using json = nlohmann::json;

std::vector<std::vector<int>> LevelManager::GetLayout(int level) const {
    // 静态缓存，所有实例共享，避免重复读取文件
    static std::unordered_map<int, std::vector<std::vector<int>>> cache;
    auto it = cache.find(level);
    if (it != cache.end()) {
        return it->second;
    }

    std::string filename = "levels/level_" + std::to_string(level) + ".json";
    std::ifstream file(filename);
    std::vector<std::vector<int>> layoutData;

    if (!file.is_open()) {
        // 文件不存在，生成默认布局（根据难度递增）
        int rows = 5 + (level - 1);
        if (rows > 8) rows = 8;
        int cols = 10;
        layoutData.assign(rows, std::vector<int>(cols, 1));
    } else {
        json config;
        try {
            file >> config;
            if (config.contains("layout") && config["layout"].is_array()) {
                for (auto& row : config["layout"]) {
                    std::vector<int> rowData;
                    for (auto& cell : row) rowData.push_back(cell.get<int>());
                    layoutData.push_back(rowData);
                }
            } else {
                int rows = config.value("brick_rows", 5);
                int cols = config.value("brick_cols", 10);
                layoutData.assign(rows, std::vector<int>(cols, 1));
            }
        } catch (...) {
            // JSON 解析失败，使用默认
            int rows = 5 + (level - 1);
            if (rows > 8) rows = 8;
            int cols = 10;
            layoutData.assign(rows, std::vector<int>(cols, 1));
        }
    }

    cache[level] = layoutData;
    return layoutData;
}

static Color StringToColor(const std::string& name) {
    if (name == "RED") return RED;
    if (name == "ORANGE") return ORANGE;
    if (name == "YELLOW") return YELLOW;
    if (name == "GREEN") return GREEN;
    if (name == "BLUE") return BLUE;
    if (name == "PURPLE") return PURPLE;
    if (name == "SKYBLUE") return SKYBLUE;
    if (name == "PINK") return PINK;
    return WHITE;
}

LevelManager::LevelManager() : brickRows(5), brickCols(10), brickW(70), brickH(25), brickSpace(5),
    ballSpeedX(4.0f), ballSpeedY(-4.0f), defaultBrickHitPoints(1) {}

bool LevelManager::LoadLevel(int level) {
    std::string filename = "levels/level_" + std::to_string(level) + ".json";
    std::ifstream file(filename);
    if (!file.is_open()) {
        // 使用默认配置
        brickRows = 5 + (level-1);
        if (brickRows > 8) brickRows = 8;
        brickCols = 10;
        brickW = 70;
        brickH = 25;
        brickSpace = 5;
        ballSpeedX = 4.0f + (level-1)*0.5f;
        ballSpeedY = -4.0f - (level-1)*0.5f;
        layout.assign(brickRows, std::vector<int>(brickCols, 1));
        rowColors.clear();
        rowScores.clear();
        Color defaultColors[] = {RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE, SKYBLUE, PINK};
        for (int r = 0; r < brickRows; ++r) {
            rowColors.push_back(defaultColors[r % 8]);
            rowScores.push_back(10);
        }
        return false;
    }
    json config;
    try { file >> config; } catch(...) { return false; }
    ballSpeedX = config.value("ball_speed_x", 4.0f);
    ballSpeedY = config.value("ball_speed_y", -4.0f);
    brickRows = config.value("brick_rows", 5);
    brickCols = config.value("brick_cols", 10);
    brickW = config.value("brick_width", 70.0f);
    brickH = config.value("brick_height", 25.0f);
    brickSpace = config.value("brick_spacing", 5.0f);
    if (config.contains("layout") && config["layout"].is_array()) {
        layout.clear();
        for (auto& row : config["layout"]) {
            std::vector<int> rowData;
            for (auto& cell : row) rowData.push_back(cell.get<int>());
            layout.push_back(rowData);
        }
        if (!layout.empty()) {
            brickRows = (int)layout.size();
            brickCols = (int)layout[0].size();
        }
    } else {
        layout.assign(brickRows, std::vector<int>(brickCols, 1));
    }
    rowColors.clear();
    if (config.contains("row_colors") && config["row_colors"].is_array()) {
        for (auto& colStr : config["row_colors"])
            rowColors.push_back(StringToColor(colStr.get<std::string>()));
    }
    while ((int)rowColors.size() < brickRows) rowColors.push_back(RED);
    rowScores.clear();
    if (config.contains("row_scores") && config["row_scores"].is_array()) {
        for (auto& sc : config["row_scores"])
            rowScores.push_back(sc.get<int>());
    }
    while ((int)rowScores.size() < brickRows) rowScores.push_back(10);
    return true;
}

std::vector<Brick> LevelManager::GenerateBricks(float screenWidth, int startY) {
    std::vector<Brick> bricks;
    float startX = (screenWidth - (brickCols * (brickW + brickSpace))) / 2.0f;
    for (int r = 0; r < brickRows; ++r) {
        if (r >= (int)layout.size()) break;
        for (int c = 0; c < brickCols; ++c) {
            if (c >= (int)layout[r].size()) break;
            if (layout[r][c] == 0) continue;
            float x = startX + c * (brickW + brickSpace);
            float y = startY + r * (brickH + brickSpace);
            Color col = (r < (int)rowColors.size()) ? rowColors[r] : RED;
            int scoreVal = (r < (int)rowScores.size()) ? rowScores[r] : 10;
            bricks.emplace_back(x, y, brickW, brickH, col, scoreVal, defaultBrickHitPoints);
        }
    }
    return bricks;
}

bool LevelManager::SaveLevel(int level, const std::vector<std::vector<int>>& newLayout,
                             int rows, int cols, float bw, float bh, float bspace) {
    std::string filename = "levels/level_" + std::to_string(level) + ".json";
    json config;
    std::ifstream inFile(filename);
    if (inFile.is_open()) {
        try { inFile >> config; } catch(...) { config = json::object(); }
    }
    config["layout"] = newLayout;
    config["brick_rows"] = rows;
    config["brick_cols"] = cols;
    config["brick_width"] = bw;
    config["brick_height"] = bh;
    config["brick_spacing"] = bspace;
    if (!config.contains("ball_speed_x")) config["ball_speed_x"] = 4.0f;
    if (!config.contains("ball_speed_y")) config["ball_speed_y"] = -4.0f;
    if (!config.contains("row_colors")) {
        std::vector<std::string> colors = {"RED","ORANGE","YELLOW","GREEN","BLUE","PURPLE"};
        json colorsArr;
        for (int i = 0; i < rows; ++i)
            colorsArr.push_back(colors[i % colors.size()]);
        config["row_colors"] = colorsArr;
    }
    if (!config.contains("row_scores")) {
        json scoresArr;
        for (int i = 0; i < rows; ++i) scoresArr.push_back(10);
        config["row_scores"] = scoresArr;
    }
    std::ofstream outFile(filename);
    if (outFile.is_open()) { outFile << config.dump(4); return true; }
    return false;
}

void LevelManager::SetDefaultBrickRows(int rows) {
    brickRows = rows;
}