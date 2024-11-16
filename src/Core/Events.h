#pragma once
#include <memory>
#include <vector>

union SDL_Event;

struct _SDL_GameController;

typedef struct _SDL_GameController SDL_GameController;

namespace Engine3
{
	class Renderer;

	class Window;

	class InputManager;

	class Events
	{
		std::vector<std::unique_ptr<SDL_GameController, void(*)(SDL_GameController*)>> Controllers;

	public:
		bool Process(Window&, Renderer&, InputManager&);
	};
}
