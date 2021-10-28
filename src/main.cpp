#include "engine/window_context_handler.hpp"
#include "smoke/smoke_system.hpp"
#include "gui/vector_direction.hpp"
#include "engine/common/transition.hpp"


int32_t main()
{
    // Create application context
	const int32_t window_width = 1920;
	const int32_t window_height = 1080;
    const sf::Vector2f mid_screen{ window_width * 0.5f, window_height * 0.5f };
	WindowContextHandler app("VPE", {window_width, window_height}, sf::Style::Fullscreen, true);
    // Load resources
    Smoke::init();
    SmokeSystem smoke_system;
    // Create two different smoke configurations
    // For explosion
    Smoke::Configuration explosion_config;
    explosion_config.setDuration(1.0f, 0.2f);
    explosion_config.min_dist_ratio = 0.25f;
    explosion_config.target_scale   = 0.9f;
    explosion_config.opacity_level  = 0.25f;
    // For the stream
    Smoke::Configuration stream_config;
    stream_config.setDuration(4.0f, 0.0f);
    stream_config.min_dist_ratio     = 0.5f;
    stream_config.target_scale       = 1.0f;
    stream_config.opacity_level      = 0.12f;
    stream_config.dissipation_vector = { 0.0f, -100.0f };
    // Gui parameters
    const float outline_width = 5.0f;
    const float radius = 15.0f;
    const float length = radius * 1.5f;
    const float rest_length = 0.1f;
    trn::Transition<float> dir_length(rest_length, 3.0f);
    // Create the events
    app.getEventManager().addKeyPressedCallback(sf::Keyboard::Space, [&](sfev::CstEv){
        const uint32_t smokes_count = 50;
        for (uint32_t i(smokes_count); i--;) {
            const float direction_angle = RNGf::getUnder(2.0f * Math::PI);
            smoke_system.create(Vec2{window_width * 0.5f, window_height * 0.5f}, Vec2{cos(direction_angle), sin(direction_angle)}, 300.0f, explosion_config);
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

	while (app.run()) {
        VectorDirection dir_o(radius, dir_length);
        VectorDirection dir_i(radius - outline_width, dir_length - outline_width);
        dir_o.position = mid_screen;
        dir_i.position = mid_screen;
        dir_i.color = sf::Color(255, 150, 0.0f);

        // Compute mid screen to mouse vector
        const sf::Vector2f mouse_position = app.getWorldMousePosition();
        const Vec2 mid_to_mouse = { mouse_position.x - mid_screen.x, mouse_position.y - mid_screen.y };
        const float mid_to_mouse_angle = MathVec2::angle(mid_to_mouse);
        dir_o.setAngle(mid_to_mouse_angle);
        dir_i.setAngle(mid_to_mouse_angle);

        // Create smoke if activated
        sf::RectangleShape stream_vector;
        if (smoke_activated) {
            const float direction_angle = mid_to_mouse_angle + RNGf::getRange(Math::PI * 0.1f);
            smoke_system.create({ window_width * 0.5f, window_height * 0.5f }, {cos(direction_angle), sin(direction_angle)}, 850.0f, stream_config);
        }        

        // Update smoke
        smoke_system.update(dt);
        // Render the scene
        RenderContext& context = app.getRenderContext();
        context.clear();
        smoke_system.render(context);
        context.draw(dir_o);
        context.draw(dir_i);
        context.display();
	}

	return 0;
}
