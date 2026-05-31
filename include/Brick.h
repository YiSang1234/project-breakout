#pragma once
#include "raylib.h"

class Brick {
private:
    Rectangle rect;
    Color color;
    bool active;
    int score;
    int hitPoints;
    int maxHitPoints;
public:
    Brick() : rect{0,0,0,0}, color(WHITE), active(false), score(0), hitPoints(0), maxHitPoints(0) {}
    Brick(float x, float y, float w, float h, Color c, int s, int hp = 1);
    
    bool IsActive() const { return active; }
    void SetActive(bool a) { active = a; }
    Rectangle GetRect() const { return rect; }
    int GetScoreValue() const { return score; }
    Color GetColor() const;
    void SetColor(Color c) { color = c; }
    int GetHitPoints() const { return hitPoints; }
    void SetHitPoints(int hp);
    void DecrementHitPoints();
    void Draw() const;
};