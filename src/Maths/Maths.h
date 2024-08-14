#pragma once
#include <concepts>
#include <numbers>

namespace Engine3
{
	template <std::floating_point T>
	constexpr T DegreesToRadians(T degrees) { return degrees * (std::numbers::pi_v<T> / static_cast<T>(180)); }
}
