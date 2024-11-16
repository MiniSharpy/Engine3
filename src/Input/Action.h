#pragma once
#include "../Maths/Vector.h"
#include "Conditions/Condition.h"
#include "Modifiers/Modifier.h"
#include <functional>
#include <map>
#include <memory>
#include <SDL.h>
#include <variant>

namespace Engine3::Implementation
{
	template <typename... T>
	class Action;
};

namespace Engine3
{
	class InputManager;

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

	public:
		// Don't want public code to be dependent on SDL,
		enum class Key
		{
			A = SDL_SCANCODE_A,
			B = SDL_SCANCODE_B,
			C = SDL_SCANCODE_C,
			D = SDL_SCANCODE_D,
			E = SDL_SCANCODE_E,
			F = SDL_SCANCODE_F,
			G = SDL_SCANCODE_G,
			H = SDL_SCANCODE_H,
			I = SDL_SCANCODE_I,
			J = SDL_SCANCODE_J,
			K = SDL_SCANCODE_K,
			L = SDL_SCANCODE_L,
			M = SDL_SCANCODE_M,
			N = SDL_SCANCODE_N,
			O = SDL_SCANCODE_O,
			P = SDL_SCANCODE_P,
			Q = SDL_SCANCODE_Q,
			R = SDL_SCANCODE_R,
			S = SDL_SCANCODE_S,
			T = SDL_SCANCODE_T,
			U = SDL_SCANCODE_U,
			V = SDL_SCANCODE_V,
			W = SDL_SCANCODE_W,
			X = SDL_SCANCODE_X,
			Y = SDL_SCANCODE_Y,
			Z = SDL_SCANCODE_Z,

			One = SDL_SCANCODE_1,
			Two = SDL_SCANCODE_2,
			Three = SDL_SCANCODE_3,
			Four = SDL_SCANCODE_4,
			Five = SDL_SCANCODE_5,
			Six = SDL_SCANCODE_6,
			Seven = SDL_SCANCODE_7,
			Eight = SDL_SCANCODE_8,
			Nine = SDL_SCANCODE_9,
			Zero = SDL_SCANCODE_0,

			Return = SDL_SCANCODE_RETURN,
			Escape = SDL_SCANCODE_ESCAPE,
			Backspace = SDL_SCANCODE_BACKSPACE,
			Tab = SDL_SCANCODE_TAB,
			Space = SDL_SCANCODE_SPACE,

			Minus = SDL_SCANCODE_MINUS,
			Equals = SDL_SCANCODE_EQUALS,
			LeftBracket = SDL_SCANCODE_LEFTBRACKET,
			RightBracket = SDL_SCANCODE_RIGHTBRACKET,
			Backslash = SDL_SCANCODE_BACKSLASH,

			SemiColon = SDL_SCANCODE_SEMICOLON,
			Apostrophe = SDL_SCANCODE_APOSTROPHE,
			Grave = SDL_SCANCODE_GRAVE,

			Comma = SDL_SCANCODE_COMMA,
			Period = SDL_SCANCODE_PERIOD,
			Slash = SDL_SCANCODE_SLASH,

			CapsLock = SDL_SCANCODE_CAPSLOCK,

			F1 = SDL_SCANCODE_F1,
			F2 = SDL_SCANCODE_F2,
			F3 = SDL_SCANCODE_F3,
			F4 = SDL_SCANCODE_F4,
			F5 = SDL_SCANCODE_F5,
			F6 = SDL_SCANCODE_F6,
			F7 = SDL_SCANCODE_F7,
			F8 = SDL_SCANCODE_F8,
			F9 = SDL_SCANCODE_F9,
			F10 = SDL_SCANCODE_F10,
			F11 = SDL_SCANCODE_F11,
			F12 = SDL_SCANCODE_F12,

			PrintScreen = SDL_SCANCODE_PRINTSCREEN,
			ScrollLock = SDL_SCANCODE_SCROLLLOCK,
			Pause = SDL_SCANCODE_PAUSE,
			Insert = SDL_SCANCODE_INSERT,
			Home = SDL_SCANCODE_HOME,
			PageUp = SDL_SCANCODE_PAGEUP,
			Delete = SDL_SCANCODE_DELETE,
			End = SDL_SCANCODE_END,
			PageDown = SDL_SCANCODE_PAGEDOWN,
			Right = SDL_SCANCODE_RIGHT,
			Left = SDL_SCANCODE_LEFT,
			Down = SDL_SCANCODE_DOWN,
			Up = SDL_SCANCODE_UP,

