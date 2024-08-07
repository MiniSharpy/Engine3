#pragma once
#include <cassert>
#include <memory>
#include <print>
#include <SDL.h>
#include <string_view>
#include <utility>

namespace Engine3
{
	// I could combine with the renderer, but in case I ever want to roll my windowing abstraction I've kept them separate.
	class Window
	{
	public:
		// DirectX doesn't need a flag for some reason.
		enum class Flags : std::uint32_t // uint32 to match SDL.
		{
			None = 0,
			Fullscreen = SDL_WINDOW_FULLSCREEN,
			OpenGL = SDL_WINDOW_OPENGL,
			Shown = SDL_WINDOW_SHOWN,
			Hidden = SDL_WINDOW_HIDDEN,
			Borderless = SDL_WINDOW_BORDERLESS,
			Resizable = SDL_WINDOW_RESIZABLE,
			Minimised = SDL_WINDOW_MINIMIZED,
			Maximised = SDL_WINDOW_MAXIMIZED,
			//Grabbed = SDL_WINDOW_MOUSE_GRABBED,
			//InputFocus = SDL_WINDOW_INPUT_FOCUS,
			//MouseFocus = SDL_WINDOW_MOUSE_FOCUS,
			FullScreenDesktop = SDL_WINDOW_FULLSCREEN_DESKTOP,
			//Foreign = SDL_WINDOW_FOREIGN,
			//AllowHighDPI = SDL_WINDOW_ALLOW_HIGHDPI, // Need to look into how this works.
			//MouseCapture = SDL_WINDOW_MOUSE_CAPTURE,
			//AlwaysOnTop = SDL_WINDOW_ALWAYS_ON_TOP,
			//SkipTaskbar = SDL_WINDOW_SKIP_TASKBAR,
			//Utility = SDL_WINDOW_UTILITY,
			//ToolTip = SDL_WINDOW_TOOLTIP,
			//PopupMenu = SDL_WINDOW_POPUP_MENU,
			//KeyboardGrabbed = SDL_WINDOW_KEYBOARD_GRABBED,
			//Vulkan = SDL_WINDOW_VULKAN,
			//Metal = SDL_WINDOW_METAL,
			//InputGrabbed = SDL_WINDOW_INPUT_GRABBED
		};

		// SDL uses a macro to centre the window on the specified
		// display. To avoid using macros, and to simplify the ctor boilerplate,
		// this is handled conditionally in a function which permits more granular
		// control of the X/Y position on that display.
		enum class Position : std::uint8_t
		{
			Centred,
			Undefined
		};

		using enum Flags;

	private:
		std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> Window_;

		bool IsInitialised = true;

	public:
		/* CONSTRUCTORS */
		// No clue why there's need for the u8string_view here but not in my other project.
		Window(const std::u8string_view title, const int x, const int y, const int w, const int h,
		       const Flags flags) noexcept : Window_(
			SDL_CreateWindow(reinterpret_cast<const char*>(title.data()), x, y, w, h, static_cast<uint32_t>(flags)),
			SDL_DestroyWindow)
		{
			if (!Window_)
			{
				std::print("{}", SDL_GetError());
				IsInitialised = false;
				assert(false);
			}
		}

		template <typename T, typename U>
			requires
			(std::same_as<T, int> || std::same_as<T, Position>) &&
			(std::same_as<U, int> || std::same_as<U, Position>)
		Window(const std::u8string_view title, const T x, const U y, const int w, const int h,
		       const Flags flags = Flags::None, const int display = 0) noexcept
			: Window(title,
			         GetDisplayCoordinate(x, display, true),
			         GetDisplayCoordinate(y, display, false),
			         w, h,
			         flags) {}

		Window(const std::u8string_view title, const int w, const int h, const Flags flags = Flags::None) noexcept
			: Window(title,
			         Position::Centred,
			         Position::Centred,
			         w, h,
			         flags) {}

		~Window() = default;

		/* COPY AND MOVE OPERATIONS*/
		Window(const Window& other) = delete;

		Window(Window&& other) noexcept = delete;

		Window& operator=(const Window& other) = delete;

		Window& operator=(Window&& other) noexcept = delete;

		/* METHODS */
		std::pair<int, int> GetSize() const
		{
			std::pair<int, int> size;
			SDL_GetWindowSize(Window_.get(), &size.first, &size.second);
			return size;
		}

		void SetSize(int width, int height) { SDL_SetWindowSize(Window_.get(), width, height); }

	private:
		// Returns either the centred/undefined SDL window position,
		// or the SDL global coordinate position for the specified window
		// offset by the specified coordinate.
		template <typename T>
			requires (std::same_as<T, int> || std::same_as<T, Position>)
		static int GetDisplayCoordinate(T coordinate, const int displayIndex, const bool isXCoordinate)
		{
			int numberOfDisplays = SDL_GetNumVideoDisplays();
			if (numberOfDisplays <= 0) { std::print("{}\n", SDL_GetError()); }
			assert(numberOfDisplays >= displayIndex + 1);
			if constexpr (std::same_as<T, Position>)
			{
				switch (coordinate)
				{
				case Position::Centred: { return SDL_WINDOWPOS_CENTERED_DISPLAY(displayIndex); }
				case Position::Undefined: { return SDL_WINDOWPOS_UNDEFINED_DISPLAY(displayIndex); }
				default: std::unreachable();
				}
			}
			else
			{
				SDL_Rect rect;
				if (SDL_GetDisplayBounds(displayIndex, &rect) != 0)
				{
					std::print("{}\n", SDL_GetError());
					assert(false);
				}

				if (isXCoordinate) { return rect.x + coordinate; }
				else { return rect.y + coordinate; }
			}
		}

	public:
		/* CONVERSIONS */
		explicit operator bool() const { return IsInitialised; }

		/* FRIEND CLASSES */
		friend class Renderer;
	};

	/* FLAG OPERATORS */
	constexpr Window::Flags operator|(Window::Flags lhs, Window::Flags rhs)
	{
		return static_cast<Window::Flags>(
			std::to_underlying(lhs) |
			std::to_underlying(rhs)
		);
	}

	constexpr Window::Flags operator&(Window::Flags lhs, Window::Flags rhs)
	{
		return static_cast<Window::Flags>(
			std::to_underlying(lhs) |
			std::to_underlying(rhs)
		);
	}

	constexpr Window::Flags operator~(Window::Flags flag)
	{
		return static_cast<Window::Flags>(
			~std::to_underlying(flag));
	}
}
