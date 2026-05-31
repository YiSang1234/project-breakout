#pragma once
#include "raylib.h"

class Paddle {
private:
    Rectangle rect;
    float speed;
    Color color;
public:
    Paddle(float x, float y, float w, float h, float s, Color c = SKYBLUE);
    void MoveLeft(int screenW);
    void MoveRight(int screenW);
    void SetX(float x) { rect.x = x; }
    Rectangle GetRect() const { return rect; }
    void SetScale(float scale);
    void Draw() const;
};