			NumLockClear = SDL_SCANCODE_NUMLOCKCLEAR,

			KeyPadDivide = SDL_SCANCODE_KP_DIVIDE,
			KeyPadMultiply = SDL_SCANCODE_KP_MULTIPLY,
			KeyPadMinus = SDL_SCANCODE_KP_MINUS,
			KeyPadPlus = SDL_SCANCODE_KP_PLUS,
			KeyPadEnter = SDL_SCANCODE_KP_ENTER,
			KeyPadOne = SDL_SCANCODE_KP_1,
			KeyPadTwo = SDL_SCANCODE_KP_2,
			KeyPadThree = SDL_SCANCODE_KP_3,
			KeyPadFour = SDL_SCANCODE_KP_4,
			KeyPadFive = SDL_SCANCODE_KP_5,
			KeyPadSix = SDL_SCANCODE_KP_6,
			KeyPadSeven = SDL_SCANCODE_KP_7,
			KeyPadEight = SDL_SCANCODE_KP_8,
			KeyPadNine = SDL_SCANCODE_KP_9,
			KeyPadZero = SDL_SCANCODE_KP_0,
			KeyPadPeriod = SDL_SCANCODE_KP_PERIOD,

			NonUSBackSlash = SDL_SCANCODE_NONUSBACKSLASH,
			Application = SDL_SCANCODE_APPLICATION,
			Power = SDL_SCANCODE_POWER,
			KeypadEquals = SDL_SCANCODE_KP_EQUALS,
			F13 = SDL_SCANCODE_F13,
			F14 = SDL_SCANCODE_F14,
			F15 = SDL_SCANCODE_F15,
			F16 = SDL_SCANCODE_F16,
			F17 = SDL_SCANCODE_F17,
			F18 = SDL_SCANCODE_F18,
			F19 = SDL_SCANCODE_F19,
			F20 = SDL_SCANCODE_F20,
			F21 = SDL_SCANCODE_F21,
			F22 = SDL_SCANCODE_F22,
			F23 = SDL_SCANCODE_F23,
			F24 = SDL_SCANCODE_F24,
			Execute = SDL_SCANCODE_EXECUTE,
			Help = SDL_SCANCODE_HELP,
			Menu = SDL_SCANCODE_MENU,
			Select = SDL_SCANCODE_SELECT,
			Stop = SDL_SCANCODE_STOP,
			Again = SDL_SCANCODE_AGAIN,
			Undo = SDL_SCANCODE_UNDO,
			Cut = SDL_SCANCODE_CUT,
			Copy = SDL_SCANCODE_COPY,
			Paste = SDL_SCANCODE_PASTE,
			Find = SDL_SCANCODE_FIND,
			Mute = SDL_SCANCODE_MUTE,
			VolumeUp = SDL_SCANCODE_VOLUMEUP,
			VolumeDown = SDL_SCANCODE_VOLUMEDOWN,

			KeypadComma = SDL_SCANCODE_KP_COMMA,
			KeypadEquals400 = SDL_SCANCODE_KP_EQUALSAS400,

			International1 = SDL_SCANCODE_INTERNATIONAL1,
			International2 = SDL_SCANCODE_INTERNATIONAL2,
			International3 = SDL_SCANCODE_INTERNATIONAL3,
			International4 = SDL_SCANCODE_INTERNATIONAL4,
			International5 = SDL_SCANCODE_INTERNATIONAL5,
			International6 = SDL_SCANCODE_INTERNATIONAL6,
			International7 = SDL_SCANCODE_INTERNATIONAL7,
			International8 = SDL_SCANCODE_INTERNATIONAL8,
			International9 = SDL_SCANCODE_INTERNATIONAL9,
			Language1 = SDL_SCANCODE_LANG1,
			Language2 = SDL_SCANCODE_LANG2,
			Language3 = SDL_SCANCODE_LANG3,
			Language4 = SDL_SCANCODE_LANG4,
			Language5 = SDL_SCANCODE_LANG5,
			Language6 = SDL_SCANCODE_LANG6,
			Language7 = SDL_SCANCODE_LANG7,
			Language8 = SDL_SCANCODE_LANG8,
			Language9 = SDL_SCANCODE_LANG9,

