#pragma once
#include "Vector.h"
#include <algorithm>
#include <array>
#include <concepts>
#include <cstddef>
#include <numeric>

namespace Engine3
{
	// Row first, then column to follow normal matrix conventions.
	/// Matrix with its elements stored in row-major order.
	/// @tparam RowSize The vertical size of the matrix.
	/// @tparam ColumnSize The horizontal size of the matrix.
	/// @tparam T The type of each element stored in the matrix.
	template <std::size_t RowSize, std::size_t ColumnSize = RowSize, typename T = float>
		requires std::integral<T> || std::floating_point<T>
	struct Matrix : std::array<T, RowSize * ColumnSize>
	{
		static constexpr Matrix IdentityMatrix() requires (RowSize == ColumnSize)
		{
			Matrix identityMatrix{};
			for (std::size_t i = 0; i < RowSize; ++i) { identityMatrix[(RowSize + 1) * i] = 1; }

			return identityMatrix;
		}

		/* Methods */
		void Transpose() requires (RowSize == ColumnSize) { (*this) = this->Transposed(); }

		Matrix<ColumnSize, RowSize, T> Transposed()
		{
			// Non-Square matrices when transposed will return a matrix that's aspect ratio is flipped.
			Matrix<ColumnSize, RowSize, T> matrix;
			for (std::size_t row = 0; row < RowSize; ++row)
			{
				for (std::size_t column = 0; column < ColumnSize; ++column)
				{
					matrix(column, row) = (*this)(row, column);
				}
			}

			return matrix;
		}

		constexpr Vector<ColumnSize> GetRow(std::size_t row) const
		{
			Vector<ColumnSize, T> vector;
			for (std::size_t column = 0; column < ColumnSize; ++column) { vector[column] = (*this)(row, column); }
			return vector;
		}

		constexpr Vector<RowSize> GetColumn(std::size_t column) const
		{
			Vector<RowSize, T> vector;
			for (std::size_t i = 0; i < RowSize; ++i) { vector[i] = (*this)(i, column); }
			return vector;
		}

		/* Operators */
		T& operator()(std::size_t row, std::size_t column) { return (*this)[ColumnSize * row + column]; }

		const T& operator()(std::size_t row, std::size_t column) const { return (*this)[ColumnSize * row + column]; }

		// Scalar multiplications.
		constexpr Matrix& operator*=(T other)
		{
			for (std::size_t i = 0; i < this->size(); ++i) { (*this)[i] *= other; }
			return *this;
		}

		constexpr friend Matrix operator*(Matrix a, T b) { return a *= b; } // Intentional Matrix copy.

		friend Matrix operator*(T left, const Matrix& right) { return right * left; }

		constexpr Matrix& operator/=(T right)
		{
			for (std::size_t i = 0; i < this->size(); ++i) { (*this)[i] /= right; }
			return *this;
		}

		constexpr friend Matrix operator/(Matrix left, T right) { return left /= right; } // Intentional Matrix copy.

		/// Matrix multiplication. \n
		/// Takes the dot product of each row in \p lhs by each column in \p rhs to create a new matrix.
		///
		/// Subsequently, matrix multiplication can only occur when the columns in \p lhs
		/// match the number of rows in \p rhs.
		template <std::size_t OtherColumnSize>
		constexpr Matrix<RowSize, OtherColumnSize, T> operator*(const Matrix<ColumnSize, OtherColumnSize, T>& rhs)
		{
			Matrix<RowSize, OtherColumnSize> result;
			for (std::size_t row = 0; row < RowSize; ++row)
			{
				for (std::size_t column = 0; column < OtherColumnSize; ++column)
				{
					// TODO: Would a for loop be more performant with a single temporary value rather than constructing
					// two vectors each time?
					// ColumnSize and OtherRowSize are equivalent, so either would work for the dimensions of the vector.
					// Additionally, dot product should work fine no matter T, as long as different typed matrices aren't
					// mixed.
					result(row, column) =
						Vector<ColumnSize, T>::DotProduct(
							this->GetRow(row), rhs.GetColumn(column)
						);
				}
			}

			return result;
		}

		/// Multiplies a row vector by a matrix.
		/// @param lhs A row vector with a number of columns equal to the number of rows in \p rhs.
		/// @param rhs A matrix with a number of rows equal to the size of \p lhs.
		/// @return A row vector with a size equal to the number of columns in \p rhs.
		constexpr friend Vector<ColumnSize> operator*(const Vector<RowSize>& lhs, const Matrix& rhs)
		{
			Vector<ColumnSize> rowVector;
			for (std::size_t column = 0; column < rowVector.size(); ++column)
			{
				rowVector[column] = Vector<RowSize>::DotProduct(lhs, rhs.GetColumn(column));
			}

			return rowVector;
		}

		/// Multiplies a column vector by a matrix.
		/// @param lhs A matrix with a number of columns equal to the size of \p rhs.
		/// @param rhs A column vector with a number of rows equal to the number of columns in \p lhs.
		/// @return A column vector with a size equal to the number of rows in \p lhs.
		constexpr friend Vector<RowSize> operator*(const Matrix& lhs, const Vector<ColumnSize>& rhs)
		{
			Vector<RowSize> columnVector;

			for (std::size_t row = 0; row < columnVector.size(); ++row)
			{
				columnVector[row] = Vector<ColumnSize>::DotProduct(lhs.GetRow(row), rhs);
			}

			return columnVector;
		}

		friend bool operator==(const Matrix& lhs, const Matrix& rhs) = default;

		friend bool operator!=(const Matrix& lhs, const Matrix& rhs) = default;

		// If these aren't deleted it makes it possible to perform comparisons like Matrix<4, 3>{} == Matrix<3, 4>{},
		// as well as to arrays of the same type and size.
		// auto val = Matrix<4, 3>{} == Matrix<3, 4>{}; // Shouldn't compile
		// auto val1 = Matrix<4, 3>{} == std::array<float, 12>{}; //  Shouldn't compile
		// auto val2 = Vector<3>{} == std::array<float, 3>{}; // Shouldn't compile.
		// auto val3 = std::array<float, 3>{} == std::array<float, 3>{}; // Should compule.
		// auto val4 = std::array<float, 12>{} == Matrix<4, 3>{}; //  Shouldn't compile

		auto operator==(const std::array<T, RowSize * ColumnSize>& rhs) = delete;

		auto operator<=>(const std::array<T, RowSize * ColumnSize>& rhs) = delete;
	};
}
