#include "engine/window_context_handler.hpp"
#include "smoke/smoke_system.hpp"
#include "gui/vector_direction.hpp"
#include "engine/common/transition.hpp"
#include <iostream>
#include <list>


int32_t main()
{
    // Create application context
	const int32_t window_width = 1920;
	const int32_t window_height = 1080;
    const sf::Vector2f mid_screen{ window_width * 0.5f, window_height * 0.5f };
	WindowContextHandler app("Smoke", {window_width, window_height}, sf::Style::Fullscreen, true);
    // Load resources
    Smoke::init();
    SmokeSystem smoke_system;
    // Create two different smoke configurations
    // For explosion
    Smoke::Configuration explosion_config;
    explosion_config.setDuration(4.0f, 0.25f);
    explosion_config.min_dist_ratio = 0.4f;
    explosion_config.target_scale   = 0.8f;
    explosion_config.opacity_level  = 0.125f;
    explosion_config.dissipation_vector = { 0.0f, -10.0f };
    // For the stream
    Smoke::Configuration stream_config;
    stream_config.setDuration(12.0f, 0.0f);
    stream_config.min_dist_ratio     = 0.5f;
    stream_config.target_scale       = 2.5f;
    stream_config.opacity_level      = 0.1f;
    stream_config.dissipation_vector = { 0.0f, -100.0f };
    stream_config.scale_variation    = 0.8f;
    // Gui parameters
    const float outline_width = 5.0f;
    const float radius        = 15.0f;
    const float length        = radius * 1.5f;
    const float rest_length   = 0.1f;
    trn::Transition<float> dir_radius(radius, 3.0f);
    trn::Transition<float> dir_length(length, 3.0f);
    // Create the events
    app.getEventManager().addKeyPressedCallback(sf::Keyboard::Space, [&](sfev::CstEv){
        const uint32_t smokes_count = 32;
        for (uint32_t i(smokes_count); i--;) {
            const float direction_angle = -RNGf::getUnder(0.05f * Math::PI);
            smoke_system.create(Vec2{ window_width * 0.5f, window_height * 0.5f }, Vec2{ cos(direction_angle), sin(direction_angle) }, 200.0f, explosion_config);
            dir_radius.setValueInstant(1.5f * radius);
            dir_radius = radius;
        }
    });
    bool smoke_activated = false;
    app.getEventManager().addMousePressedCallback(sf::Mouse::Left, [&](sfev::CstEv) {
        smoke_activated = true;
        dir_length = length;
    });
    app.getEventManager().addMouseReleasedCallback(sf::Mouse::Left, [&](sfev::CstEv) {
        smoke_activated = false;
        dir_length = rest_length;
    });

    const float dt = 1.0f / 60.0f;
    sf::Clock clock;

	while (app.run()) {
        // Compute mid screen to mouse vector
        const sf::Vector2f mouse_position = app.getWorldMousePosition();
        const Vec2 mid_to_mouse = { mouse_position.x - mid_screen.x, mouse_position.y - mid_screen.y };
        const float mid_to_mouse_angle = MathVec2::angle(mid_to_mouse);
        
        if (clock.getElapsedTime().asSeconds() > 5.0f) {
            {
                const float direction_angle = -Math::PI * 0.5f + RNGf::getRange(Math::PI * 0.5f);
                smoke_system.create({ RNGf::getUnder(window_width), window_height * 1.2f }, { cos(direction_angle), sin(direction_angle) }, 250.0f, stream_config);
            }
        }
       
        // Update smoke
        smoke_system.update(dt);
        // Render the scene
        RenderContext& context = app.getRenderContext();
        context.clear(sf::Color::Black);
        smoke_system.render(context);
        VectorDirection dir_o(dir_radius, dir_length);
        VectorDirection dir_i(dir_radius - std::min(dir_radius.get(), outline_width), dir_length - std::min(dir_length.get(), outline_width));
        dir_o.position = { window_width * 0.25f, window_height * 0.5f };
        dir_i.position = { window_width * 0.25f, window_height * 0.5f };
        dir_o.color = sf::Color(200, 200, 200);
        dir_i.color = sf::Color(255, 100, 0);
        dir_o.setAngle(mid_to_mouse_angle);
        dir_i.setAngle(mid_to_mouse_angle);
        /*context.draw(dir_o);
        context.draw(dir_i);*/
        context.display();
	}

	return 0;
}
