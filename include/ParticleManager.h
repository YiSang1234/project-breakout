#pragma once
#include "Particle.h"
#include <vector>

class ParticleManager {
private:
    std::vector<Particle> particles;
public:
    void Update(float dt);
    void Draw() const;
    void SpawnParticles(Vector2 pos, Color color, int count, ParticleType type = PARTICLE_DEFAULT);
    void SpawnDirectionalParticle(Vector2 pos, Vector2 dir, ParticleType type = PARTICLE_DEFAULT, Color color = WHITE);
    void Clear() { particles.clear(); }
};