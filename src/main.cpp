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

    app.getEventManager().addKeyPressedCallback(sf::Keyboard::Space, [&](sfev::CstEv){
        const uint32_t smokes_count = 32;
        for (uint32_t i(smokes_count); i--;) {
            const float angle = RNGf::getUnder(2.0f * Math::PI);
            const float dist = RNGf::getUnder(300.0f);
            smoke_system.create(Vec2{window_width * 0.5f, window_height * 0.5f}, Vec2{cos(angle), sin(angle)}, dist, 1.0f);
        }
    });

    const float dt = 1.0f / 60.0f;

	while (app.run()) {
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
