#pragma once
#include <string>
#include <vector>

class GameCore;
class LevelManager;
class PowerUpManager;
class ParticleManager;
class Ball;
class Brick;

struct SlotInfo {
    bool exists = false;
    int level = 1, score = 0, lives = 3;
    std::string timestamp;
};

class SaveManager {
private:
    std::string slotPaths[4];
    SlotInfo slotInfos[4];

public:
    SaveManager();
    void RefreshSlotInfos();
    bool SaveToSlot(int slot, const GameCore& core, const LevelManager& level,
                    const PowerUpManager& powerUp, const ParticleManager& particles,
                    const std::vector<Ball>& multiBalls, const std::vector<Brick>& bricks);
    bool LoadFromSlot(int slot, GameCore& core, LevelManager& level,
                      PowerUpManager& powerUp, ParticleManager& particles,
                      std::vector<Ball>& multiBalls, std::vector<Brick>& bricks);
    std::string GetSlotInfoText(int slot) const;
    bool SlotExists(int slot) const;
    int GetSlotLevel(int slot) const;
    int GetSlotScore(int slot) const;
    int GetSlotLives(int slot) const;
};