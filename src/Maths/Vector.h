#pragma once
#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <concepts>

namespace Engine3
{
	template <std::size_t Dimensions, typename T = float>
		requires std::integral<T> || std::floating_point<T>
	class Vector
	{
	public:
		static constexpr Vector Zero() { return {}; }

		static constexpr Vector Forward() requires (Dimensions == 3)
		{
			return {static_cast<T>(0), static_cast<T>(0), static_cast<T>(1)};
		}

		static constexpr Vector Back() requires (Dimensions == 3) { return {-Vector::Forward()}; }

		static constexpr Vector Right() requires (Dimensions == 3)
		{
			return {static_cast<T>(1), static_cast<T>(0), static_cast<T>(0)};
		}

		static constexpr Vector Up() requires (Dimensions == 3)
		{
			return {static_cast<T>(0), static_cast<T>(1), static_cast<T>(0)};
		}

		static constexpr Vector Right() requires (Dimensions == 2) { return {static_cast<T>(1), static_cast<T>(0)}; }

		static constexpr Vector Up() requires(Dimensions == 2) { return {static_cast<T>(0), static_cast<T>(1)}; }

		static constexpr Vector Left() requires (Dimensions == 2 || Dimensions == 3) { return {-Vector::Right()}; }

		static constexpr Vector Down() requires (Dimensions == 2 || Dimensions == 3) { return {-Vector::Up()}; }

	private:
		std::array<T, Dimensions> Values_;

	public:
		/* Constructors and Move/Copy Operations */
		/// Constructs an uninitialized vector.
		constexpr Vector() = default;

		~Vector() = default;

		/// Constructs each element in the vector to a value.
		/// @tparam Args Type of each parameter, implicitly converted in aggregate initialization of the underlying std::array.
		/// @param args Value of each element in the vector.
		template <typename... Args>
			requires(sizeof...(Args) == Dimensions) // Require all parameters specified.
		constexpr Vector(Args&&... args) : Values_{std::forward<Args>(args)...} {}

		Vector(const Vector& other) = default;

		Vector(Vector&& other) noexcept = default;

		Vector& operator=(const Vector& other) = default;

		Vector& operator=(Vector&& other) noexcept = default;

		/* Vector Methods */
		/// A commutative function that sums the products of each component in \p a and \p b.
		/// @return A negative scalar value when \p a points towards \p b. \n
		/// A zero value when the vectors are perpendicular, or when one is a zero vector.
		static constexpr T DotProduct(const Vector& a, const Vector& b)
		{
			// An integer squared is always an integer, no need to worry about precision loss when std::integral<T>,
			// just need to allow other typed vectors as a parameter.
			T value = 0;
			for (size_t i = 0; i < Dimensions; ++i) { value += a[i] * b[i]; }

			return value;
		}

		/// A commutative function that sums the products of each component in two vectors.
		/// @return A negative scalar value when the vector points towards the vector \p other. \n
		/// A zero value when the vectors are perpendicular, or when one is a zero vector.
		constexpr T DotProduct(const Vector& other) const { return DotProduct(*this, other); }

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
		[[nodiscard]] Vector Normalised() const requires std::floating_point<T>
		// [[nodiscard]] to help emphasise this is not the same as Normalise().
		{
			Vector copy = *this;
			copy.Normalise();

			return copy;
		}

		/// Calculates the squared distance between two vectors.
		/// @return The squared magnitude of the vector \p b - \p a.
		static constexpr T DistanceSquared(const Vector& a, const Vector& b) { return (b - a).LengthSquared(); }

		/// Calculates the distance between two vectors.
		/// @return The magnitude of the vector \p b - \p a.
		static constexpr auto Distance(const Vector& a, const Vector& b) { return (b - a).Length(); }

		/// Calculates the squared distance between two vectors.
		/// @return The squared magnitude of the vector \p other - \p this.
		constexpr T DistanceSquared(const Vector& other) { return DistanceSquared(*this, other); }

		/// Calculates the distance between two vectors.
		/// @return The magnitude of the vector \p other - \p this.
		constexpr auto Distance(const Vector& other) { return Distance(*this, other); }

		// Strictly speaking only other needs to be a floating point, not this. Could
		// template the type of other to require floating point instead, but then I'd
		// need to deal with converting the result of DotProduct.
		/// Projects a vector onto an infinitely long line. \n
		///	Consider a right-angled triangle made up of vectors where the hypotenuse is \p b, the adjacent side \p Project(a, \p b), and the opposite side is \p ProjectPerpendicular(a, \p b).
		/// @param a Unit vector that determines the direction of the line that \p b will be projected upon.
		/// @param b Vector to project.
		/// @return The projection of \p b onto an infinitely long line which is parallel to \p a. 
		static constexpr Vector Project(const Vector& a, const Vector& b) requires std::floating_point<T>
		{
			assert(a.IsUnit());
			return DotProduct(a, b) * a;
		}

