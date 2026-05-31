// LevelManager.h
#pragma once
#include "Brick.h"
#include <vector>
#include <string>

class LevelManager {
public:
    static const int MAX_LEVEL = 5;   // 移到 public

    LevelManager();
    bool LoadLevel(int level);
    std::vector<Brick> GenerateBricks(float screenWidth, int startY = 60);
    bool SaveLevel(int level, const std::vector<std::vector<int>>& newLayout,
                   int rows, int cols, float bw, float bh, float bspace);
    void SetDefaultBrickRows(int rows);
    int GetBrickRows() const { return brickRows; }
    int GetBrickCols() const { return brickCols; }
    float GetBrickWidth() const { return brickW; }
    float GetBrickHeight() const { return brickH; }
    float GetBrickSpacing() const { return brickSpace; }
    float GetBallSpeedX() const { return ballSpeedX; }
    float GetBallSpeedY() const { return ballSpeedY; }

    // 新增：获取指定关卡的布局（用于预览）
    std::vector<std::vector<int>> GetLayout(int level) const;

private:
    int brickRows, brickCols;
    float brickW, brickH, brickSpace;
    float ballSpeedX, ballSpeedY;
    std::vector<std::vector<int>> layout;
    std::vector<Color> rowColors;
    std::vector<int> rowScores;
    int defaultBrickHitPoints;
};