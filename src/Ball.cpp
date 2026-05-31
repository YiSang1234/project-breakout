#include "Ball.h"
#include <cmath>

inline float Clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

Ball::Ball(Vector2 p, Vector2 v, float r, Color c)
    : pos(p), vel(v), radius(r), color(c), trailIndex(0) {
    for (int i = 0; i < TRAIL_LENGTH; ++i) trailPositions[i] = pos;
}

void Ball::Reset(Vector2 np, Vector2 nv) {
    pos = np; vel = nv;
    for (int i = 0; i < TRAIL_LENGTH; ++i) trailPositions[i] = pos;
    trailIndex = 0;
}

void Ball::Move(float m) {
    pos.x += vel.x * m;
    pos.y += vel.y * m;
    UpdateTrail();
}

void Ball::UpdateTrail() {
    trailIndex = (trailIndex + 1) % TRAIL_LENGTH;
    trailPositions[trailIndex] = pos;
}

void Ball::BounceEdge(int w, int h) {
    if (pos.x - radius <= 0 || pos.x + radius >= w) {
        vel.x *= -1;
        if (pos.x < radius) pos.x = radius;
        if (pos.x > w - radius) pos.x = w - radius;
    }
    if (pos.y - radius <= 0) {
        vel.y *= -1;
        pos.y = radius;
    }
}

bool Ball::CheckPaddleCollision(Rectangle r) {
    if (!CheckCollisionCircleRec(pos, radius, r) || vel.y < 0) return false;
    vel.y = -fabs(vel.y);
    float hit = (pos.x - (r.x + r.width/2)) / (r.width/2);
    vel.x = hit * 5.0f;
    pos.y = r.y - radius - 1;
    UpdateTrail();
    return true;
}

bool Ball::CheckBrickCollision(Rectangle r) {
    if (!CheckCollisionCircleRec(pos, radius, r)) return false;
    float closestX = Clamp(pos.x, r.x, r.x + r.width);
    float closestY = Clamp(pos.y, r.y, r.y + r.height);
    float dx = pos.x - closestX;
    float dy = pos.y - closestY;
    float distSq = dx*dx + dy*dy;
    if (distSq < 1e-6f) {
        if (fabsf(vel.x) > fabsf(vel.y)) vel.x = -vel.x;
        else vel.y = -vel.y;
        return true;
    }
    float dist = sqrtf(distSq);
    float overlap = radius - dist;
    float nx = dx / dist;
    float ny = dy / dist;
    pos.x += nx * overlap;
    pos.y += ny * overlap;
    UpdateTrail();
    float dot = vel.x * nx + vel.y * ny;
    if (dot < 0) {
        vel.x -= 2 * dot * nx;
        vel.y -= 2 * dot * ny;
    }
    return true;
}

bool Ball::CheckBottomDeath(int h) { return pos.y + radius >= h; }

void Ball::Draw() const { DrawCircleV(pos, radius, color); }
void Ball::DrawAsClone() const { DrawCircleV(pos, radius, SKYBLUE); }
void Ball::DrawWithTrail() const {
    for (int i = 0; i < TRAIL_LENGTH; ++i) {
        int idx = (trailIndex - i + TRAIL_LENGTH) % TRAIL_LENGTH;
        float t = 1.0f - (float)i / TRAIL_LENGTH;
        Color trailColor = color;
        trailColor.a = (unsigned char)(t * 180);
        float trailRadius = radius * (0.4f + t * 0.6f);
        DrawCircleV(trailPositions[idx], trailRadius, trailColor);
    }
    DrawCircleV(pos, radius, color);
}