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
			for (size_t i = 0; i < RowSize; ++i) { identityMatrix[(RowSize + 1) * i] = 1; }

			return identityMatrix;
		}

		T& operator()(size_t row, size_t column) { return (*this)[ColumnSize * row + column]; }

		const T& operator()(size_t row, size_t column) const { return (*this)[ColumnSize * row + column]; }

		friend bool operator==(const Matrix& lhs, const Matrix& rhs) = default;

		friend bool operator!=(const Matrix& lhs, const Matrix& rhs) = default;

		// If these aren't deleted it makes it possible to comparisons like Matrix<4, 3>{} == Matrix<3, 4>{}.
		friend auto operator==(const std::array<T, RowSize * ColumnSize>& lhs,
		                       const std::array<T, RowSize * ColumnSize>& rhs) = delete;

		friend auto operator<=>(const std::array<T, RowSize * ColumnSize>& lhs,
		                        const std::array<T, RowSize * ColumnSize>& rhs) = delete;
	};
}
