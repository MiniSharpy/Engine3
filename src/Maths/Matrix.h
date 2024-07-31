#pragma once
#include <array>
#include <cassert>

namespace Engine3
{
	/// Wraps an std::array, storing the matrix values in row-major order.
	///	TODO: Could add a parameter to allow changing between row-major order and column major.
	template <size_t Dimensions>
	class Matrix
	{
		/* STATIC */
	public:
		/// <summary>
		/// Any matrix multiplied by the identity matrix returns the same value.
		/// </summary>
		/// <returns>A zeroed matrix, except for its diagonals which have the value 1.</returns>
		constexpr static Matrix IdentityMatrix()
		{
			Matrix identityMatrix;
			for (int i = 0; i < Dimensions; ++i) { identityMatrix[(Dimensions + 1) * i] = 1; }

			return identityMatrix;
		}

		/// <summary>
		/// An identity matrix where the right most column is filled from top to bottom with the passed values.
		/// </summary>
		constexpr static Matrix Translation(float x, float y, float z) requires (Dimensions == 4)
		{
			Matrix translation = IdentityMatrix();
			translation(0, 3) = x;
			translation(1, 3) = y;
			translation(2, 3) = z;

			return translation;
		}

		/// <summary>
		/// An identity matrix where the diagonal elements of the matrix are filled with the passed values.
		/// </summary>
		constexpr static Matrix Scaling(float x, float y, float z) requires (Dimensions == 4)
		{
			Matrix scale = IdentityMatrix();
			scale(0, 0) = x;
			scale(1, 1) = y;
			scale(2, 2) = z;

			return scale;
		}

		constexpr static Matrix RotationX(float radians) requires (Dimensions == 4)
		{
			Matrix rotate = IdentityMatrix();
			rotate(1, 1) = cosf(radians);
			rotate(2, 1) = sinf(radians);
			rotate(1, 2) = -sinf(radians);
			rotate(2, 2) = cosf(radians);

			return rotate;
		}

		constexpr static Matrix RotationY(float radians) requires (Dimensions == 4)
		{
			Matrix rotate = IdentityMatrix();
			rotate(0, 0) = cosf(radians);
			rotate(0, 2) = sinf(radians);
			rotate(2, 0) = -sinf(radians);
			rotate(2, 2) = cosf(radians);

			return rotate;
		}

		constexpr static Matrix RotationZ(float radians) requires (Dimensions == 4)
		{
			Matrix rotate = IdentityMatrix();
			rotate(0, 0) = cosf(radians);
			rotate(0, 1) = -sinf(radians);
			rotate(1, 0) = sinf(radians);
			rotate(1, 1) = cosf(radians);
			return rotate;
		}

		constexpr static Matrix Shearing(float xy, float xz, float yx, float yz, float zx, float zy) requires (
			Dimensions == 4)
		{
			Matrix shear = IdentityMatrix();
			shear(0, 1) = xy;
			shear(0, 2) = xz;
			shear(1, 0) = yx;
			shear(1, 2) = yz;
			shear(2, 0) = zx;
			shear(2, 1) = zy;

			return shear;
		}

		/* NON-STATIC */
	private:
		std::array<float, Dimensions * Dimensions> Values;

	public:
		// Allow uninitialised internal values.
		Matrix() = default;

		// Don't allow some uninitialised values. Either all of it, or none of it.
		template <typename... Args>
			requires(sizeof...(Args) == Dimensions * Dimensions)
		Matrix(Args&&... args) : Values(std::forward<Args>(args)...) {}

		/// <summary>
		///	For every row in left hand side matrix:
		/// Multiply each element in a left hand side column
		/// by the corresponding element in a right hand side row,
		/// adding the products together to form a single element in the new matrix
		/// corresponding to the respective row and column. \n
		/// Each element of the final matrix is a dot product of a row-column
		/// combination.
		/// </summary>
		constexpr Matrix operator*(const Matrix& rhs) const
		{
			Matrix result{};

			for (int row = 0; row < Dimensions; ++row)
			{
				for (int column = 0; column < Dimensions; ++column)
				{
					for (int i = 0; i < Dimensions; ++i) { result(row, column) += (*this)(row, i) * rhs(i, column); }
				}
			}

			return result;
		}

		/// <summary>
		/// When multiplying a translation by a vector, due to the vector's W component
		///	the translation values of the matrix are ignored. This is because a vector is
		///	a direction, moving it in space does not change the direction it points.
		/// </summary>
		//constexpr Tuple operator*(const Tuple &rhs) const requires(Dimensions == 4)
		//{
		//	Tuple result{};
		//	for (int row = 0; row < 4; ++row)
		//	{
		//		for (int i = 0; i < 4; ++i) { result[row] += (*this)(row, i) * rhs[i]; }
		//	}

		//	return result;
		//}

		bool operator==(const Matrix& rhs) const
		{
			bool isAlmostEqual = true;
			for (int i = 0; i < Dimensions * Dimensions; ++i) { isAlmostEqual &= AlmostEquals((*this)[i], rhs[i]); }

			return isAlmostEqual;
		}

		bool operator!=(const Matrix& rhs) const { return !(*this == rhs); }

		// TODO: Not supported by MSVC yet.
		// Multidimensional subscript operator:
		// https://en.cppreference.com/w/cpp/23
		//float& operator[](std::size_t row, std::size_t column) { return Values[Dimensions * column + row]; }

