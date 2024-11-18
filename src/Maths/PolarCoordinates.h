#pragma once
#include "Maths.h"
#include <numbers>

namespace Engine3
{
	template <std::size_t Dimensions, Number T>
	struct Vector;

	template <std::floating_point T = float>
	struct PolarCoordinates
	{
		// Radius, aka distance, relative to the origin.
		T Radius;

		// Relative to the origin and "right" direction in radians.
		T Angle;

		constexpr PolarCoordinates() = default;

		constexpr PolarCoordinates(T radius, T angle) : Radius(radius), Angle(angle) {}

		constexpr Vector<2, T> ToVector2();

		/// Simplifies the polar point into its canonical form, where:
		///	\n \p Distance >= 0,
		///	\n -pi < \p Angle <= pi,
		/// \n \p Distance = 0 => \p Angle = 0
		/// @return The canonical polar coordinate in radians.
		constexpr PolarCoordinates CanonicalForm()
		{
			constexpr T halfTurn = std::numbers::pi_v<T>;
			constexpr T fullTurn = 2 * std::numbers::pi_v<T>;

			PolarCoordinates canonical = *this;

			// Distance is 0, making angle irrelevant.
			// None of the other code needs to be run if this is the case.
			if (canonical.Radius == 0)
			{
				canonical.Angle = 0;
				return canonical;
			}

			// Negative distance.
			// Make it positive and add 180-degree turn to get the same position.
			if (canonical.Radius < 0)
			{
				canonical.Radius = -canonical.Radius;
				canonical.Angle += halfTurn;
			}

			// Angle out of range.
			// Offset a half turn and then subtract by the degrees of the full turns to get in the range 0-360 degrees.
			// Finally, subtract by a half turn to get in the correct range.
			if (Abs(canonical.Angle) > halfTurn)
			{
				canonical.Angle += halfTurn;
				canonical.Angle -= std::floor(canonical.Angle / fullTurn) * fullTurn;
				canonical.Angle -= halfTurn;
			}

			// When really close to being on the lower bounds it may result in incorrect results,
			// such as (-5 distance, -720 degrees) ending up as (5, -180) when it should be (5, 180).
			// TODO: I wonder if by solving such an edge case it'll cause another?
			if (AlmostEquals(canonical.Angle, -halfTurn)) { canonical.Angle = halfTurn; }

			return canonical;
		}

		constexpr friend bool operator==(const PolarCoordinates& lhs, const PolarCoordinates& rhs)
		{
			return lhs.Radius == rhs.Radius
				&& lhs.Angle == rhs.Angle;
		}

		constexpr friend bool operator!=(const PolarCoordinates& lhs, const PolarCoordinates& rhs)
		{
			return !(lhs == rhs);
		}
	};

	template <std::floating_point T = float>
	struct CylindricalCoordinates : Engine3::PolarCoordinates<T>
	{
		// Height relative to the origin.
		T Z;

		constexpr CylindricalCoordinates() = default;

		constexpr CylindricalCoordinates(T radius, T angle, T z) : PolarCoordinates<T>(radius, angle), Z{z} {}

		constexpr Vector<3, T> ToVector3();

		/// Simplifies the polar point into its canonical form, where:
		///	\n \p Distance >= 0,
		///	\n -pi < \p Angle <= pi,
		/// \n \p Distance = 0 => \p Angle = 0
		/// @return The canonical polar coordinate in radians.
		constexpr CylindricalCoordinates CanonicalForm()
		{
			PolarCoordinates<T> base = static_cast<PolarCoordinates<T>*>(this)->CanonicalForm();
			return {base.Radius, base.Angle, Z};
		}

		constexpr friend bool operator==(const CylindricalCoordinates& lhs, const CylindricalCoordinates& rhs)
		{
			return lhs.Radius == rhs.Radius &&
				lhs.Angle == rhs.Angle &&
				lhs.Z == rhs.Z;
		}

		constexpr friend bool operator!=(const CylindricalCoordinates& lhs, const CylindricalCoordinates& rhs)
		{
			return !(lhs == rhs);
		}
	};
}

// This order and prior forward declaration handle the circular dependency.
#include "Vector.h"

template <std::floating_point T>
constexpr Engine3::Vector<2, T> Engine3::PolarCoordinates<T>::ToVector2()
{
	// TODO: std::cos/std::sin prevents constexpr.
	return
	{
		Radius * std::cos(Angle),
		Radius * std::sin(Angle)
	};
}

template <std::floating_point T>
constexpr Engine3::Vector<3, T> Engine3::CylindricalCoordinates<T>::ToVector3()
{
	// TODO: std::cos/std::sin prevents constexpr.
	return
	{
		this->Radius * std::cos(this->Angle),
		this->Radius * std::sin(this->Angle),
		Z
	};
}
