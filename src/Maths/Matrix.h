#pragma once
#include <algorithm>
#include <array>
#include <concepts>
#include <cstddef>

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

		/* Operators */
		T& operator()(size_t row, size_t column) { return (*this)[ColumnSize * row + column]; }

		const T& operator()(size_t row, size_t column) const { return (*this)[ColumnSize * row + column]; }

		constexpr Matrix& operator*=(T other)
		{
			for (size_t i = 0; i < this->size(); ++i) { (*this)[i] *= other; }
			return *this;
		}

		constexpr friend Matrix operator*(Matrix a, T b) { return a *= b; }

		friend Matrix operator*(T left, const Matrix& right) { return right * left; }

		constexpr Matrix& operator/=(T right)
		{
			for (size_t i = 0; i < this->size(); ++i) { (*this)[i] /= right; }
			return *this;
		}

		constexpr friend Matrix operator/(Matrix left, T right) { return left /= right; }

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
