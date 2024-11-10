#pragma once
#include "Condition.h"
#include "../Action.h"
namespace Engine3
{
	class ReleasedCondition : public Condition
	{
	public:
		bool operator()(const Input &input) override
		{
			return input.GetCurrentState() == ProcessState::Release;
		}
	};
}