		//constexpr float& operator[](std::size_t row, std::size_t column) const
		//{
		//	return Values[Dimensions * column + row];
		//}

		float& operator()(size_t row, size_t column) { return Values[Dimensions * row + column]; }

		const float& operator()(size_t row, size_t column) const { return Values[Dimensions * row + column]; }

		// These operators could cause problems if the row-major order is not kept.
		//float& operator[](std::size_t index) { return Values[index]; }

		//constexpr float& operator[](std::size_t index) const { return Values[index]; }

		Matrix Transposed()
		{
			Matrix transposed;
			for (int row = 0; row < Dimensions; ++row)
			{
				for (int column = 0; column < Dimensions; ++column) { transposed(row, column) = (*this)(column, row); }
			}

			return transposed;
		}

		float Determinant() const requires(Dimensions == 2)
		{
			return (Values[0] * Values[3]) - (Values[1] * Values[2]);
		}

		float Determinant() const requires(Dimensions > 2)
		{
			// Doesn't matter which row or column is picked.
			float determinant = 0;
			for (int column = 0; column < Dimensions; ++column) { determinant += Values[column] * Cofactor(0, column); }

			return determinant;
		}

		Matrix<Dimensions - 1> Submatrix(size_t rowToRemove, size_t columnToRemove) const
		{
			Matrix<Dimensions - 1> submatrix;

			for (int row = 0; row < Dimensions - 1; ++row)
			{
				for (int column = 0; column < Dimensions - 1; ++column)
				{
					size_t rowOffset = row >= rowToRemove ? 1 : 0;
					size_t columnOffset = column >= columnToRemove ? 1 : 0;
					submatrix(row, column) = (*this)
					(
						row + rowOffset,
						column + columnOffset
					);
				}
			}

			return submatrix;
		}

		float Minor(size_t rowToRemove, size_t columnToRemove) const
		{
			return Submatrix(rowToRemove, columnToRemove).Determinant();
		}

		float Cofactor(size_t row, size_t column) const
		{
			float minor = Minor(row, column);
			bool isOdd = (row + column) % 2;
			return isOdd ? -minor : minor;
		}

		Matrix Inverted() const
		{
			assert(Determinant() != 0); // Can't divide by zero.
			float determinant = Determinant();

			Matrix inverted;
			for (int row = 0; row < Dimensions; ++row)
			{
				for (int column = 0; column < Dimensions; ++column)
				{
					float cofactor = Cofactor(row, column);
					// Flipped row, column to also handle transposing.
					inverted(column, row) = cofactor / determinant;
				}
			}

			return inverted;
		}

		Matrix& Translate(float x, float y, float z) requires (Dimensions == 4)
		{
			*this = Translation(x, y, z) * (*this);
			return *this;
		}

		Matrix& RotateX(float radians) requires (Dimensions == 4)
		{
			*this = RotationX(radians) * (*this);
			return *this;
		}

		Matrix& Scale(float x, float y, float z) requires (Dimensions == 4)
		{
			*this = Scaling(x, y, z) * (*this);
			return *this;
		}

		Matrix& RotateY(float radians) requires (Dimensions == 4)
		{
			*this = RotationY(radians) * (*this);
			return *this;
		}

		Matrix& RotateZ(float radians) requires (Dimensions == 4)
		{
			*this = RotationZ(radians) * (*this);
			return *this;
		}

		Matrix& Shear(float xy, float xz, float yx, float yz, float zx, float zy) requires (Dimensions == 4)
		{
			*this = Shearing(xy, xz, yx, yz, zx, zy) * (*this);
			return *this;
		}

		/* ALTERED COPIES */
		/// Returns a translated copy of the matrix.
		Matrix Translated(float x, float y, float z) requires (Dimensions == 4)
		{
			return Translation(x, y, z) * (*this);
		}

		/// Returns a scaled copy of the matrix.
		Matrix Scaled(float x, float y, float z) requires (Dimensions == 4) { return Scaling(x, y, z) * (*this); }

		/// Returns a copy of the matrix rotated along the X.
		Matrix RotatedX(float radians) requires (Dimensions == 4) { return RotationX(radians) * (*this); }

		/// Returns a copy of the matrix rotated along the Y.
		Matrix RotatedY(float radians) requires (Dimensions == 4) { return RotationY(radians) * (*this); }

		/// Returns a copy of the matrix rotated along the Z.
		Matrix RotatedZ(float radians) requires (Dimensions == 4) { return RotationZ(radians) * (*this); }

		/// Returns a sheared copy of the matrix.
		Matrix Sheared(float xy, float xz, float yx, float yz, float zx, float zy) requires (Dimensions == 4)
		{
			return Shearing(xy, xz, yx, yz, zx, zy) * (*this);
		}

		/* STD::ARRAY WRAPPING */
		typename decltype(Values)::iterator begin() { return Values.begin(); }

		typename decltype(Values)::const_iterator begin() const { return Values.begin(); }

		typename decltype(Values)::iterator end() { return Values.end(); }

		typename decltype(Values)::const_iterator end() const { return Values.end(); }

		float* data() { return Values.data(); }

		const float* data() const { return Values.data(); }
	};
}
