#pragma once
#include "smoke.hpp"
#include <vector>
#include <algorithm>


struct SmokeSystem
{
    std::vector<Smoke> particles;

    void create(Vec2 pos, Vec2 vel, float dist, const Smoke::Configuration& configuration, sf::Color color = sf::Color::White)
    {
        auto& particle = particles.emplace_back(pos, vel, dist, configuration);
        particle.color = color;
    }

    void update(float dt)
    {
        // Update smoke particles
        for (Smoke& smoke : particles) {
            smoke.update(dt);
        }
        // Remove old ones
        removeOldParticles();
    }

    void removeOldParticles()
    {
        for (uint32_t i(0); i<particles.size();) {
            if (particles[i].done()) {
                std::swap(particles[i], particles.back());
                particles.pop_back();
            } else {
                ++i;
            }
        }
    }

    void render(RenderContext& context)
    {
        for (Smoke& smoke : particles) {
            smoke.render(context);
        }
    }
};
