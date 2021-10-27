#include "engine/window_context_handler.hpp"
#include "smoke/smoke_system.hpp"


int32_t main()
{
    // Create application context
	const int32_t window_width = 1920;
	const int32_t window_height = 1080;
	WindowContextHandler app("VPE", {window_width, window_height}, sf::Style::Fullscreen, true);
    // Load resources
    Smoke::init();
    SmokeSystem smoke_system;
    // Create two different smoke configurations
    // For explosion
    Smoke::Configuration explosion_config;
    explosion_config.setDuration(1.0f, 0.2f);
    explosion_config.min_dist_ratio = 0.4f;
    explosion_config.target_scale   = 1.0f;
    explosion_config.opacity_level  = 0.25f;
    // For the stream
    Smoke::Configuration stream_config;
    stream_config.setDuration(4.0f, 0.0f);
    stream_config.min_dist_ratio = 0.5f;
    stream_config.target_scale = 1.0f;
    stream_config.opacity_level = 0.1f;
    stream_config.dissipation_vector = { 0.0f, -100.0f };
    // Create the events
    app.getEventManager().addKeyPressedCallback(sf::Keyboard::Space, [&](sfev::CstEv){
        const uint32_t smokes_count = 40;
        for (uint32_t i(smokes_count); i--;) {
            const float direction_angle = RNGf::getUnder(2.0f * Math::PI);
            smoke_system.create(Vec2{window_width * 0.5f, window_height * 0.5f}, Vec2{cos(direction_angle), sin(direction_angle)}, 300.0f, explosion_config);
        }
    });    
    bool smoke_activated = false;
    app.getEventManager().addMousePressedCallback(sf::Mouse::Left, [&](sfev::CstEv) {
        smoke_activated = true;
    });
    app.getEventManager().addMouseReleasedCallback(sf::Mouse::Left, [&](sfev::CstEv) {
        smoke_activated = false;
    });

    const float dt = 1.0f / 60.0f;

	while (app.run()) {
        // Create smoke if activated
        if (smoke_activated) {
            const sf::Vector2f mouse_posiiton = app.getWorldMousePosition();
            const Vec2 mid_screen{ window_width * 0.5f, window_height * 0.5f };
            const Vec2 mid_to_mouse = Vec2{ mouse_posiiton.x, mouse_posiiton.y } - mid_screen;
            const float direction_angle = MathVec2::angle(mid_to_mouse) + RNGf::getRange(Math::PI * 0.1f);
            smoke_system.create({ window_width * 0.5f, window_height * 0.5f }, {cos(direction_angle), sin(direction_angle)}, 800.0f, stream_config);
        }
        // Update smoke
        smoke_system.update(dt);
        // Render the scene
        RenderContext& context = app.getRenderContext();
        context.clear();
        smoke_system.render(context);
        context.display();
	}

	return 0;
}