			AlternateErase = SDL_SCANCODE_ALTERASE,
			SystemRequest = SDL_SCANCODE_SYSREQ,
			Cancel = SDL_SCANCODE_CANCEL,
			Clear = SDL_SCANCODE_CLEAR,
			Prior = SDL_SCANCODE_PRIOR,
			Return2 = SDL_SCANCODE_RETURN2,
			Separator = SDL_SCANCODE_SEPARATOR,
			Out = SDL_SCANCODE_OUT,
			Oper = SDL_SCANCODE_OPER,
			ClearAgain = SDL_SCANCODE_CLEARAGAIN,
			CrSel = SDL_SCANCODE_CRSEL,
			ExSel = SDL_SCANCODE_EXSEL,

			Keypad00 = SDL_SCANCODE_KP_00,
			Keypad000 = SDL_SCANCODE_KP_000,
			ThousandsSeparator = SDL_SCANCODE_THOUSANDSSEPARATOR,
			DecimalSeparator = SDL_SCANCODE_DECIMALSEPARATOR,
			CurrencyUnit = SDL_SCANCODE_CURRENCYUNIT,
			CurrencySubunit = SDL_SCANCODE_CURRENCYSUBUNIT,
			KeypadLeftParentheses = SDL_SCANCODE_KP_LEFTPAREN,
			KeypadRightParentheses = SDL_SCANCODE_KP_RIGHTPAREN,
			KeypadLeftBrace = SDL_SCANCODE_KP_LEFTBRACE,
			KeypadRightBrace = SDL_SCANCODE_KP_RIGHTBRACE,
			KeypadTab = SDL_SCANCODE_KP_TAB,
			KeypadBackspace = SDL_SCANCODE_KP_BACKSPACE,
			KeypadA = SDL_SCANCODE_KP_A,
			KeypadB = SDL_SCANCODE_KP_B,
			KeypadC = SDL_SCANCODE_KP_C,
			KeypadD = SDL_SCANCODE_KP_D,
			KeypadE = SDL_SCANCODE_KP_E,
			KeypadF = SDL_SCANCODE_KP_F,
			KeypadXOR = SDL_SCANCODE_KP_XOR,
			KeypadPower = SDL_SCANCODE_KP_POWER,
			KeypadPercent = SDL_SCANCODE_KP_PERCENT,
			KeypadLess = SDL_SCANCODE_KP_LESS,
			KeypadGreater = SDL_SCANCODE_KP_GREATER,
			KeypadAmpersand = SDL_SCANCODE_KP_AMPERSAND,
			KeypadDoubleAmpersand = SDL_SCANCODE_KP_DBLAMPERSAND,
			KeypadVerticalBar = SDL_SCANCODE_KP_VERTICALBAR,
			KeypadDoubleVerticalBar = SDL_SCANCODE_KP_DBLVERTICALBAR,
			KeypadColon = SDL_SCANCODE_KP_COLON,
			KeypadHash = SDL_SCANCODE_KP_HASH,
			KeypadSpace = SDL_SCANCODE_KP_SPACE,
			KeypadAt = SDL_SCANCODE_KP_AT,
			KeypadExclamation = SDL_SCANCODE_KP_EXCLAM,
			KeypadMemoryStore = SDL_SCANCODE_KP_MEMSTORE,
			KeypadMemoryRecall = SDL_SCANCODE_KP_MEMRECALL,
			KeypadMemoryClear = SDL_SCANCODE_KP_MEMCLEAR,
			KeypadMemoryAdd = SDL_SCANCODE_KP_MEMADD,
			KeypadMemorySubtract = SDL_SCANCODE_KP_MEMSUBTRACT,
			KeypadMemoryMultiply = SDL_SCANCODE_KP_MEMMULTIPLY,
			KeypadMemoryDivide = SDL_SCANCODE_KP_MEMDIVIDE,
			KeypadPlusMinus = SDL_SCANCODE_KP_PLUSMINUS,
			KeypadClear = SDL_SCANCODE_KP_CLEAR,
			KeypadClearEntry = SDL_SCANCODE_KP_CLEARENTRY,
			KeypadBinary = SDL_SCANCODE_KP_BINARY,
			KeypadOctal = SDL_SCANCODE_KP_OCTAL,
			KeypadDecimal = SDL_SCANCODE_KP_DECIMAL,
			KeypadHexadecimal = SDL_SCANCODE_KP_HEXADECIMAL,

			LeftControl = SDL_SCANCODE_LCTRL,
			LeftShift = SDL_SCANCODE_LSHIFT,
			LeftAlt = SDL_SCANCODE_LALT,
			LeftGUI = SDL_SCANCODE_LGUI,
			RightControl = SDL_SCANCODE_RCTRL,
			RightShift = SDL_SCANCODE_RSHIFT,
			RightAlt = SDL_SCANCODE_RALT,
			RightGUI = SDL_SCANCODE_RGUI,
		};

