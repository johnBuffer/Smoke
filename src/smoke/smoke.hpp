#pragma once
#include "engine/common/vec.hpp"
#include "engine/common/utils.hpp"
#include "engine/common/smooth.hpp"
#include "engine/common/math.hpp"
#include "engine/common/number_generator.hpp"
#include "engine/window_context_handler.hpp"
#include <SFML/Graphics.hpp>


struct Smoke
{
    struct Configuration
    {
        float duration             = 1.0f;
        float skip_duration_offset = 0.0f;
        float starting_scale       = 0.1f;
        float target_scale         = 1.0f;
        float min_dist_ratio       = 0.0f;
        float ratio_skip           = 0.0f;
        float opacity_level        = 1.0f;
        Vec2  dissipation_vector   = {};

        void setDuration(float lifetime, float skip = 0.0f)
        {
            skip_duration_offset = lifetime * skip;
            duration             = lifetime * (1.0f + skip);
            ratio_skip           = skip;
        }

        [[nodiscard]]
        float getTargetDist(float max_dist) const
        {
            return RNGf::getRange(min_dist_ratio * max_dist, max_dist);
        }

        [[nodiscard]]
        float getCurrentScale(float t) const
        {
            return starting_scale + (target_scale - starting_scale) * t;
        }
    };

    static sf::Texture texture;

    static void init()
    {
        texture.loadFromFile("res/smoke.png");
    }

    Vec2          position;
    Vec2          direction;
    float         target_dist  = 0.0f;
    float         angle        = 0.0f;
    float         target_angle = 0.0f;
    float         current_time = 0.0f;
    float         ratio        = 0.0f;
    float         target_scale = 0.5f;
    Configuration configuration;
    sf::RectangleShape    sprite;

    Smoke() = default;

    Smoke(Vec2 pos, Vec2 dir, float max_dist, const Configuration& config)
        : position(pos)
        , direction(dir)
        , target_dist(config.getTargetDist(max_dist))
        , angle(0.0f * RNGf::getUnder(2.0f * Math::PI))
        , target_angle(0.0f * RNGf::getFullRange(1.5f))
        , current_time(config.skip_duration_offset)
        , configuration(config)
    {
        target_scale = 0.2f + 1.0f * target_dist / max_dist;
        const sf::Vector2u texture_size = texture.getSize();
        sprite.setSize({to<float>(texture_size.x), to<float>(texture_size.y)});
        sprite.setOrigin(to<float>(texture_size.x) * 0.5f, to<float>(texture_size.y) * 0.5f);
    }

    void update(float dt)
    {
        // Update time
        current_time   += dt;
        ratio           = current_time / configuration.duration;
    }

    bool done() const
    {
        return ratio > 1.0f;
    }

    void render(RenderContext& context)
    {
        const float t_scale       = Smooth::smoothStop(ratio, 10);
        const float t_dist        = Smooth::smoothStop(ratio, 5);
        const float t_angle       = Smooth::smoothStop(ratio, 1);
        const float current_scale = 0.1f; // configuration.getCurrentScale(t_scale);
        const float current_angle = 0.0f; // angle + (target_angle * t_angle);
        const Vec2  current_pos   = position + direction * (target_dist * t_dist) + configuration.dissipation_vector * (current_time - configuration.skip_duration_offset);
        sprite.setPosition({ current_pos.x, current_pos.y});
        sprite.setScale(current_scale, current_scale);
        sprite.setRotation(Math::radToDeg(current_angle));
        //sprite.setColor(sf::Color(200, 200, 200, to<uint8_t>(255.0f * configuration.opacity_level * (1.0f - ratio))));
        //sprite.setFillColor(sf::Color(200, 200, 200, to<uint8_t>(255.0f * configuration.opacity_level * (1.0f - ratio))));
        sprite.setFillColor(sf::Color(200, 200, 200, 255));
        context.draw(sprite);
    }
};
