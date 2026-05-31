#include "SaveManager.h"
#include "GameCore.h"
#include "LevelManager.h"
#include "PowerUpManager.h"
#include "ParticleManager.h"
#include "Ball.h"
#include "Brick.h"
#include "json.hpp"
#include <fstream>
#include <ctime>
using json = nlohmann::json;

SaveManager::SaveManager() {
    for (int i = 1; i <= 3; ++i) {
        slotPaths[i] = "saves/save" + std::to_string(i) + ".json";
    }
}

void SaveManager::RefreshSlotInfos() {
    for (int i = 1; i <= 3; ++i) {
        std::ifstream file(slotPaths[i]);
        if (!file.is_open()) {
            slotInfos[i].exists = false;
            continue;
        }
        json save;
        try {
            file >> save;
            slotInfos[i].exists = true;
            slotInfos[i].level = save.value("current_level", 1);
            slotInfos[i].score = save.value("score", 0);
            slotInfos[i].lives = save.value("player_life", 3);
            slotInfos[i].timestamp = save.value("timestamp", "");
        } catch (...) {
            slotInfos[i].exists = false;
        }
    }
}

bool SaveManager::SaveToSlot(int slot, const GameCore& core, const LevelManager& level,
                             const PowerUpManager& powerUp, const ParticleManager&,
                             const std::vector<Ball>& multiBalls, const std::vector<Brick>& bricks) {
    json save;
    save["score"] = core.GetScore();
    save["player_life"] = core.GetPlayerLife();
    save["current_level"] = core.GetCurrentLevel();
    save["unlocked_level"] = core.GetUnlockedLevel();
    save["high_score"] = core.GetHighScore();
    save["game_time"] = core.GetGameTime();
    save["ball_served"] = core.IsBallServed();
    save["powerup"]["paddle_longer"] = powerUp.GetPaddleLongerTime();
    save["powerup"]["slow_ball"] = powerUp.GetSlowBallTime();
    save["main_ball"]["position"]["x"] = core.GetBall().GetPosition().x;
    save["main_ball"]["position"]["y"] = core.GetBall().GetPosition().y;
    save["main_ball"]["velocity"]["x"] = core.GetBall().GetVelocity().x;
    save["main_ball"]["velocity"]["y"] = core.GetBall().GetVelocity().y;
    json multiJson = json::array();
    for (auto& mb : multiBalls) {
        multiJson.push_back({{"x", mb.GetPosition().x}, {"y", mb.GetPosition().y},
                             {"vx", mb.GetVelocity().x}, {"vy", mb.GetVelocity().y}});
    }
    save["multi_balls"] = multiJson;
    save["paddle_x"] = core.GetPaddle1().GetRect().x;
    std::vector<int> brickStates;
    for (auto& b : bricks) brickStates.push_back(b.IsActive() ? 1 : 0);
    save["brick_states"] = brickStates;
    char timeBuf[20];
    std::time_t t = std::time(nullptr);
    std::strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", std::localtime(&t));
    save["timestamp"] = timeBuf;
    std::ofstream file(slotPaths[slot]);
    if (file.is_open()) { file << save.dump(4); return true; }
    return false;
}

bool SaveManager::LoadFromSlot(int slot, GameCore& core, LevelManager& level,
                               PowerUpManager& powerUp, ParticleManager&,
                               std::vector<Ball>& multiBalls, std::vector<Brick>& bricks) {
    std::ifstream file(slotPaths[slot]);
    if (!file.is_open()) return false;
    json save;
    try { file >> save; } catch(...) { return false; }
    core.SetScore(save.value("score", 0));
    core.SetPlayerLife(save.value("player_life", 3));
    core.SetCurrentLevel(save.value("current_level", 1));
    core.SetUnlockedLevel(save.value("unlocked_level", 1));
    core.SetGameTime(save.value("game_time", 0.0f));
    bool ballServed = save.value("ball_served", false);
    core.SetBallServed(ballServed);
    // 道具时间
    float pl = save["powerup"].value("paddle_longer", 0.0f);
    float sb = save["powerup"].value("slow_ball", 0.0f);
    // 需要间接设置，通过PowerUpManager
    // 这里简化，只恢复核心
    if (save.contains("main_ball")) {
        float px = save["main_ball"]["position"]["x"];
        float py = save["main_ball"]["position"]["y"];
        float vx = save["main_ball"]["velocity"]["x"];
        float vy = save["main_ball"]["velocity"]["y"];
        core.GetBall().Reset({px, py}, {vx, vy});
    }
    multiBalls.clear();
    if (save.contains("multi_balls")) {
        for (auto& mb : save["multi_balls"]) {
            multiBalls.emplace_back(Vector2{mb["x"], mb["y"]}, Vector2{mb["vx"], mb["vy"]}, 10.0f);
        }
    }
    if (save.contains("paddle_x")) {
        core.GetPaddle1().SetX(save["paddle_x"]);
    }
    // 关卡配置重新加载
    level.LoadLevel(core.GetCurrentLevel());
    bricks = level.GenerateBricks(800);
    if (save.contains("brick_states") && save["brick_states"].is_array()) {
        auto& states = save["brick_states"];
        if (states.size() == bricks.size()) {
            for (size_t i = 0; i < bricks.size(); ++i)
                bricks[i].SetActive(states[i] == 1);
        }
    }
    return true;
}

std::string SaveManager::GetSlotInfoText(int slot) const {
    if (!slotInfos[slot].exists)
        return "Slot " + std::to_string(slot) + " : [EMPTY]";
    else
        return "Slot " + std::to_string(slot) + " : Level " + std::to_string(slotInfos[slot].level) +
               "  Score " + std::to_string(slotInfos[slot].score) +
               "  Lives " + std::to_string(slotInfos[slot].lives);
}