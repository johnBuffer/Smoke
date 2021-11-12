#include "engine/window_context_handler.hpp"
#include "smoke/smoke_generator.hpp"
#include "gui/vector_direction.hpp"
#include "engine/common/transition.hpp"
#include "rocket/rocket.hpp"
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
    stream_config.target_scale       = 1.5f;
    stream_config.opacity_level      = 0.1f;
    stream_config.dissipation_vector = { 0.0f, -30.0f };
    stream_config.scale_variation    = 0.8f;
    // Create the events
    bool smoke_activated = false;
    std::list<Rocket> rockets;

    app.getEventManager().addMousePressedCallback(sf::Mouse::Left, [&](sfev::CstEv) {
        rockets.emplace_back(app.getWorldMousePosition(), 0.0f, 0.0f);
    });

    const float dt = 1.0f / 60.0f;
    sf::Clock clock;

    const sf::Vector2f dir_1_position{ 100.0f, 100.0f };
    const sf::Vector2f dir_2_position{ 1820.0f, 980.0f };
    SmokeGenerator generator_1(dir_1_position, 0.0f, 0.0f);
    SmokeGenerator generator_2(dir_2_position, 0.0f, 0.0f);
    generator_1.setConfiguration(stream_config);
    generator_2.setConfiguration(stream_config);

    app.getEventManager().addKeyPressedCallback(sf::Keyboard::Space, [&](sfev::CstEv) {
        smoke_activated = !smoke_activated;
        if (smoke_activated) {
            generator_1.radius = 10.0f;
            generator_2.radius = 10.0f;
            generator_1.length = 15.0f;
            generator_2.length = 15.0f;
            generator_1.enable();
            generator_2.enable();
        }
        else {
            generator_1.radius = 0.0f;
            generator_2.radius = 0.0f;
            generator_1.length = 0.0f;
            generator_2.length = 0.0f;
            generator_1.disable();
            generator_2.disable();
        }
    });

	while (app.run()) {
        // Update smoke generators
        const float dir_1_angle =  Math::PI * 0.25f + Math::PI * 0.125f * sin(clock.getElapsedTime().asSeconds());
        const float dir_2_angle = -Math::PI * 0.75f + Math::PI * 0.125f * sin(clock.getElapsedTime().asSeconds());
        generator_1.update(dt, smoke_system);
        generator_2.update(dt, smoke_system);
        const float t_1 = 0.2f * clock.getElapsedTime().asSeconds();
        const float t_2 = t_1 + Math::PI * 0.5f;
        generator_1.setColor(ColorUtils::getRainbow(t_1));
        generator_2.setColor(ColorUtils::getRainbow(t_2));
        generator_1.setDirection({ cos(dir_1_angle), sin(dir_1_angle) });
        generator_2.setDirection({ cos(dir_2_angle), sin(dir_2_angle) });
        // Update rockets
        for (Rocket& r : rockets) {
            r.update(dt, smoke_system);
        }
        rockets.remove_if([](Rocket& r) {return r.isDone(); });
        // Update smoke
        smoke_system.update(dt);
        // Render the scene
        RenderContext& context = app.getRenderContext();
        context.clear(sf::Color::Black);
        smoke_system.render(context);
        generator_1.render(context);
        generator_2.render(context);
        context.display();
	}

	return 0;
}
