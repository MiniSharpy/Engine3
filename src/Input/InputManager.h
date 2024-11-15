#pragma once
#include "Action.h"
#include "../Maths/Vector.h"
#include <functional>
#include <memory>
#include <vector>

namespace Engine3
{
	template <typename... T>
	concept IsFloat = (std::same_as<T, float> || ...);

	template <typename... T>
	concept IsVector2 = (std::same_as<T, Vector<2>> || ...);

	template <typename... T>
	concept IsValidType = (IsFloat<T...> || IsVector2<T...>);

	class InputManager
	{
		friend class Events;

	private:
		std::vector<std::unique_ptr<Action>> Actions;

		void Update(std::string_view name, ProcessState state, InputValue value) const
		{
			for (auto& action : Actions) { action->Update(name, state, value); }
		}

		void Process() const { for (const auto& action : Actions) { action->Process(); } }

	public:
		template <IsValidType ...T>
			requires (sizeof...(T) == 0 ||
				(sizeof...(T) == 1))
		Action& AddAction(std::function<void(T...)> function, bool cumulateInputs = false)
		{
			return *Actions.emplace_back(new Implementation::Action(std::move(function), cumulateInputs)).get();
		}
	};
}
