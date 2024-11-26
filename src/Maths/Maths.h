#pragma once
#include <cmath>
#include <concepts>
#include <limits>
#include <numbers>
#include <print>

namespace Engine3
{
	/*
	 * Recreating STD functions for constexpr.
	 *
	 */

	template <typename T>
	constexpr T Abs(T value)
	{
		// TODO: Use constexpr std::abs: P0533 https://en.cppreference.com/w/cpp/compiler_support
		return value < 0 ? -value : value;
	}

	template <std::floating_point T>
	constexpr T SquareRoot(T number)
	{
		// TODO: Use constexpr std::sqrt: P0533 https://en.cppreference.com/w/cpp/compiler_support
		// TODO: Handle edge cases/Hope by the time there's problems the compilers are updated.

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

	/*
	 * Engine Functions
	 *
	 */
	template <class T>
	concept Number = std::is_floating_point_v<T> || std::is_integral_v<T>;

	template <std::size_t a, std::size_t b>
	concept IsSquare = (a == b);

	template <std::size_t Size, std::size_t ... ValidSizes>
	concept IsValidDimensions = ((Size == ValidSizes) || ...);

	template <std::floating_point T>
	constexpr T DegreesToRadians(T degrees) { return degrees * (std::numbers::pi_v<T> / static_cast<T>(180)); }

	template <typename T>
	constexpr bool AlmostEquals(T lhs, T rhs, T epsilon = std::numeric_limits<T>::epsilon() * 100)
	{
		T difference = lhs - rhs;
		return Abs(difference) < epsilon;
	}

	/// Account for the cyclic nature of angles by wrapping the passed angle around
	/// in the inclusive, exclusive range (-180, +180].
	/// \n This is useful for interpolating between the shortest path between two angles.
	/// @param angle Angle in radians to wrap.
	/// @return The wrapped angle, e.g. 190 degrees becomes -170 degrees.
	template <std::floating_point T>
	constexpr T WrapAngle(T angle)
	{
		constexpr T halfTurn = std::numbers::pi_v<T>;

		// If already in range, skip calculations.
		if (std::abs(angle) <= halfTurn) { return angle; }

		// Add or subtract the necessary full turns to wrap around.
		constexpr T fullTurn = halfTurn * 2;
		T revolutions = std::floor((angle + halfTurn) * (1.f / fullTurn));
		angle -= revolutions * fullTurn;

		return angle;
	}
}
