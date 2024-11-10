#pragma once

namespace Engine3
{
	class Input;

	class Condition
	{
	public:
		virtual ~Condition() = default;
		virtual bool operator()(const Input &input) = 0;
	};
}