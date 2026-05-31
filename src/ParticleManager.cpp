#include "ParticleManager.h"
#include <algorithm>

void ParticleManager::Update(float dt) {
    for (auto& p : particles) p.Update(dt);
    particles.erase(std::remove_if(particles.begin(), particles.end(),
        [](const Particle& p) { return !p.IsActive(); }), particles.end());
}

void ParticleManager::Draw() const {
    for (const auto& p : particles) p.Draw();
}

void ParticleManager::SpawnParticles(Vector2 pos, Color color, int count, ParticleType type) {
    for (int i = 0; i < count; ++i) {
        particles.emplace_back(pos, color, type);
    }
}

void ParticleManager::SpawnDirectionalParticle(Vector2 pos, Vector2 dir, ParticleType type, Color color) {
    Particle p(pos, color, type);
    float speed = 200 + rand() % 200;
    p.SetVelocity({ dir.x * speed, dir.y * speed });  // 使用 setter
    particles.push_back(p);
}