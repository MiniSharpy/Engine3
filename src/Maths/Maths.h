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
		return std::abs(lhs - rhs) < std::numeric_limits<T>::epsilon() * 100 ? true : false;
	};
}
