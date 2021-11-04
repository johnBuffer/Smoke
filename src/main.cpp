#include "engine/window_context_handler.hpp"
#include "smoke/smoke_generator.hpp"
#include "gui/vector_direction.hpp"
#include "engine/common/transition.hpp"
#include "rocket/rocket.hpp"
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
    // For the stream
    Smoke::Configuration stream_config;
    stream_config.setDuration(12.0f, 0.0f);
    stream_config.min_dist_ratio     = 0.5f;
    stream_config.target_scale       = 2.5f;
    stream_config.opacity_level      = 0.1f;
    stream_config.dissipation_vector = { 0.0f, -100.0f };
    stream_config.scale_variation    = 0.8f;
    // Create the events
    bool smoke_activated = false;
    std::list<Rocket> rockets;

    app.getEventManager().addMousePressedCallback(sf::Mouse::Left, [&](sfev::CstEv) {
        rockets.emplace_back(app.getWorldMousePosition(), 0.0f, 0.0f);
    });
    app.getEventManager().addMouseReleasedCallback(sf::Mouse::Left, [&](sfev::CstEv) {
        smoke_activated = false;
    });
    const float dt = 1.0f / 60.0f;
    sf::Clock clock;


    SoundManager::registerSound("res/explosion.wav");

	while (app.run()) {
        // Compute mid screen to mouse vector
        const sf::Vector2f mouse_position = app.getWorldMousePosition();
        const Vec2 mid_to_mouse = { mouse_position.x - mid_screen.x, mouse_position.y - mid_screen.y };
        const float mid_to_mouse_angle = MathVec2::angle(mid_to_mouse);

        SoundManager::update();
        if (clock.getElapsedTime().asSeconds() > 1.5f) {
            clock.restart();
            
        }

        for (Rocket& r : rockets) {
            r.update(dt, smoke_system);
        }
        rockets.remove_if([](const Rocket& r) { return r.isDone(); });

        // Update smoke
        smoke_system.update(dt);
        // Render the scene
        RenderContext& context = app.getRenderContext();
        context.clear(sf::Color::Black);
        smoke_system.render(context);
        context.display();
	}

	return 0;
}
