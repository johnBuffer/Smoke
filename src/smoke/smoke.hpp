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
    static sf::Texture texture;

    static void init()
    {
        texture.loadFromFile("res/smoke.png");
    }

    Vec2  position;
    Vec2  direction;
    float target_dist  = 0.0f;
    float angle        = 0.0f;
    float target_angle = 0.0f;
    float duration     = 0.0f;
    float current_time = 0.0f;
    float ratio        = 0.0f;
    float scale        = 0.2f;
    float target_scale = 0.5f;
    sf::Sprite sprite;

    Smoke() = default;

    Smoke(Vec2 pos, Vec2 dir, float dist, float lifetime)
        : position(pos)
        , direction(dir)
        , target_dist(dist)
        , angle(RNGf::getUnder(2.0f * Math::PI))
        , target_angle(RNGf::getFullRange(1.5f))
        , target_scale(RNGf::getUnder(1.0f))
        , duration(1.25f * lifetime)
        , current_time(0.25f * lifetime)
        , sprite(texture)
    {
        const sf::Vector2u texture_size = texture.getSize();
        sprite.setOrigin(to<float>(texture_size.x / 2), to<float>(texture_size.y / 2));
    }

    void update(float dt)
    {
        // Update time
        current_time   += dt;
        ratio           = current_time / duration;
    }

    bool done() const
    {
        return ratio > 1.0f;
    }

    void render(RenderContext& context)
    {
        const float t_scale = Smooth::smoothStop(ratio, 10);
        const float t_dist = Smooth::smoothStop(ratio, 5);
        const float t_angle = Smooth::smoothStop(ratio, 1);
        const float current_scale = scale + target_scale * t_scale;
        const float current_angle = angle + (target_angle * t_angle);
        const Vec2  current_pos   = position + direction * (target_dist * t_dist);
        sprite.setPosition({ current_pos.x, current_pos.y});
        sprite.setScale(current_scale, current_scale);
        sprite.setRotation(Math::radToDeg(current_angle));
        sprite.setColor(sf::Color(200, 200, 200, to<uint8_t>(220 * (1.0f - ratio))));
        context.draw(sprite);
    }
};
