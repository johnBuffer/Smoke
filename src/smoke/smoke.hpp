#pragma once
#include "engine/common/vec.hpp"
#include "engine/common/utils.hpp"
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

    Vec2       position;
    Vec2       velocity;
    float      angle          = 0.0f;
    float      angle_velocity = 0.0f;
    float      scale          = 0.1f;
    float      ratio          = 0.0f;
    float      duration       = 0.0f;
    float      current_time   = 0.0f;
    sf::Sprite sprite;

    Smoke() = default;

    Smoke(Vec2 pos, Vec2 vel, float lifetime)
        : position(pos)
        , velocity(vel)
        , angle(RNGf::getUnder(2.0f * Math::PI))
        , duration(lifetime)
        , sprite(texture)
    {
        const sf::Vector2u texture_size = texture.getSize();
        sprite.setOrigin(to<float>(texture_size.x / 2), to<float>(texture_size.y / 2));
    }

    void update(float dt)
    {
        // Update physic properties
        position     += velocity * dt;
        current_time += dt;
        ratio         = current_time / duration;
        scale        *= 1.0f + 1.4f * dt;
        const float slow_down_factor = 10.0f;
        velocity.x   *= 1.0f - slow_down_factor * dt;
        velocity.y   *= 1.0f - slow_down_factor * dt;
    }

    bool done() const
    {
        return ratio > 1.0f;
    }

    void render(RenderContext& context)
    {
        sprite.setPosition({position.x, position.y});
        sprite.setScale(scale, scale);
        sprite.setRotation(Math::radToDeg(angle));
        sprite.setColor(sf::Color(200, 200, 200, to<uint8_t>(220 * (1.0f - ratio))));
        context.draw(sprite);
    }
};
