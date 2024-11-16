#include "Action.h"
#include <ranges>

void Engine3::Action::Update(InternalInputType type, ProcessState state, InputValue value)
{
	auto iterator = BoundInputs.find(type);
	if (iterator == BoundInputs.end()) { return; }

	Input& input = iterator->second;
	input.CurrentState = state;
	input.Value = value;
}

template <typename... T>
auto Engine3::Implementation::Action<T...>::FilterInputs()
{
	auto enabled = [](const Input& input) { return input.CurrentState != ProcessState::Stop; };
	auto conditions = [](const Input& input)
	{
		return std::ranges::all_of(input.Conditions, [&input](const std::unique_ptr<Condition>& condition)
		{
			return (*condition)(input);
		});
	};

	// This is lazily evaluated and the modifiers alter their own state, so it must not be accessed until
	// each element is acted upon otherwise a condition could change.
	// E.G. PressedCondition will set its previous process state which if called multiple times will
	// result in it treating an input as a hold when it's actually only been pressed.
	return BoundInputs
		| std::views::values
		| std::views::filter(enabled)
		| std::views::filter(conditions);
}

template <>
void Engine3::Implementation::Action<>::Process()
{
	bool execute = false;
	for (auto& input : FilterInputs())
	{
		execute = true;
		if (input.CurrentState != ProcessState::Continuous) { input.CurrentState = ProcessState::Stop; }
	}

	if (execute) { BoundFunction(); }
}

template <>
void Engine3::Implementation::Action<float>::Process()
{
	auto toFloat = [](auto&& arg)
	{
		using T = std::decay_t<decltype(arg)>;
		if constexpr (std::is_same_v<T, std::monostate>) { return 1.f; }
		else if constexpr (std::is_same_v<T, float>) { return arg; }
		else if constexpr (std::is_same_v<T, Vector<2>>) { return arg.Length(); }
		else { static_assert(false, "non-exhaustive visitor!"); }
	};

	bool execute = false;
	float finalValue = 0;
	for (Input& input : FilterInputs())
	{
		execute = true;
		float value = std::visit(toFloat, input.Value);
		for (const auto& modifier : input.Modifiers) { (*modifier)(value); }

		if (CumulateInputs) { finalValue += value; }
		else { finalValue = std::abs(value) > std::abs(finalValue) ? value : finalValue; }

		if (input.CurrentState != ProcessState::Continuous) { input.CurrentState = ProcessState::Stop; }
	}

	if (execute) { BoundFunction(finalValue); }
}

template <>
void Engine3::Implementation::Action<Engine3::Vector<2>>::Process()
{
	auto toVector2 = [](auto&& arg)
	{
		using T = std::decay_t<decltype(arg)>;
		if constexpr (std::is_same_v<T, std::monostate>) { return Vector<2>::Right(); }
		else if constexpr (std::is_same_v<T, float>) { return Vector<2>::Right() * arg; }
		else if constexpr (std::is_same_v<T, Vector<2>>) { return arg; }
		else { static_assert(false, "non-exhaustive visitor!"); }
	};

	bool execute = false;
	Vector<2> finalValue = Vector<2>::Zero();
	for (Input& input : FilterInputs())
	{
		execute = true;
		Vector<2> value = std::visit(toVector2, input.Value);
		for (const auto& modifier : input.Modifiers) { (*modifier)(value); }

		if (CumulateInputs) { finalValue += value; }
		else
		{
			finalValue.X(std::abs(value.X()) > std::abs(finalValue.X()) ? value.X() : finalValue.X());
			finalValue.Y(std::abs(value.Y()) > std::abs(finalValue.Y()) ? value.Y() : finalValue.Y());
		}

		if (input.CurrentState != ProcessState::Continuous) { input.CurrentState = ProcessState::Stop; }
	}

	if (execute) { BoundFunction(finalValue); }
}
