#pragma once
#include "Maths.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <concepts>
#include <cstddef>

namespace Engine3
{
	template <std::size_t Dimensions, Number T = float>
	struct Vector : std::array<T, Dimensions>
	{
		/* Static Methods */
		static constexpr Vector Zero() { return {}; }

		static constexpr Vector Up() requires(Dimensions == 2) { return {static_cast<T>(0), static_cast<T>(1)}; }

		static constexpr Vector Right() requires (Dimensions == 2) { return {static_cast<T>(1), static_cast<T>(0)}; }

		static constexpr Vector Up() requires (Dimensions == 3)
		{
			return {static_cast<T>(0), static_cast<T>(1), static_cast<T>(0)};
		}

		static constexpr Vector Right() requires (Dimensions == 3)
		{
			return {static_cast<T>(1), static_cast<T>(0), static_cast<T>(0)};
		}

		static constexpr Vector Forward() requires (Dimensions == 3)
		{
			return {static_cast<T>(0), static_cast<T>(0), static_cast<T>(1)};
		}

		static constexpr Vector Back() requires (Dimensions == 3) { return {-Vector::Forward()}; }

		static constexpr Vector Left() requires (Dimensions == 2 || Dimensions == 3) { return {-Vector::Right()}; }

		static constexpr Vector Down() requires (Dimensions == 2 || Dimensions == 3) { return {-Vector::Up()}; }

		/// A commutative function that sums the products of each component in \p lhs and \p rhs.
		/// @return A negative scalar value when \p lhs points towards \p rhs. \n
		/// A zero value when the vectors are perpendicular, or when one is a zero vector.
		static constexpr T DotProduct(const Vector& lhs, const Vector& rhs)
		{
			// An integer squared is always an integer, no need to worry about precision loss when std::integral<T>,
			// just need to not allow other typed vectors as lhs parameter.
			T value = 0;
			for (size_t i = 0; i < Dimensions; ++i) { value += lhs[i] * rhs[i]; }

			return value;
		}

		/// An anti-commutative function, \p lhs x \p rhs, that can only be applied using 3D vectors,
		/// yielding a 3D vector perpendicular to the original two.
		/// @return Zero vector if \p lhs or \p rhs are parallel, of if either are the zero vector. \n
		/// Otherwise, a vector perpendicular to both vector.
		static constexpr Vector CrossProduct(const Vector& lhs, const Vector& rhs) requires (Dimensions == 3)
		{
			return {
				lhs.Y() * rhs.Z() - lhs.Z() * rhs.Y(),
				lhs.Z() * rhs.X() - lhs.X() * rhs.Z(),
				lhs.X() * rhs.Y() - lhs.Y() * rhs.X()
			};
		}

		/// Calculates the squared distance between two vectors.
		/// @return The squared magnitude of the vector \p rhs - \p lhs.
		static constexpr T DistanceSquared(const Vector& lhs, const Vector& rhs) { return (rhs - lhs).LengthSquared(); }

		/// Calculates the distance between two vectors.
		/// @return The magnitude of the vector \p rhs - \p lhs.
		static constexpr auto Distance(const Vector& lhs, const Vector& rhs) { return (rhs - lhs).Length(); }

		// Static to better emphasise the usage of the unit vector.
		/// Projects a vector onto an infinitely long line. \n
		///	Consider a right-angled triangle made up of vectors where the hypotenuse is \p rhs,
		///	the adjacent side \p Project(lhs, \p rhs), and the opposite side is \p ProjectPerpendicular(lhs, \p rhs).
		/// @param lhs Unit vector that determines the direction of the line that \p rhs will be projected upon.
		/// @param rhs Vector to project.
		/// @return The projection of \p rhs onto an infinitely long line which is parallel to \p lhs. 
		static constexpr Vector Project(const Vector& lhs, const Vector& rhs) requires std::floating_point<T>
		{
			assert(lhs.IsUnit());
			return DotProduct(lhs, rhs) * lhs;
		}

