#pragma once
#include "raylib.h"

class Ball {
private:
    Vector2 pos, vel;
    float radius;
    Color color;                     // 新增：小球颜色
    static const int TRAIL_LENGTH = 8;
    Vector2 trailPositions[TRAIL_LENGTH];
    int trailIndex;
public:
    Ball(Vector2 p, Vector2 v, float r, Color c = RED);
    void Reset(Vector2 np, Vector2 nv);
    void Move(float mul = 1.0f);
    void UpdateTrail();
    void BounceEdge(int w, int h);
    bool CheckPaddleCollision(Rectangle pad);
    bool CheckBrickCollision(Rectangle brk);
    bool CheckBottomDeath(int h);
    void Draw() const;
    void DrawAsClone() const;
    void DrawWithTrail() const;
    Vector2 GetPosition() const { return pos; }
    Vector2 GetVelocity() const { return vel; }
    void SetColor(Color c) { color = c; }
};