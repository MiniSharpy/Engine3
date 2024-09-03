#pragma once
#include <cmath>
#include <concepts>
#include <limits>
#include <numbers>

namespace Engine3
{
	template <std::floating_point T>
	constexpr T DegreesToRadians(T degrees) { return degrees * (std::numbers::pi_v<T> / static_cast<T>(180)); }

	template <class T>
	concept Number = std::is_floating_point_v<T> || std::is_integral_v<T>;

	template <std::size_t a, std::size_t b>
	concept IsSquare = (a == b);

	template <typename T>
	constexpr bool AlmostEquals(T lhs, T rhs)
	{
		// TODO: Use constexpr std::abs: P0533 https://en.cppreference.com/w/cpp/compiler_support
		T difference = lhs - rhs;
		T abs = difference < 0 ? -difference : difference;

		return abs < std::numeric_limits<T>::epsilon() * 100;
	}

	template <std::floating_point T>
	constexpr T SquareRoot(T number)
	{
		// TODO: Use constexpr std::sqrt: P0533 https://en.cppreference.com/w/cpp/compiler_support
		// TODO: Consider edge cases.

		// https://stackoverflow.com/a/34134071
		constexpr auto newtonRaphson = [](this auto const& newtonRaphson,
		                                       T original, T current, T previous) constexpr -> T
		{
			return current == previous
				       ? current
				       : newtonRaphson(original, static_cast<T>(0.5) * (current + original / current), current);
		};

		return number >= 0 && number < std::numeric_limits<T>::infinity()
			       ? newtonRaphson(number, number, 0)
			       : std::numeric_limits<T>::quiet_NaN();
	}

	template <std::integral T>
	constexpr double SquareRoot(T number) { return SquareRoot(static_cast<double>(number)); }
}
