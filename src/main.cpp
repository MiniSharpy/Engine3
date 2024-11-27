#include "Core/Engine.h"
#include "Core/Events.h"
#include "Core/Renderer.h"
#include "Core/Window.h"
#include "Input/InputManager.h"
#include "Input/Conditions/PressedCondition.h"
#include "Input/Modifiers/DeadZoneModifier.h"
#include "Input/Modifiers/SwizzleModifier.h"

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

	std::function printVector2 = [](Vector<2> value) { std::print("X:{} Y:{}\n", value.X(), value.Y()); };
	Action& mousePos = inputManager.AddAction(printVector2);
	mousePos.AddInput(Input::Mouse::Left).AddCondition<PressedCondition>();

	Action& leftAxis = inputManager.AddAction(printVector2);
	leftAxis.AddInput(Input::GamepadAxis::LeftX).AddModifier<DeadZoneModifier>();
	leftAxis.AddInput(Input::GamepadAxis::LeftY).AddModifier<DeadZoneModifier>().AddModifier<SwizzleModifier>();

	Events events;
	while (events.Process(window, renderer, inputManager))
	{
		engine.Update();
		renderer.Render();
	}

	return EXIT_SUCCESS;
}