		enum class Mouse
		{
			Left = SDL_BUTTON_LEFT,
			Middle = SDL_BUTTON_MIDDLE,
			Right = SDL_BUTTON_RIGHT,
			Extra1 = SDL_BUTTON_X1,
			Extra2 = SDL_BUTTON_X2,
			MouseAxisX,
			MouseAxisY,
			MouseWheelX,
			MouseWheelY
		};

		enum class GamepadAxis
		{
			// Axis
			LeftX = SDL_CONTROLLER_AXIS_LEFTX,
			LeftY = SDL_CONTROLLER_AXIS_LEFTY,
			RightX = SDL_CONTROLLER_AXIS_RIGHTX,
			RightY = SDL_CONTROLLER_AXIS_RIGHTY,
			TriggerLeft = SDL_CONTROLLER_AXIS_TRIGGERLEFT,
			TriggerRight = SDL_CONTROLLER_AXIS_TRIGGERRIGHT
		};

		enum class GamepadButton
		{
			A = SDL_CONTROLLER_BUTTON_A,
			B = SDL_CONTROLLER_BUTTON_B,
			X = SDL_CONTROLLER_BUTTON_X,
			Y = SDL_CONTROLLER_BUTTON_Y,
			Back = SDL_CONTROLLER_BUTTON_BACK,
			Guide = SDL_CONTROLLER_BUTTON_GUIDE,
			Start = SDL_CONTROLLER_BUTTON_START,
			LeftStick = SDL_CONTROLLER_BUTTON_LEFTSTICK,
			RightStick = SDL_CONTROLLER_BUTTON_RIGHTSTICK,
			LeftShoulder = SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
			RightShoulder = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,
			Up = SDL_CONTROLLER_BUTTON_DPAD_UP,
			Down = SDL_CONTROLLER_BUTTON_DPAD_DOWN,
			Left = SDL_CONTROLLER_BUTTON_DPAD_LEFT,
			Right = SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
			Misc = SDL_CONTROLLER_BUTTON_MISC1,
			Paddle1 = SDL_CONTROLLER_BUTTON_PADDLE1,
			Paddle2 = SDL_CONTROLLER_BUTTON_PADDLE2,
			Paddle3 = SDL_CONTROLLER_BUTTON_PADDLE3,
			Paddle4 = SDL_CONTROLLER_BUTTON_PADDLE4,
			Touchpad = SDL_CONTROLLER_BUTTON_TOUCHPAD,
		};

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

	// Mouse inputs Engine3in SDL don't have enums, so need to use my own.
	// This is used internally so that rebinding can be done dynamically at runtime without me having
	// to explicitly support the type. As long as SDL does, and it's in these enums then it can be used.
	using InternalInputType = std::variant<
		SDL_Scancode, Input::Mouse, SDL_GameControllerButton, SDL_GameControllerAxis
	>;

	// These are only used to create a non-SDL dependent public interface when setting the initial
	// bindings.
	using InputType = std::variant<
		Input::Key, Input::Mouse, Input::GamepadButton, Input::GamepadAxis
	>;

	class Action
	{
		friend class InputManager;

	protected:
		std::map<InternalInputType, Input> BoundInputs;

		bool CumulateInputs;

		Action(bool cumulateInputs) : CumulateInputs(cumulateInputs) {}

		void Update(InternalInputType type, ProcessState state, InputValue value);

		virtual void Process() = 0;

	public:
		virtual ~Action() = default;

		Input& AddInput(Input::Key input)
		{
			return BoundInputs.emplace(static_cast<SDL_Scancode>(input), Input{}).first->second;
		}

		Input& AddInput(Input::Mouse input) { return BoundInputs.emplace(input, Input{}).first->second; }

		Input& AddInput(Input::GamepadButton input)
		{
			return BoundInputs.emplace(static_cast<SDL_GameControllerButton>(input), Input{}).first->second;
		}

		Input& AddInput(Input::GamepadAxis input)
		{
			return BoundInputs.emplace(static_cast<SDL_GameControllerAxis>(input), Input{}).first->second;
		}
	};

	namespace Implementation
	{
		// This is needed so that Process can be specialised for each valid type of T.
		template <typename... T>
		class Action final : public Engine3::Action
		{
			friend class Engine3::InputManager;

		private:
			std::function<void(T...)> BoundFunction;

			auto FilterInputs();

		protected:
			void Process() override;

			Action(std::function<void(T...)> function, bool cumulateInputs)
				: Engine3::Action(cumulateInputs), BoundFunction(function) {}
		};
	}
}