		/// Calculates the vector from \p Project(a, \p b) to \p b. \n
		///	Consider a right-angled triangle made up of vectors where the hypotenuse is \p b, the adjacent side \p Project(a, \p b), and the opposite side is \p ProjectPerpendicular(a, \p b).
		/// @param a Unit vector that determines the direction of the line that \p b will be projected upon.
		/// @param b Vector to project.
		/// @return The vector of \p b, which is parallel to \p a. 
		static constexpr Vector ProjectPerpendicular(const Vector& a, const Vector& b) requires std::floating_point<T>
		{
			assert(a.IsUnit());
			return b - (DotProduct(a, b) * a);
		}

		/* Getters and Setters */
		T X() requires (Dimensions <= 4) { return Values_[0]; }

		constexpr T X() const requires (Dimensions <= 4) { return Values_[0]; }

		void X(T value) requires (Dimensions <= 4) { Values_[0] = value; }

		T Y() requires (Dimensions <= 4 && Dimensions > 1) { return Values_[1]; }

		constexpr T Y() const requires (Dimensions <= 4 && Dimensions > 1) { return Values_[1]; }

		void Y(T value) requires (Dimensions <= 4 && Dimensions > 1) { Values_[1] = value; }

		T Z() requires (Dimensions <= 4 && Dimensions > 2) { return Values_[2]; }

		constexpr T Z() const requires (Dimensions <= 4 && Dimensions > 2) { return Values_[2]; }

		void Z(T value) requires (Dimensions <= 4 && Dimensions > 2) { Values_[2] = value; }

		T W() requires (Dimensions <= 4 && Dimensions > 3) { return Values_[3]; }

		constexpr T W() const requires (Dimensions <= 4 && Dimensions > 3) { return Values_[3]; }

		void W(T value) requires (Dimensions <= 4 && Dimensions > 3) { Values_[3] = value; }

		constexpr bool IsUnit() const { return std::abs(1 - LengthSquared()) < std::numeric_limits<T>::epsilon(); }

		/* Operator Overloading */
		T& operator[](std::size_t index) { return Values_[index]; }

		constexpr T operator[](std::size_t index) const { return Values_[index]; }

		/// @return A negated copy of the vector.
		Vector operator-() const
		{
			Vector negated;
			for (size_t i = 0; i < Dimensions; ++i) { negated[i] = -(*this)[i]; }

			return negated;
		}

		constexpr Vector& operator+=(const Vector& right)
		{
			for (size_t i = 0; i < Dimensions; ++i) { Values_[i] += right[i]; }
			return *this;
		}

		constexpr friend Vector operator+(Vector left, const Vector& right) { return left += right; }

		constexpr Vector& operator-=(const Vector& right)
		{
			for (size_t i = 0; i < Dimensions; ++i) { Values_[i] -= right[i]; }
			return *this;
		}

		constexpr friend Vector operator-(Vector left, const Vector& right) { return left -= right; }

		/// Multiplies each element in the vector by a scalar.
		/// @tparam U Arithmetic type of the scalar.
		/// @param right Scalar value.
		/// @return A reference to the altered vector.
		constexpr Vector& operator*=(T right)
		{
			for (size_t i = 0; i < Dimensions; ++i) { Values_[i] *= right; }
			return *this;
		}

		/// Multiplies each element in a vector by a scalar.
		/// @param left Vector that's elements will be multiplied.
		/// @param right Scalar value by which to multiply each element.
		/// @return A copy of the passed vector, multiplied by the scalar.
		constexpr friend Vector operator*(Vector left, T right) { return left *= right; }

		/// Multiplies each element in a vector by a scalar.
		/// @param left Scalar value by which to multiply each element.
		/// @param right Vector that's elements will be multiplied.
		/// @return A copy of the passed vector, multiplied by the scalar.
		friend Vector operator*(T left, const Vector& right) { return right * left; }

		/// Divides each element in the vector by a scalar.
		/// @param right Scalar value.
		/// @return A reference to the altered vector.
		constexpr Vector& operator/=(T right)
		{
			for (size_t i = 0; i < Dimensions; ++i) { Values_[i] /= right; }
			return *this;
		}

		/// Divides each element in a vector by a scalar.
		/// @param left Vector that's elements will be divided.
		/// @param right Scalar value by which to multiply each element.
		/// @return A copy of the passed vector, divided by the scalar.
		constexpr friend Vector operator/(Vector left, T right) { return left /= right; }

		friend bool operator==(const Vector& lhs, const Vector& rhs) { return lhs.Values_ == rhs.Values_; }

		friend bool operator!=(const Vector& lhs, const Vector& rhs) { return !(lhs == rhs); }

		friend bool operator<(const Vector& lhs, const Vector& rhs)
		{
			return lhs.LengthSquared() < rhs.LengthSquared();
		}

		friend bool operator<=(const Vector& lhs, const Vector& rhs) { return !(rhs < lhs); }

		friend bool operator>(const Vector& lhs, const Vector& rhs) { return rhs < lhs; }

		friend bool operator>=(const Vector& lhs, const Vector& rhs) { return !(lhs < rhs); }
	};
}
