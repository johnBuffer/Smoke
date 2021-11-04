#pragma once

#include "smoke/smoke_system.hpp"
#include "engine/sound/sound_manager.hpp"


struct Rocket
{
	sf::Vector2f position;
	sf::Vector2f direction;
	Smoke::Configuration configuration;
	float speed;
	float angle;
    float duration  = RNGf::getRange(1.5f, 3.0f);
	float deviation = RNGf::getRange(0.5f);
	float current_time = 0.0f;

	Rocket(sf::Vector2f pos, float angle_, float speed_)
		: position(pos)
		, speed(speed_)
		, angle(angle_)
	{
		configuration.setDuration(2.0f, 0.0f);
		configuration.min_dist_ratio = 0.1f;
		configuration.opacity_level = 0.05f;
		configuration.target_scale = 0.35f;
		configuration.scale_variation = 0.7f;
	}

    [[nodiscard]]
	bool isDone() const
	{
		return current_time >= duration;
	}

	void update(float dt, SmokeSystem& smoke_system)
	{
        current_time += dt;
        angle += deviation * dt;
        direction = { cos(angle), sin(angle) };
        position += direction * (speed * dt);
        const float dist = 100.0f;
        {
            const float smoke_angle = angle + Math::PI + RNGf::getRange(Math::PI * 0.05f);
            smoke_system.create({ position.x, position.y }, { cos(smoke_angle), sin(smoke_angle) }, dist, configuration);
        }
        {
            const float smoke_angle = angle + Math::PI + RNGf::getRange(Math::PI * 0.25f);
            smoke_system.create({ position.x, position.y }, { cos(smoke_angle), sin(smoke_angle) }, dist, configuration);
        }

		if (isDone()) {
			SoundManager::playInstanceOf(0);

			const float t = RNGf::getUnder(Math::PI);
			float r = sin(t);
			float g = sin(t + 0.33f * 2.0f * Math::PI);
			float b = sin(t + 0.66f * 2.0f * Math::PI);

			{
				Smoke::Configuration explosion_small_config;
				explosion_small_config.setDuration(1.0f, 0.1f);
				explosion_small_config.min_dist_ratio = 0.2f;
				explosion_small_config.target_scale = 0.015f;
				explosion_small_config.opacity_level = 0.5f;
				explosion_small_config.scale_variation = 0.2f;

				const uint32_t smokes_count = 512;
				for (uint32_t i(smokes_count); i--;) {
					const float direction_angle = -RNGf::getUnder(2.0f * Math::PI);
					Vec2 dir = { cos(direction_angle), sin(direction_angle) };
					smoke_system.create({ position.x, position.y }, dir, 400.0f, explosion_small_config);
					smoke_system.particles.back().color = sf::Color(255 * r * r, 255 * g * g, 255 * b * b);
				}
			}

			Smoke::Configuration explosion_config;
			explosion_config.setDuration(2.0f, 0.35f);
			explosion_config.min_dist_ratio = 0.4f;
			explosion_config.target_scale = 1.0f;
			explosion_config.opacity_level = 0.25f;
			explosion_config.dissipation_vector = { 0.0f, -10.0f };
			explosion_config.scale_variation = 0.2f;

			const uint32_t smokes_count = 60;
			for (uint32_t i(smokes_count); i--;) {
				const float direction_angle = -RNGf::getUnder(2.0f * Math::PI);
				Vec2 dir = { cos(direction_angle), sin(direction_angle) };
				explosion_config.dissipation_vector = -dir * 20.0f;
				smoke_system.create({ position.x, position.y }, dir, 250.0f, explosion_config);
				smoke_system.particles.back().color = sf::Color(255 * r * r, 255 * g * g, 255 * b * b);
			}

			{
				Smoke::Configuration explosion_small_config;
				explosion_small_config.setDuration(1.0f, 0.25f);
				explosion_small_config.min_dist_ratio = 0.8f;
				explosion_small_config.target_scale = 0.5f;
				explosion_small_config.opacity_level = 0.25f;
				explosion_small_config.scale_variation = 0.4f;

				const uint32_t smokes_count = 100;
				for (uint32_t i(smokes_count); i--;) {
					const float direction_angle = -RNGf::getUnder(2.0f * Math::PI);
					Vec2 dir = { cos(direction_angle), sin(direction_angle) };
					explosion_small_config.dissipation_vector = dir * 20.0f;
					smoke_system.create({ position.x, position.y }, { cos(direction_angle), sin(direction_angle) }, 180.0f, explosion_small_config);
					smoke_system.particles.back().color = sf::Color(255 * r * r, 255 * g * g, 255 * b * b);
				}
			}
		}

	}
};
