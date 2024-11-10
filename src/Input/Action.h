#pragma once
#include "../Maths/Vector.h"
#include "Conditions/Condition.h"
#include "Modifiers/Modifier.h"
#include <functional>
#include <map>
#include <string_view>
#include <variant>

namespace Engine3::Implementation
{
	template <typename... T>
	class Action;
};

namespace Engine3
{
	using InputValue = std::variant<std::monostate, float, Vector<2>>;

	enum class ProcessState
	{
		Stop,
		Once,
		Continuous,
		Release
	};

	class Input
	{
		friend class Action;

		template <typename... T>
		friend class Implementation::Action;

	private:
		Input() = default;

		std::vector<std::unique_ptr<Modifier>> Modifiers;

		std::vector<std::unique_ptr<Condition>> Conditions;

		ProcessState CurrentState = ProcessState::Stop;

		InputValue Value;

	public:
		ProcessState GetCurrentState() const { return CurrentState; }

		InputValue GetValue() const { return Value; }

		template <typename T, typename... Args>
		Input& AddModifier(Args&&... args)
		{
			Modifiers.emplace_back(std::make_unique<T>(std::forward<T>(args)...));
			return *this;
		}

		template <typename T, typename... Args>
		Input& AddCondition(Args&&... args)
		{
			Conditions.emplace_back(std::make_unique<T>(std::forward<T>(args)...));
			return *this;
		}
	};

	class Action
	{
		friend class InputManager;

	protected:
		std::map<std::string_view, Input> BoundInputs;

		bool CumulateInputs;

		Action(bool cumulateInputs) : CumulateInputs(cumulateInputs) {}

		void Update(std::string_view name, ProcessState state, InputValue value)
		{
			auto iterator = BoundInputs.find(name);
			if (iterator == BoundInputs.end()) { return; }

			Input& input = iterator->second;
			input.CurrentState = state;
			input.Value = value;
		}

		virtual void Process() = 0;

	public:
		virtual ~Action() = default;

		Input& AddInput(std::string_view name) { return BoundInputs.emplace(name, Input{}).first->second; }
	};

	namespace Implementation
	{
		template <typename... T>
		class Action final : public Engine3::Action
		{
			friend class Engine3::InputManager;

		private:
			std::function<void(T...)> BoundFunction;

			auto FilterInputs();

		protected:
			// Specialised for each valid type.
			void Process() override;

			Action(std::function<void(T...)> function, bool cumulateInputs)
				: Engine3::Action(cumulateInputs), BoundFunction(function) {}
		};
	}
}
