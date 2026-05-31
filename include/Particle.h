#pragma once
#include "raylib.h"

enum ParticleType {
    PARTICLE_DEFAULT,
    PARTICLE_SPARK,
    PARTICLE_SMOKE,
    PARTICLE_SPEED_LINE
};

class Particle {
private:
    Vector2 pos, vel;
    Color color;
    float size, life, maxLife;
    bool active;
    ParticleType type;
public:
    Particle(Vector2 p, Color c, ParticleType t = PARTICLE_DEFAULT);
    void Update(float dt);
    void Draw() const;
    bool IsActive() const { return active; }
    void SetVelocity(Vector2 v) { vel = v; }
};