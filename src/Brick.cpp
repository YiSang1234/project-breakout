#include "Brick.h"
#include <cstdio>

Brick::Brick(float x, float y, float w, float h, Color c, int s, int hp)
    : rect{x,y,w,h}, color(c), active(true), score(s), hitPoints(hp), maxHitPoints(hp) {}

void Brick::SetHitPoints(int hp) {
    hitPoints = hp;
    maxHitPoints = hp;
}

void Brick::DecrementHitPoints() {
    if (!active) return;
    hitPoints--;
    if (hitPoints <= 0) active = false;
}

Color Brick::GetColor() const {
    if (!active) return BLANK;
    if (maxHitPoints <= 1) return color;
    float t = (float)hitPoints / maxHitPoints;
    if (t > 0.66f) return color;
    else if (t > 0.33f) {
        return Color{color.r, (unsigned char)(color.g * 0.6), (unsigned char)(color.b * 0.6), 255};
    } else {
        return Color{255, (unsigned char)(color.g * 0.3), (unsigned char)(color.b * 0.3), 255};
    }
}

void Brick::Draw() const {
    if (!active) return;
    Color drawColor = GetColor();
    DrawRectangleRec(rect, drawColor);
    DrawRectangleLinesEx(rect, 1, BLACK);
    if (maxHitPoints > 1) {
        char text[4];
        sprintf(text, "%d", hitPoints);
        DrawText(text, rect.x + rect.width/2 - 5, rect.y + rect.height/2 - 8, 15, WHITE);
    }
}