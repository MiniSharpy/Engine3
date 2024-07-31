#pragma once
#include <SDL.h>

namespace Engine3
{
	class Renderer;

	class Window;

	class Input
	{
	public:
		bool Process(Window& window, Renderer& renderer);
	};
}
