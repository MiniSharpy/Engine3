#pragma once
#include "Maths.h"
#include <numbers>

namespace Engine3
{
	template <std::size_t Dimensions, Number T>
	struct Vector;

	/// Follows mathematical convention.
	template <std::floating_point T = float>
	struct PolarCoordinates2D
	{
		// Radius relative to the origin.
		T Radius;

		// Angle relative to the origin and "right" direction in radians.
		// Positive rotation is counterclockwise.
		T Angle;

		/// Simplifies the polar point into its canonical form, where:
		///	\n \p Distance >= 0,
		///	\n -pi < \p Angle <= pi,
		/// \n \p Distance = 0 => \p Angle = 0
		/// @return The canonical polar coordinate in radians.
		constexpr PolarCoordinates2D CanonicalForm() const
		{
			constexpr T halfTurn = std::numbers::pi_v<T>;
			constexpr T fullTurn = 2 * std::numbers::pi_v<T>;

			PolarCoordinates2D canonical = *this;
			auto& [radius, angle] = canonical;

			// Distance is 0, making angle irrelevant.
			// None of the other code needs to be run if this is the case.
			if (radius == 0)
			{
				angle = 0;
				return canonical;
			}

			// Negative distance.
			// Make it positive and add 180-degree turn to get the same position.
			if (radius < 0)
			{
				radius = -radius;
				angle += halfTurn;
			}

			// Angle out of range.
			// Offset a half turn and then subtract by the degrees of the full turns to get in the range 0-360 degrees.
			// Finally, subtract by a half turn to get in the correct range.
			if (Abs(angle) > halfTurn)
			{
				angle += halfTurn;
				angle -= std::floor(angle / fullTurn) * fullTurn;
				angle -= halfTurn;
			}

			// When really close to being on the lower bounds it may result in incorrect results,
			// such as (-5 distance, -720 degrees) ending up as (5, -180) when it should be (5, 180).
			// TODO: I wonder if by solving such an edge case it'll cause another?
			if (AlmostEquals(angle, -halfTurn)) { angle = halfTurn; }

			return canonical;
		}

		constexpr Vector<2, T> ToVector2();

		constexpr friend bool operator==(const PolarCoordinates2D& lhs, const PolarCoordinates2D& rhs) = default;

		constexpr friend bool operator!=(const PolarCoordinates2D& lhs, const PolarCoordinates2D& rhs) = default;
	};

	// Hopefully things like creating a copy of the 2D coordinates will be optimised away,
	// but if not I doubt this will see much, if any, use.
	/// Follows mathematical convention.
	template <std::floating_point T = float>
	struct CylindricalCoordinates
	{
		// Radius relative to the origin.
		T Radius;

		// Angle relative to the origin and "right" direction in radians.
		// Positive rotation is counterclockwise.
		T Angle;

		// Height relative to the origin.
		T Z;

		/// Simplifies the polar point into its canonical form, where:
		///	\n \p Distance >= 0,
		///	\n -pi < \p Angle <= pi,
		/// \n \p Distance = 0 => \p Angle = 0
		/// @return The canonical polar coordinate in radians.
		constexpr CylindricalCoordinates CanonicalForm() const
		{
			PolarCoordinates2D<T> base = PolarCoordinates2D<T>{Radius, Angle}.CanonicalForm();
			return {base.Radius, base.Angle, Z};
		}

		constexpr Vector<3, T> ToVector3();

		constexpr friend bool operator==(const CylindricalCoordinates& lhs, const CylindricalCoordinates& rhs)
		= default;

		constexpr friend bool operator!=(const CylindricalCoordinates& lhs, const CylindricalCoordinates& rhs)
		= default;
	};

	/// Follows conventions for a left-handed coordinate system.
	template <std::floating_point T = float>
	struct SphericalCoordinates
	{
		// Radius relative to the origin.
		T Radius;

		// Heading relative to the origin and "forward" direction in radians.
		// Positive rotation is clockwise.
		T Heading;

		// Pitch relative to the origin and "up" direction in radians.
		// Positive rotation is downwards, to be consistent with the left-hand rule.
		T Pitch;

		/// Simplifies the polar point into its canonical form, where:
		/// TODO
		/// @return The canonical spherical coordinate in radians.
		constexpr SphericalCoordinates CanonicalForm() const
		{
			constexpr T quarterTurn = std::numbers::pi_v<T> / 2; // 90
			constexpr T halfTurn = std::numbers::pi_v<T>; // 180
			constexpr T threeQuarterTurn = quarterTurn * 3; // 270
			constexpr T fullTurn = 2 * std::numbers::pi_v<T>; // 370

			SphericalCoordinates canonical = *this;
			auto& [radius, heading, pitch] = canonical;

			// Distance is 0, making angle irrelevant.
			// None of the other code needs to be run if this is the case.
			if (radius == 0)
			{
				heading = pitch = 0;
				return canonical;
			}

			// Negative distance.
			// Make it positive and add 180-degree turn to get the same position.
			if (radius < 0)
			{
				radius = -radius;
				heading += halfTurn;
				pitch = -pitch;
			}

			// Pitch out of range.
			if (Abs(pitch) > quarterTurn)
			{
				// Wrap in 0-360 degrees.
				pitch += quarterTurn;
				pitch -= std::floor(pitch / fullTurn) * fullTurn;

				// If final adjusted value would exceed 0-90 degrees, adjust further.
				if (pitch > halfTurn)
				{
					heading += halfTurn;
					pitch = threeQuarterTurn - pitch;
				}
				else { pitch -= quarterTurn; }
			}

			// Gimbal lock.
			if (Abs(pitch) > quarterTurn || AlmostEquals(pitch, quarterTurn))
			{
				heading = 0;

				// If pitch is almost completely up or down, guarantee it.
				pitch = std::floor(pitch / quarterTurn) * pitch;
				return canonical;
			}

			// Heading out of range.
			// Offset a half turn and then subtract by the degrees of the full turns to get in the range 0-360 degrees.
			// Finally, subtract by a half turn to get in the correct range.
			if (Abs(heading) > halfTurn)
			{
				heading += halfTurn;
				heading -= std::floor(heading / fullTurn) * fullTurn;
				heading -= halfTurn;
			}

			// Being really close caused inaccurate in 2D polar coordinates, so just in case handle floating point imprecision.
			if (AlmostEquals(heading, -halfTurn)) { heading = halfTurn; }

			return canonical;
		}

		constexpr Vector<3, T> ToVector3();

		constexpr friend bool operator==(const SphericalCoordinates& lhs, const SphericalCoordinates& rhs) = default;

		constexpr friend bool operator!=(const SphericalCoordinates& lhs, const SphericalCoordinates& rhs) = default;
	};
}

// The positioning of this include is important to handle circular dependencies.
#include "Vector.h"

template <std::floating_point T>
constexpr Engine3::Vector<2, T> Engine3::PolarCoordinates2D<T>::ToVector2()
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

template <std::floating_point T>
constexpr Engine3::Vector<3, T> Engine3::SphericalCoordinates<T>::ToVector3()
{
	// TODO: std::cos/std::sin prevents constexpr.
	return
	{
		Radius * std::cos(Pitch) * std::sin(Heading),
		-Radius * std::sin(Pitch),
		Radius * std::cos(Pitch) * std::cos(Heading)
	};
}
