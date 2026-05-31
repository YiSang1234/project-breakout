#include "Particle.h"
#include <cmath>
#include <cstdlib>

Particle::Particle(Vector2 p, Color c, ParticleType t)
    : pos(p), color(c), active(true), type(t) {
    float ang = (rand() % 360) * DEG2RAD;
    float sp = rand() % 100 + 50;
    vel.x = cosf(ang) * sp;
    vel.y = sinf(ang) * sp;
    switch (type) {
        case PARTICLE_SPARK:
            size = 2 + rand() % 4;
            life = maxLife = 0.5f + (rand() % 100) / 200.0f;
            color = YELLOW;
            vel.y -= 100;
            break;
        case PARTICLE_SMOKE:
            size = 4 + rand() % 6;
            life = maxLife = 0.8f;
            color = DARKGRAY;
            vel.x *= 0.5f;
            vel.y = -fabsf(vel.y) * 0.8f;
            break;
        case PARTICLE_SPEED_LINE:
            size = 2.0f;
            life = maxLife = 0.2f;
            color = WHITE;
            vel = { (rand() % 200 - 100) / 10.0f, (rand() % 200 - 100) / 10.0f };
            break;
        default:
            size = (rand() % 8 + 4) / 10.0f;
            life = maxLife = 1.0f;
            break;
    }
}

void Particle::Update(float dt) {
    if (!active) return;
    life -= dt;
    if (life <= 0) { active = false; return; }
    pos.x += vel.x * dt;
    pos.y += vel.y * dt;
    if (type != PARTICLE_SPARK && type != PARTICLE_SMOKE) vel.y += 100 * dt;
}

void Particle::Draw() const {
    if (!active) return;
    Color drawColor = color;
    drawColor.a = (unsigned char)(255 * (life / maxLife));
    switch (type) {
        case PARTICLE_SPARK: {
            // 计算终点：pos + vel * dt * 2
            float dt = GetFrameTime();
            Vector2 end = { pos.x + vel.x * dt * 2, pos.y + vel.y * dt * 2 };
            DrawLineV(pos, end, drawColor);
            break;
        }
        case PARTICLE_SMOKE:
            DrawCircleV(pos, size, drawColor);
            break;
        case PARTICLE_SPEED_LINE:
            DrawRectangle(pos.x - 1, pos.y - 1, 2, 2, drawColor);
            break;
        default:
            DrawCircleV(pos, size, drawColor);
            break;
    }
}