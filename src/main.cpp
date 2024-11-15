#include "Core/Engine.h"
#include "Core/Events.h"
#include "Core/Renderer.h"
#include "Core/Window.h"
#include "Input/InputManager.h"

int main(int argc, char* argv[])
{
	using namespace Engine3;

	Engine engine;
	if (!engine) { return EXIT_FAILURE; }

	Window window{u8"Engine3", 1280, 720, Window::OpenGL | Window::Resizable};
	if (!window) { return EXIT_FAILURE; }

	Renderer renderer(window);
	if (!renderer) { return EXIT_FAILURE; }

	InputManager inputManager;

	Events events;
	while (events.Process(window, renderer, inputManager))
	{
		engine.Update();
		renderer.Render();
	}

	return EXIT_SUCCESS;
}
