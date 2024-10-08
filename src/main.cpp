#include "Engine.h"
#include "Input.h"
#include "Renderer.h"
#include "Window.h"

int main(int argc, char* argv[])
{
	using namespace Engine3;

	Engine engine;
	if (!engine) { return -1; }

	Window window{u8"Engine3", 1280, 720, Window::OpenGL | Window::Resizable};
	if (!window) { return -1; }

	Renderer renderer(window);
	if (!renderer) { return -1; }

	Input input;
	while (input.Process(window, renderer))
	{
		engine.Update();
		renderer.Render();
	}

	return 0;
}
