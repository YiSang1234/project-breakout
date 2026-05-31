#include "Paddle.h"

Paddle::Paddle(float x, float y, float w, float h, float s, Color c)
    : rect{x,y,w,h}, speed(s), color(c) {}

void Paddle::MoveLeft(int screenW) {
    rect.x -= speed;
    if (rect.x < 0) rect.x = 0;
}
void Paddle::MoveRight(int screenW) {
    rect.x += speed;
    if (rect.x + rect.width > screenW) rect.x = screenW - rect.width;
}
void Paddle::SetScale(float scale) {
    float old = rect.width;
    rect.width = 100 * scale;
    rect.x -= (rect.width - old) * 0.5f;
}
void Paddle::Draw() const {
    DrawRectangleRec(rect, color);
    DrawRectangleLinesEx(rect, 1, DARKBLUE);
}