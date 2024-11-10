#pragma once
#include "Condition.h"
#include "../Action.h"

namespace Engine3
{
	class PressedCondition : public Condition
	{
	private:
		ProcessState PreviousProcessState = ProcessState::Stop;

	public:
		bool operator()(const Input& input) override
		{
			bool isHeld = input.GetCurrentState() == PreviousProcessState;
			bool isPressed = !isHeld && input.GetCurrentState() != ProcessState::Release;
			PreviousProcessState = input.GetCurrentState();

			return isPressed;
		}
	};
}
