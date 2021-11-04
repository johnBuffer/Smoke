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
    sf::Clock rocket_clock;

    bool first_round = false;
    float rocket_delay = 1.0f;
    float rocket_duration = 20.0f;

    const sf::Vector2f dir_1_position{ 100.0f, 100.0f };
    const sf::Vector2f dir_2_position{ 1820.0f, 980.0f };
    SmokeGenerator generator_1(dir_1_position, 0.0f, 0.0f);
    SmokeGenerator generator_2(dir_2_position, 0.0f, 0.0f);

    sf::Font font;
    font.loadFromFile("res/font.ttf");
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(150);
    text.setString("Thanks for Watching!");
    text.setOrigin(text.getGlobalBounds().width * 0.5f, text.getGlobalBounds().height * 0.5f);
    text.setPosition(mid_screen.x, 400.0f);

    SoundManager::registerSound("res/explosion.wav");

	while (app.run()) {
        // Compute mid screen to mouse vector
        const float dir_1_angle = Math::PI * 0.25f + Math::PI * 0.125f * sin(clock.getElapsedTime().asSeconds());
        const float dir_2_angle = -Math::PI * 0.75f + Math::PI * 0.125f * sin(clock.getElapsedTime().asSeconds());

        SoundManager::update();
        if (clock.getElapsedTime().asSeconds() > rocket_duration + 3.0f) {
            if (!first_round) {
                generator_1.radius = 10.0f;
                generator_2.radius = 10.0f;
                generator_1.length = 15.0f;
                generator_2.length = 15.0f;
                generator_1.enable();
                generator_2.enable();
            }
            generator_1.update(dt, smoke_system);
            generator_2.update(dt, smoke_system);
            first_round = true;
            const float t = 0.2f * clock.getElapsedTime().asSeconds();
            float r = sin(t);
            float g = sin(t + 0.33f * 2.0f * Math::PI);
            float b = sin(t + 0.66f * 2.0f * Math::PI);

            const float t2 = t + Math::PI * 0.5f;
            float r2 = sin(t2);
            float g2 = sin(t2 + 0.33f * 2.0f * Math::PI);
            float b2 = sin(t2 + 0.66f * 2.0f * Math::PI);

            sf::Color color = sf::Color(255 * r * r, 255 * g * g, 255 * b * b);
            generator_1.setColor(color);
            sf::Color color2 = sf::Color(255 * r2 * r2, 255 * g2 * g2, 255 * b2 * b2);
            generator_2.setColor(color2);
            generator_1.setDirection({ cos(dir_1_angle), sin(dir_1_angle) });
            generator_2.setDirection({ cos(dir_2_angle), sin(dir_2_angle) });
        }
        // Update rockets
        for (Rocket& r : rockets) {
            r.update(dt, smoke_system);
        }
        rockets.remove_if([](Rocket& r) {return r.isDone(); });
        if (clock.getElapsedTime().asSeconds() > 5.0f && rocket_clock.getElapsedTime().asSeconds() > rocket_delay && clock.getElapsedTime().asSeconds() < rocket_duration) {
            rocket_clock.restart();
            rockets.push_back({ { RNGf::getUnder(1920.0f), 1080.f }, -Math::PI * 0.5f, 300.0f });
            rocket_delay *= 0.97f;
        }

        rockets.remove_if([](const Rocket& r) { return r.isDone(); });

        // Update smoke
        smoke_system.update(dt);
        // Render the scene
        RenderContext& context = app.getRenderContext();
        context.clear(sf::Color::Black);
        context.draw(text);
        smoke_system.render(context);
        generator_1.render(context);
        generator_2.render(context);
        context.display();
	}

	return 0;
}
