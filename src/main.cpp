#include "engine/window_context_handler.hpp"
#include "smoke/smoke_system.hpp"
#include "gui/vector_direction.hpp"
#include "gui/graph.hpp"
#include "engine/common/transition.hpp"
#include <iostream>
#include "drone/drone.hpp"
#include "drone/drone_renderer.hpp"
#include "drone/dna_loader.hpp"
#include "rocket/rocket.hpp"
#include <list>
#include "engine/sound/sound_manager.hpp"


bool checkAlive(const Drone& drone)
{
    return std::abs(drone.angle) < Math::PI;
}


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
    stream_config.setDuration(6.0f, 0.0f);
    stream_config.min_dist_ratio     = 0.5f;
    stream_config.target_scale       = 1.5f;
    stream_config.opacity_level      = 0.12f;
    stream_config.dissipation_vector = { 0.0f, -50.0f };
    stream_config.scale_variation    = 0.35f;
    // Gui parameters
    const float outline_width = 5.0f;
    const float radius        = 15.0f;
    const float length        = radius * 1.5f;
    const float rest_length   = 0.1f;
    const float r = 0.0f;
    trn::Transition<float> dir_radius(r, 3.0f);
    trn::Transition<float> dir_length(length, 3.0f);
    // Demo
    Smoke::Configuration demo_config;
    demo_config.setDuration(4.0f, 0.0f);
    demo_config.min_dist_ratio     = 1.0f;
    demo_config.target_scale       = 1.0f;
    demo_config.opacity_level      = 0.12f;
    demo_config.dissipation_vector = { 0.0f, 0.0f };
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

    sf::Font font;
    font.loadFromFile("res/font.ttf");
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(150);
    text.setString("Thanks for Watching!");
    text.setOrigin(text.getGlobalBounds().width * 0.5f, text.getGlobalBounds().height * 0.5f);
    text.setPosition(mid_screen.x, 400.0f);

    sf::Texture round_texture;
    round_texture.loadFromFile("res/round.png");
    sf::Sprite sprite(round_texture);
    sprite.setOrigin(300.0f, 300.0f);
    sprite.setPosition(mid_screen.x, 800.0f);
    sprite.setScale(0.4f, 0.4f);
    bool first_round = false;

    SoundManager::registerSound("res/explosion.wav");

    std::list<Rocket> rockets;
    sf::Clock rocket_clock;

    const float rocket_duration = 15.0f;
    float rocket_delay = 1.0f;

	while (app.run()) {
        const sf::Vector2f dir_1_position{ 100.0f, 100.0f };
        const sf::Vector2f dir_2_position{ 1820.0f, 980.0f};
        const float dir_1_angle = Math::PI * 0.25f + Math::PI * 0.125f * sin(clock.getElapsedTime().asSeconds());
        const float dir_2_angle = -Math::PI * 0.75f + Math::PI * 0.125f * sin(clock.getElapsedTime().asSeconds());
        // Compute mid screen to mouse vector
        const sf::Vector2f mouse_position = app.getWorldMousePosition();
        const Vec2 mid_to_mouse = { mouse_position.x - mid_screen.x, mouse_position.y - mid_screen.y };
        const float mid_to_mouse_angle = MathVec2::angle(mid_to_mouse);
        // Create smoke if activated
        SoundManager::update();
        if (clock.getElapsedTime().asSeconds() > rocket_duration + 3.0f) {
            if (!first_round) {
                dir_radius = radius;
            }
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
            sf::Color color2 = sf::Color(255 * r2 * r2, 255 * g2 * g2, 255 * b2 * b2);
            const float direction_angle = dir_1_angle + RNGf::getRange(Math::PI * 0.1f);
            smoke_system.create({ dir_1_position.x, dir_1_position.y }, { cos(dir_1_angle), sin(dir_1_angle) }, 1200.0f, stream_config);
            smoke_system.particles.back().color = color;
            smoke_system.create({ dir_2_position.x, dir_2_position.y }, { cos(dir_2_angle), sin(dir_2_angle) }, 1200.0f, stream_config);
            smoke_system.particles.back().color = color2;
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
        // Update smoke
        smoke_system.update(dt);
        // Render the scene
        RenderContext& context = app.getRenderContext();
        context.clear(sf::Color::Black);
        context.draw(text);
        //drone_renderer.draw(drone, context);
        smoke_system.render(context);
        VectorDirection dir_o(dir_radius, dir_length);
        VectorDirection dir_i(dir_radius - std::min(dir_radius.get(), outline_width), dir_length - std::min(dir_length.get(), outline_width));
        dir_o.position = dir_1_position;
        dir_i.position = dir_1_position;
        dir_o.color = sf::Color(200, 200, 200);
        dir_i.color = sf::Color(255, 100, 0);
        dir_o.setAngle(dir_1_angle);
        dir_i.setAngle(dir_1_angle);
        context.draw(dir_o);
        context.draw(dir_i);
        dir_o.position = dir_2_position;
        dir_i.position = dir_2_position;
        dir_o.setAngle(dir_2_angle);
        dir_i.setAngle(dir_2_angle);
        context.draw(dir_o);
        context.draw(dir_i);
        context.display();
	}

	return 0;
}
