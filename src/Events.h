#pragma once
#include <memory>
#include <SDL.h>
#include <vector>
#include <xstring>

namespace Engine3
{
	class Renderer;

	class Window;

	class InputManager;

	class Events
	{
	private:
		static std::string_view GetMouseButtonName(const SDL_Event& event);

		std::vector<std::unique_ptr<SDL_GameController, void(*)(SDL_GameController*)>> Controllers;

	public:
		bool Process(Window&, Renderer&, InputManager&);
	};
}
