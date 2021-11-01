#include "gui/vector_direction.hpp"
#include "engine/common/transition.hpp"
#include "engine/common/math.hpp"
#include "engine/window_context_handler.hpp"
#include "smoke_system.hpp"


struct SmokeGenerator
{
    VectorDirection        outer;
    VectorDirection        inner;
    trn::Transition<float> radius;
    trn::Transition<float> length;
    float                  radius_delta;
    float                  length_delta;
    Smoke::Configuration   configuration;

    SmokeGenerator()
    {
        configuration.setDuration(12.0f, 0.0f);
        configuration.min_dist_ratio     = 0.5f;
        configuration.target_scale       = 2.5f;
        configuration.opacity_level      = 0.1f;
        configuration.dissipation_vector = { 0.0f, -100.0f };
        configuration.scale_variation    = 0.8f;
    }

    void update(float dt, SmokeSystem& smoke_system)
    {
        outer.radius = radius;
        inner.radius = radius - radius_delta;
        outer.length = length;
        inner.length = length - length_delta;

        const float direction_angle = MathVec2::angle(outer.direction) + RNGf::getRange(Math::PI * 0.1f);
        smoke_system.create({ outer.position.x, outer.position.y }, { cos(direction_angle), sin(direction_angle) }, 250.0f, configuration);
    }

    void setPosition(sf::Vector2f position)
    {
        outer.position = position;
        inner.position = position;
    }

    void render(RenderContext& context)
    {
        context.draw(outer);
        context.draw(inner);
    }
};
