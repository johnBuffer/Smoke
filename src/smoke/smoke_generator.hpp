#include "gui/vector_direction.hpp"
#include "engine/common/transition.hpp"
#include "engine/common/math.hpp"
#include "engine/window_context_handler.hpp"
#include "smoke_system.hpp"


struct SmokeGenerator
{
    trn::Transition<float> radius;
    trn::Transition<float> length;
    float                  outline = 3.0f;
    VectorDirection        outer;
    VectorDirection        inner;
    Smoke::Configuration   configuration;
    sf::Color              color;
    bool                   enabled = false;

    SmokeGenerator() = default;

    SmokeGenerator(sf::Vector2f position, float radius_, float length_)
        : radius(radius_)
        , length(length_)
        , outer(0.0f, 0.0f)
        , inner(0.0f, 0.0f)
    {
        configuration.setDuration(8.0f, 0.0f);
        configuration.min_dist_ratio     = 0.5f;
        configuration.target_scale       = 1.8f;
        configuration.opacity_level      = 0.1f;
        configuration.dissipation_vector = { 0.0f, -30.0f };
        configuration.scale_variation    = 0.5f;

        outer.color = sf::Color::White;
        setPosition(position);
    }

    void update(float dt, SmokeSystem& smoke_system)
    {
        setLength(length);
        setRadius(radius);

        if (enabled) {
            const float direction_angle = MathVec2::angle(outer.direction) + RNGf::getRange(Math::PI * 0.05f);
            smoke_system.create({outer.position.x, outer.position.y}, {cos(direction_angle), sin(direction_angle)},
                                1200.0f, configuration, color);
        }
    }

    void setRadius(float r)
    {
        outer.radius = r;
        inner.radius = r - std::min(radius.get(), outline);
    }

    void setLength(float l)
    {
        outer.length = l;
        inner.length = l - std::min(length.get(), outline);
    }

    void setDirection(sf::Vector2f direction)
    {
        outer.direction = direction;
        inner.direction = direction;
    }

    void setPosition(sf::Vector2f position)
    {
        outer.position = position;
        inner.position = position;
    }

    void setColor(sf::Color c)
    {
        color = c;
        inner.color = color;
    }

    void enable()
    {
        enabled = true;
    }

    void disable()
    {
        enabled = false;
    }

    void render(RenderContext& context) const
    {
        context.draw(outer);
        context.draw(inner);
    }
};
