#include "engine/window_context_handler.hpp"


int32_t main()
{
    // Create application context
	const int32_t window_width = 1920;
	const int32_t window_height = 1080;
	WindowContextHandler app("VPE", {window_width, window_height}, sf::Style::Fullscreen, true);
    

	while (app.run()) {        
        // Render the scene
        RenderContext& context = app.getRenderContext();
        context.clear();

        context.display();
	}

	return 0;
}
