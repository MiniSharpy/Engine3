#include "Input.h"
#include "Renderer.h"
#include "Window.h"

bool Engine3::Input::Process(Window& window, Renderer& renderer)
{
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type) // SDL_EventType
	{
	case SDL_QUIT: { return false; }
	case SDL_WINDOWEVENT:
		{
			switch (const SDL_WindowEvent& windowEvent = event.window; windowEvent.event)
			{
			// This should always trigger when size is changed, even if done through the SDL API
			// unlike with SDL_WINDOWEVENT_RESIZED.
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				{
					renderer.SetSize(windowEvent.data1, windowEvent.data2);
					break;
				}
			default: break;
			}
			break;
		}
	default: break;
	}

	return true;
}