		// Static to better emphasise the usage of the unit vector.
		/// Calculates the vector from \p Project(lhs, \p b) to \p rhs. \n
		///	Consider a right-angled triangle made up of vectors where the hypotenuse is \p rhs,
		///	the adjacent side \p Project(lhs, \p rhs), and the opposite side is \p ProjectPerpendicular(lhs, \p rhs).
		/// @param lhs Unit vector that determines the direction of the line that \p rhs will be projected upon.
		/// @param rhs Vector to project.
		/// @return The vector of \p rhs, which is parallel to \p lhs. 
		static constexpr Vector ProjectPerpendicular(const Vector& lhs, const Vector& rhs) requires std::floating_point<
			T>
		{
			assert(lhs.IsUnit());
			return rhs - (DotProduct(lhs, rhs) * lhs);
		}

		static constexpr bool IsPerpendicular(const Vector& lhs, const Vector& rhs)
		{
			return AlmostEquals<T>(0, Vector::DotProduct(lhs, rhs));
		}

		static constexpr bool IsParallel(const Vector& lhs, const Vector& rhs)
		{
			return Vector::CrossProduct(lhs, rhs).IsZeroVector();
		}

		/* Methods */
		constexpr bool IsZeroVector()
		{
			return std::all_of(this->begin(), this->end(), [](T value) { return AlmostEquals<T>(0, value); });
		}

		/// A commutative function that sums the products of each component in two vectors.
		/// @return A negative scalar value when the vector points towards the vector \p rhs. \n
		/// A zero value when the vectors are perpendicular, or when one is a zero vector.
		constexpr T DotProduct(const Vector& rhs) const { return DotProduct(*this, rhs); }

		/// An anti-commutative function, \p lhs x \p rhs, that can only be applied to 3D vectors,
		/// yielding a 3D vector perpendicular to the original two.
		///	@param rhs \p rhs in \p lhs x \p rhs.
		/// @return Zero vector if \p lhs or \p rhs are parallel, of if either are the zero vector. \n
		/// Otherwise, a vector perpendicular to both vectors.
		constexpr Vector CrossProduct(const Vector& rhs) const requires (Dimensions == 3)
		{
			return CrossProduct(*this, rhs);
		}

		/// Calculates the squared length of the vector by applying the dot product to itself.\n
		/// This is useful for when relative comparisons are necessary and avoids a costly square root.
		/// @return The squared length of the vector.
		constexpr T LengthSquared() const { return DotProduct(*this); }

		/// Calculates the length of a vector by applying Pythagoras' theorem.
		/// @return Magnitude of the vector. The type is determined by the overload of std::sqrt.
		constexpr auto Length() const { return std::sqrt(LengthSquared()); }

		/// Normalises a vector by dividing each of its components by the vector's magnitude.
		void Normalise() requires std::floating_point<T>
		{
			auto scale = 1 / Length();
			*this *= scale;
		}

		/// Normalises a vector by dividing each of its components by the vector's magnitude.
		/// @return A normalised copy of the vector.
		[[nodiscard]] constexpr Vector Normalised() const requires std::floating_point<T>
		// [[nodiscard]] to help emphasise this is not the same as Normalise().
		{
			Vector copy = *this;
			copy.Normalise();

			return copy;
		}

		/// Calculates the squared distance between two vectors.
		/// @return The squared magnitude of the vector \p rhs - \p (*this).
		constexpr T DistanceSquared(const Vector& rhs) { return DistanceSquared(*this, rhs); }

		/// Calculates the distance between two vectors.
		/// @return The magnitude of the vector \p rhs - \p (*this).
		constexpr auto Distance(const Vector& rhs) { return Distance(*this, rhs); }

		/* Getters and Setters */
		constexpr T X() const requires (Dimensions <= 4) { return (*this)[0]; }

		void X(T value) requires (Dimensions <= 4) { (*this)[0] = value; }

		constexpr T Y() const requires (Dimensions <= 4 && Dimensions > 1) { return (*this)[1]; }

		void Y(T value) requires (Dimensions <= 4 && Dimensions > 1) { (*this)[1] = value; }

		constexpr T Z() const requires (Dimensions <= 4 && Dimensions > 2) { return (*this)[2]; }

		void Z(T value) requires (Dimensions <= 4 && Dimensions > 2) { (*this)[2] = value; }

		constexpr T W() const requires (Dimensions <= 4 && Dimensions > 3) { return (*this)[3]; }

