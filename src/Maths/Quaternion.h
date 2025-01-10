#pragma once
#include "Maths.h"
#include <cassert>
#include <numbers>

namespace Engine3
{
	// Probably could use the Vector struct to reduce code repetition, but I don't want to create a dependency if I can help it.
	template <std::floating_point T>
	struct Quaternion
	{
		/* STATIC */
		/// When a quaternion, q, is multiplied by the identity quaternion the result is the unaltered q.
		/// 
		/// Geometrically, the identity quaternion represents no angular displacement.
		/// @return The identity quaternion.
		static constexpr Quaternion Identity() { return {0, 0, 0, 1}; }

		/// Compute the angular displacement that rotates \p lhs into \p rhs.
		/// 
		/// This function assumes both quaternions involved are unit rotation quaternions to use the conjugate
		/// instead of inverse.
		static constexpr Quaternion Difference(const Quaternion& lhs, const Quaternion& rhs)
		{
			assert(lhs.IsUnit());
			assert(rhs.IsUnit());

			// In rotation quaternions, the conjugate is the same as the inverse.
			// This is effectively quaternion division, rhs / this.
			return rhs * lhs.Conjugate();
		}

		/// The larger the absolute value of the quaternion dot product, the smaller the angular displacement between
		/// \p lhs and \p rhs.
		/// @return For unit quaternions, a scalar value in the inclusive range [-1, 1].
		static constexpr float DotProduct(const Quaternion& lhs, const Quaternion& rhs)
		{
			auto [x1, y1, z1, w1] = lhs;
			auto [x2, y2, z2, w2] = rhs;

			return x1 * x2 + y1 * y2 + z1 * z2 + w1 * w1;
		}

		/* INSTANCE */
		/// Quaternions are an encoding of the axis-angle format.
		/// xyz is sin(theta / 2) multiplied by the unit axis,
		/// and w is cos(theta / 2).
		T X, Y, Z, W;

		/// For unit quaternions that represent orientations, the magnitude is always 1.
		constexpr T LengthSquared() const { return DotProduct(*this, *this); }

		/// For unit quaternions that represent orientations, the magnitude is always 1.
		/// Prefer using LengthSquared where possible to avoid unnecessary computation.
		constexpr T Length() const { return SquareRoot(LengthSquared()); }

		/// For unit quaternions that represent orientations, the conjugate is the same as the inverse and is equivalent
		/// to flipping the direction of the encoded axis.
		/// @return The quaternion with a negated vector portion.
		constexpr Quaternion Conjugate() const { return {-X, -Y, -Z, W}; }

		/// When multiplying a quaternion by its inverse, you get the identity quaternion.
		/// 
		/// If the quaternion is a unit quaternion, its conjugate is equivalent to its inverse and should be preferred to
		/// avoid unnecessary computation.
		/// @return The inverse quaternion.
		constexpr Quaternion Inverted() const
		{
			assert(LengthSquared() != 0);
			return Conjugate() / Length();
		}

		/// Compute the angular displacement that rotates \p this into \p rhs. \n
		/// This function assumes both quaternions involved are rotation quaternions.
		constexpr Quaternion Difference(const Quaternion& rhs) const { return Difference(*this, rhs); }

		constexpr T DotProduct(const Quaternion& rhs) const { return DotProduct(*this, rhs); }

		/// Quaternion exponentiation can be computed to get a portion of the angular displacement represented
		/// by a quaternion.
		/// 
		/// e.g. A quaternion that represents a 30-degree clockwise rotation about the x-axis raised to the power of 1/2
		/// would become a 15-degree clockwise rotation about the x-axis.
		/// 
		/// This function assumes unit quaternions.
		constexpr Quaternion Exponentiated(T exponent) const
		{
			// The assumption is only unit rotation quaternions will be used with this.
			assert(IsUnit());
			Quaternion result = *this;
			auto& [x, y, z, w] = result;

			// Prevent divide by zero when identity quaternion.
			// The negative identity quaternion results in the same value
			// as the identity quaternion, hence absolute value.
			if (AlmostLessThan(std::abs(w), 1))
			{
				// The w component of a quaternion is equal to cos(theta/2).
				T halfAngle = std::acos(w);

				// Compute the new half angle
				T newHalfAngle = halfAngle * exponent;

				// Compute new w value
				w = std::cos(newHalfAngle);

				// Compute new xyz values by scaling the values by the proportional difference in angle.
				T scale = std::sin(newHalfAngle) / std::sin(halfAngle);
				x *= scale;
				y *= scale;
				z *= scale;
			}

			// This shouldn't occur, but just in case.
			assert(result.IsUnit());
			return result;
		}

