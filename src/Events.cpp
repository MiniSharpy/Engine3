#include "Events.h"
#include "Renderer.h"
#include "Window.h"
#include "Input/InputManager.h"

std::string_view Engine3::Events::GetMouseButtonName(const SDL_Event& event)
{
	switch (event.button.button)
	{
	case SDL_BUTTON_LEFT:
		return "Mouse Button Left";
	case SDL_BUTTON_MIDDLE:
		return "Mouse Button Middle";
	case SDL_BUTTON_RIGHT:
		return "Mouse Button Right";
	case SDL_BUTTON_X1:
		return "Mouse Button Extra1";
	case SDL_BUTTON_X2:
		return "Mouse Button Extra2";
	default:
		std::unreachable();
	}
}

bool Engine3::Events::Process(Window& window, Renderer& renderer, InputManager& inputManager)
{
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type) // SDL_EventType
	{
	case SDL_QUIT:
		return false;
	case SDL_WINDOWEVENT:
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
	case SDL_KEYDOWN:
		inputManager.Update(SDL_GetScancodeName(event.key.keysym.scancode), ProcessState::Continuous, {});
		break;
	case SDL_KEYUP:
		inputManager.Update(SDL_GetScancodeName(event.key.keysym.scancode), ProcessState::Release, {});
		break;
	case SDL_MOUSEMOTION:
		inputManager.Update("Mouse Axis X", ProcessState::Once, static_cast<float>(event.motion.xrel));
		inputManager.Update("Mouse Axis X", ProcessState::Once, static_cast<float>(event.motion.yrel));
		break;
	case SDL_MOUSEBUTTONDOWN:
		inputManager.Update(GetMouseButtonName(event),
		                    ProcessState::Continuous,
		                    Vector<2>{static_cast<float>(event.button.x), static_cast<float>(event.button.y)});
		break;
	case SDL_MOUSEBUTTONUP:
		inputManager.Update(GetMouseButtonName(event),
		                    ProcessState::Release,
		                    Vector<2>{static_cast<float>(event.button.x), static_cast<float>(event.button.y)});
		break;
	case SDL_MOUSEWHEEL:
		inputManager.Update("Mouse Wheel Y", ProcessState::Once, event.wheel.preciseY);
		break;
	case SDL_CONTROLLERDEVICEADDED:
		if (SDL_GameController* controller = SDL_GameControllerOpen(event.cdevice.which))
		{
			Controllers.emplace_back(controller, SDL_GameControllerClose);
		}
		break;
	case SDL_CONTROLLERDEVICEREMOVED:
		std::erase_if(Controllers,
		              [&event](const auto& controller)
		              {
			              return event.cdevice.which == SDL_JoystickInstanceID(
				              SDL_GameControllerGetJoystick(controller.get()));
		              }
		);
		break;
	case SDL_CONTROLLERBUTTONDOWN:
		for (auto& controller : Controllers)
		{
			if (event.cbutton.which == SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controller.get())))
			{
				inputManager.Update(
					SDL_GameControllerGetStringForButton(static_cast<SDL_GameControllerButton>(event.cbutton.button)),
					ProcessState::Continuous,
					{});
			}
		}

		break;
	case SDL_CONTROLLERBUTTONUP:
		for (auto& controller : Controllers)
		{
			if (event.cbutton.which == SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controller.get())))
			{
				inputManager.Update(
					SDL_GameControllerGetStringForAxis(static_cast<SDL_GameControllerAxis>(event.caxis.axis)),
					ProcessState::Release,
					{});
			}
		}
		break;
	case SDL_CONTROLLERAXISMOTION:
		for (auto& controller : Controllers)
		{
			if (event.cbutton.which == SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controller.get())))
			{
				float value = event.caxis.value < 0
					              ? -static_cast<float>(event.caxis.value) / std::numeric_limits<Sint16>::min()
					              : static_cast<float>(event.caxis.value) / std::numeric_limits<Sint16>::max();
				inputManager.Update(
					SDL_GameControllerGetStringForAxis(static_cast<SDL_GameControllerAxis>(event.caxis.axis)),
					ProcessState::Continuous,
					value);
			}
		}
		break;
	default: break;
	}

	inputManager.Process();
	return true;
}
