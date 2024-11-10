#include "Engine.h"
#include "Events.h"
#include "functional"
#include "Renderer.h"
#include "Window.h"
#include "Input/InputManager.h"
#include "Input/Conditions/PressedCondition.h"
#include "Input/Conditions/ReleasedCondition.h"
#include "Input/Modifiers/DeadZoneModifier.h"
#include "Input/Modifiers/SwizzleModifier.h"

int main(int argc, char* argv[])
{
	using namespace Engine3;

	Engine engine;
	if (!engine) { return -1; }

	Window window{u8"Engine3", 1280, 720, Window::OpenGL | Window::Resizable};
	if (!window) { return -1; }

	Renderer renderer(window);
	if (!renderer) { return -1; }

	InputManager inputManager;

	std::function printVector2 = [](Vector<2> value) { std::print("X: {}, Y: {}\n", value.X(), value.Y()); };

	Action& pressAction = inputManager.AddAction(printVector2);
	pressAction.AddInput("Mouse Button Left").AddCondition<PressedCondition>();

	Action& releaseAction = inputManager.AddAction(printVector2);
	releaseAction.AddInput("Mouse Button Left").AddCondition<ReleasedCondition>();

	Action& controllerAxis = inputManager.AddAction(printVector2);
	controllerAxis.AddInput(SDL_GameControllerGetStringForAxis(SDL_CONTROLLER_AXIS_LEFTX))
	              .AddModifier<DeadZoneModifier>();
	controllerAxis.AddInput(SDL_GameControllerGetStringForAxis(SDL_CONTROLLER_AXIS_LEFTY))
	              .AddModifier<SwizzleModifier>()
	              .AddModifier<DeadZoneModifier>();

	Events events;
	while (events.Process(window, renderer, inputManager))
	{
		engine.Update();
		//renderer.Render();
	}

	return 0;
}