		constexpr bool IsUnit() const
		{
			// It is not necessary to check Length(), because if it is a unit vector then LengthSquared() is the same value.
			return AlmostEquals<T>(1, LengthSquared());
		}

		/* OPERATORS */
		/// Negating each component in a quaternion is equivalent to adding 360 degrees to its rotation about an axis.
		/// In other words, there's no angular displacement and aliasing occurs.
		constexpr Quaternion operator-() const { return {-X, -Y, -Z, -W}; }

		/// Also known as the Hamilton product.
		constexpr friend Quaternion operator*(Quaternion lhs, Quaternion rhs)
		{
			// The new vector component (xyz) is calculated by
			// multiplying each w component by the other quaternion's vector component, and then
			// adding the cross product of the two vector components.
			// The new w component is calculated by
			// multiplying the two w components together and subtracting the dot product of the two vector components.

			auto& [x1, y1, z1, w1] = lhs;
			auto& [x2, y2, z2, w2] = rhs;
			return
			{
				w1 * x2 + x1 * w2 + y1 * z2 - z1 * y2,
				w1 * y2 + y1 * w2 + z1 * x2 - x1 * z2,
				w1 * z2 + z1 * w2 + x1 * y2 - y1 * x2,
				w1 * w2 - x1 * x2 - y1 * y2 - z1 * z2
			};
		}
	};

	/// Compute a quaternion between two others by a percentage.
	template <typename T, typename U>
	constexpr Quaternion<T> LinearInterpolation(const Quaternion<T>& start, const Quaternion<T>& end, U fraction)
	{
		Quaternion<T> difference = Quaternion<T>::Difference(start, end);
		Quaternion<T> portion = difference.Exponentiated(fraction);
		return start * portion;
	}

	/// Compute a quaternion by interpolating around the arc that connects two quaternions on the surface of a 4D hypersphere.
	/// Unlike linear interpolation this method of interpolation maintains a uniform angular rotation.
	template <typename T, typename U>
	constexpr Quaternion<T> SphericalLinearInterpolation(const Quaternion<T>& start, const Quaternion<T>& end,
	                                                     U fraction)
	{
		assert(start.IsUnit());
		assert(end.IsUnit());

		auto& [x0, y0, z0, w0] = start;
		auto [x1, y1, z1, w1] = end;

		T cosineOfTheAngle = Quaternion<T>::DotProduct(start, end);

		// If the dot product is negative there's a shorter arc available
		// by negating the values.
		if (cosineOfTheAngle < 0)
		{
			x1 = -x1;
			y1 = -y1;
			z1 = -z1;
			w1 = -z1;
			cosineOfTheAngle = -cosineOfTheAngle;
		}

		// If divide by zero, linearly interpolate to avoid NaN/infinity.
		T k0, k1;
		if (AlmostGreaterThan(cosineOfTheAngle, 1))
		{
			k0 = 1.0f - fraction;
			k1 = fraction;
		}
		else
		{
			// Calculated using the trig identity: sin^2(omega) + cos^2(omega) = 1
			T sinOfTheAngle = std::sqrt(1 - cosineOfTheAngle * cosineOfTheAngle);

			T angle = std::atan2(sinOfTheAngle, cosineOfTheAngle);

			// Cache so only a single division is necessary.
			float inverseAngle = 1 / angle;

			k0 = std::sin((1.0f - fraction) * angle) * inverseAngle;
			k1 = std::sin(fraction * angle) * inverseAngle;
		}

		// Interpolate
		Quaternion result
		{
			x0 * k0 + x1 * k1,
			y0 * k0 + y1 * k1,
			z0 * k0 + z1 * k1,
			w0 * k0 + w1 * k1
		};

		// This shouldn't occur, but just in case.
		auto [x, y, z, w] = result;
		assert(result.IsUnit());
		return result;
	}
}
