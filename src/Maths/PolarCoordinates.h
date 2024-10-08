#pragma once
#include "Maths.h"
#include "Vector.h"
#include <concepts>
#include <numbers>
#include <tuple>

namespace Engine3
{
	template <std::floating_point T = float>
	struct PolarPoint
	{
		// aka radius
		T Distance;

		// The angle in radians.
		T Angle;

		constexpr PolarPoint() = default;

		constexpr PolarPoint(T distance, T angle) : Distance(distance), Angle(angle) {}

		/// Converts a point in cartesian space to polar space. 
		/// @param cartesianCoordinate The point in cartesian space to convert to polar space.
		// explicit to prevent something like "PolarPoint{} == Vector<2>{}" from compiling.
		explicit constexpr PolarPoint(Vector<2, T> cartesianCoordinate)
		{
			if (cartesianCoordinate.X() == 0 && cartesianCoordinate.Y() == 0)
			{
				Distance = 0;
				Angle = 0;
			}
			else
			{
				// TODO: std::atan2 prevents constexpr.
				Distance = cartesianCoordinate.Length();
				Angle = std::atan2(cartesianCoordinate.Y(), cartesianCoordinate.X());
			}
		}

		constexpr Vector<2, T> ToCartesian()
		{
			// TODO: std::cos/std::sin prevents constexpr.
			return
			{
				Distance * std::cos(Angle),
				Distance * std::sin(Angle)
			};
		}

		/// Simplifies the polar point into its canonical form, where:
		///	\n \p Distance >= 0,
		///	\n -pi < \p Angle <= pi,
		/// \n \p Distance = 0 => \p Angle = 0
		/// @return The canonical polar coordinate in radians.
		constexpr PolarPoint CanonicalForm()
		{
			constexpr T halfTurn = std::numbers::pi_v<T>;
			constexpr T fullTurn = 2 * std::numbers::pi_v<T>;

			PolarPoint canonical = *this;

			// Distance is 0, making angle irrelevant.
			// None of the other code needs to be run if this is the case.
			if (canonical.Distance == 0)
			{
				canonical.Angle = 0;
				return canonical;
			}

			// Negative distance.
			// Make it positive and add 180-degree turn to get the same position.
			if (canonical.Distance < 0)
			{
				canonical.Distance = -canonical.Distance;
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

		friend bool operator==(const PolarPoint& lhs, const PolarPoint& rhs)
		{
			return lhs.Distance == rhs.Distance
				&& lhs.Angle == rhs.Angle;
		}

		friend bool operator!=(const PolarPoint& lhs, const PolarPoint& rhs) { return !(lhs == rhs); }
	};
}