		void W(T value) requires (Dimensions <= 4 && Dimensions > 3) { (*this)[3] = value; }

		constexpr bool IsUnit() const
		{
			// It is not necessary to check Length(), because if it is a unit vector then LengthSquared() is the same value.
			return AlmostEquals<T>(1, LengthSquared());
		}

		/* Operator Overloading */
		/// @return A negated copy of the vector.
		Vector operator-() const
		{
			Vector negated;
			for (size_t i = 0; i < Dimensions; ++i) { negated[i] = -(*this)[i]; }

			return negated;
		}

		/// Adds each corresponding element in two vectors together.
		constexpr Vector& operator+=(const Vector& rhs)
		{
			for (size_t i = 0; i < Dimensions; ++i) { (*this)[i] += rhs[i]; }
			return *this;
		}

		/// Adds each corresponding element in two vectors together.
		constexpr friend Vector operator+(Vector lhs, const Vector& rhs) { return lhs += rhs; }

		/// Subtracts each component in the vector by the corresponding component in \p rhs.
		constexpr Vector& operator-=(const Vector& rhs)
		{
			for (size_t i = 0; i < Dimensions; ++i) { (*this)[i] -= rhs[i]; }
			return *this;
		}

		/// Subtracts each component in \p lhs by the corresponding component in \p rhs.
		constexpr friend Vector operator-(Vector lhs, const Vector& rhs) { return lhs -= rhs; }

		/// Multiplies each element in the vector by a scalar.
		/// @tparam U Arithmetic type of the scalar.
		/// @param rhs Scalar value.
		/// @return A reference to the altered vector.
		constexpr Vector& operator*=(T rhs)
		{
			for (size_t i = 0; i < Dimensions; ++i) { (*this)[i] *= rhs; }
			return *this;
		}

		/// Multiplies each element in a vector by a scalar.
		/// @param lhs Vector that's elements will be multiplied.
		/// @param rhs Scalar value by which to multiply each element.
		/// @return A copy of the passed vector, multiplied by the scalar.
		constexpr friend Vector operator*(Vector lhs, T rhs) { return lhs *= rhs; }

		// Intentionally copied parameter to re-use already implemented operator.
		/// Multiplies each element in a vector by a scalar.
		/// @param lhs Scalar value by which to multiply each element.
		/// @param rhs Vector that's elements will be multiplied.
		/// @return A copy of the passed vector, multiplied by the scalar.
		friend Vector operator*(T lhs, const Vector& rhs) { return rhs * lhs; }

		/// Divides each element in the vector by a scalar.
		/// @param rhs Scalar value.
		/// @return A reference to the altered vector.
		constexpr Vector& operator/=(T rhs)
		{
			for (size_t i = 0; i < Dimensions; ++i) { (*this)[i] /= rhs; }
			return *this;
		}

		// Intentionally copied parameter to re-use already implemented operator.
		/// Divides each element in a vector by a scalar.
		/// @param lhs Vector that's elements will be divided.
		/// @param rhs Scalar value by which to multiply each element.
		/// @return A copy of the passed vector, divided by the scalar.
		constexpr friend Vector operator/(Vector lhs, T rhs) { return lhs /= rhs; }

		friend bool operator==(const Vector& lhs, const Vector& rhs) = default;

		friend bool operator<(const Vector& lhs, const Vector& rhs)
		{
			return lhs.LengthSquared() < rhs.LengthSquared();
		}

		friend bool operator!=(const Vector& lhs, const Vector& rhs) { return !(lhs == rhs); }

		friend bool operator<=(const Vector& lhs, const Vector& rhs) { return !(rhs < lhs); }

		friend bool operator>(const Vector& lhs, const Vector& rhs) { return rhs < lhs; }

		friend bool operator>=(const Vector& lhs, const Vector& rhs) { return !(lhs < rhs); }

		// If these aren't deleted it makes it possible to compare to a plain array of the same type and size.
		friend auto operator==(const std::array<T, Dimensions>& lhs, const std::array<T, Dimensions>& rhs) = delete;

		friend auto operator<=>(const std::array<T, Dimensions>& lhs, const std::array<T, Dimensions>& rhs) = delete;
	};
}
