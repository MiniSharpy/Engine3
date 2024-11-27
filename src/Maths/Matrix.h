#pragma once
#include "Maths.h"
#include "Vector.h"
#include <algorithm>
#include <array>
#include <concepts>
#include <cstddef>
#include <print>
#include <utility>

namespace Engine3
{
	/// Calls the passed function, passing the current index as a parameter.
	///	Indexing starts from 0, and counts up to \p Count - 1.
	/// @tparam T The type of the indices.
	/// @tparam Count The number of indices.
	/// @param function The function to be called, passing the current index.
	template <std::integral T, T Count, class Function>
	constexpr void UnrollLoop(Function&& function)
	{
		// https://stackoverflow.com/a/46873787
		auto unrollIntegerSequence = []
			<T... Indices>
		(std::integer_sequence<T, Indices...>, Function&& function)
		{
			(function(std::integral_constant<T, Indices>{}), ...);
		};

		unrollIntegerSequence(std::make_integer_sequence<T, Count>{}, std::forward<Function>(function));
	}

	template <std::size_t RowSize, std::size_t ColumnSize, Number T>
	struct Matrix;

	namespace Detail
	{
		template <std::size_t RowSize, std::size_t ColumnSize, Number T>
		struct MatrixBase : std::array<T, RowSize * ColumnSize>
		{
		public:
			/*
			 * Static Methods
			 */
			/// @return A square matrix that's zeroed except for its diagonal elements, which are all one.
			static constexpr auto IdentityMatrix() requires (IsSquare<RowSize, ColumnSize>)
			{
				Matrix<RowSize, ColumnSize, T> identityMatrix{};
				for (std::size_t i = 0; i < RowSize; ++i) { identityMatrix[(RowSize + 1) * i] = 1; }

				return identityMatrix;
			}

			/*
			 * Methods
			 */
			/// @param row The row to remove. Indexed from 0.
			/// @param column The column to remove. Indexed from 0.
			/// @return The submatrix with \p row and \p column removed.
			constexpr Matrix<RowSize - 1, ColumnSize - 1, T> Submatrix(const std::size_t row,
			                                                           const std::size_t column) const
			{
				Matrix<RowSize - 1, ColumnSize - 1, T> submatrix;
				for (std::size_t currentRow = 0; currentRow < RowSize - 1; ++currentRow)
				{
					for (std::size_t currentColumn = 0; currentColumn < ColumnSize - 1; ++currentColumn)
					{
						const size_t rowOffset = currentRow >= row ? 1 : 0;
						const size_t columnOffset = currentColumn >= column ? 1 : 0;
						submatrix(currentRow, currentColumn) = (*this)(currentRow + rowOffset,
						                                               currentColumn + columnOffset);
					}
				}

				return submatrix;
			}

			constexpr T Minor(const std::size_t row, const std::size_t column) const
				requires IsSquare<RowSize, ColumnSize> { return Submatrix(row, column).Determinant(); }

			constexpr T Cofactor(const std::size_t row, const std::size_t column) const
				requires IsSquare<RowSize, ColumnSize>
			{
				// This could also be implemented by raising -1 to the power of (row + column)
				// and multiplying by the minor.
				T minor = Minor(row, column);
				bool isNegated = (row + column) & 1;
				return isNegated ? -minor : minor;
			}

			constexpr Matrix<RowSize, ColumnSize, T> CofactorMatrix() const requires IsSquare<RowSize, ColumnSize>
			{
				Matrix<RowSize, ColumnSize, T> cofactorMatrix;
				for (std::size_t row = 0; row < RowSize; ++row)
				{
					for (int column = 0; column < ColumnSize; ++column)
					{
						cofactorMatrix(row, column) = Cofactor(row, column);
					}
				}

				return cofactorMatrix;
			}

			constexpr T Determinant() const requires IsSquare<RowSize, ColumnSize>
			{
				// Doesn't matter whether column or row is iterated over.
				T determinant = 0;
				for (std::size_t column = 0; column < ColumnSize; ++column)
				{
					determinant += (*this)(0, column) * Cofactor(0, column);
				}

				return determinant;
			}

			/// Flips a square matrix diagonally in place.
			constexpr Matrix<RowSize, ColumnSize, T>& Transpose() requires (RowSize == ColumnSize)
			{
				// Effectively swapping rows with columns.
				// Could do in place, not sure if there would be any performance benefits besides memory usage.
				// https://en.wikipedia.org/wiki/In-place_matrix_transposition#Square_matrices
				*this = Transposed();

				return static_cast<Matrix<RowSize, ColumnSize, T>&>(*this);
			}

			/// Flips a matrix diagonally.
			[[nodiscard]] constexpr Matrix<ColumnSize, RowSize, T> Transposed()
			// [[nodiscard]] to help emphasise this is not the same as Transpose().
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

			constexpr Matrix<RowSize, ColumnSize, T> Adjoint() requires (IsSquare<RowSize, ColumnSize>)
			{
				return CofactorMatrix().Transpose();
			}

			constexpr bool IsInvertible() const requires (IsSquare<RowSize, ColumnSize>)
			{
				// A matrix is inverted by dividing its adjoint by the determinant.
				return !(Determinant() == 0);
			}

			constexpr Matrix<RowSize, ColumnSize, T> Inverted() requires (IsSquare<RowSize, ColumnSize>)
			{
				assert(IsInvertible());
				return Adjoint() / Determinant();
			}

			constexpr Matrix<RowSize, ColumnSize, T>& Invert() requires (IsSquare<RowSize, ColumnSize>)
			{
				assert(IsInvertible());
				*this = Inverted();

				return static_cast<Matrix<RowSize, ColumnSize, T>&>(*this);
			}

			constexpr bool IsOrthogonal() const requires (IsSquare<RowSize, ColumnSize>)
			{
				// If there's only one row/column we can guarantee it's not orthogonal.
				// constexpr if should mean the machine code is simplified without needing to
				// specialise.
				if constexpr (RowSize == 1) { return false; }

				// Performing "bitwise and" on an already true value will reduce the need for branching.
				bool isOrthogonal = true;

				// Need the first/previous row so it can be compared inside the loop.
				std::size_t row = 0;
				Vector<RowSize, T> previousRow = GetRow(row);
				isOrthogonal &= previousRow.IsUnit();

				// Check each row is unit, and compare to the previous to determine perpendicularly.
				// Could potentially exit early, but not sure if it's worth the branching in small matrices.
				for (row = 1; row < RowSize; ++row)
				{
					Vector<RowSize, T> currentRow = GetRow(row);
					isOrthogonal &= currentRow.IsUnit();
					isOrthogonal &= currentRow.IsPerpendicular(previousRow, currentRow);
					previousRow = currentRow;
				}

				return isOrthogonal;
			}

			/// Gram-Schmidt orthonormalisation to construct a set of orthonormal vectors given a finite, 
			/// linearly independent set of vectors.
			/// @return The orthogonal matrix.
			constexpr Matrix<RowSize, ColumnSize, T> Orthonormalised() const
				requires (IsSquare<RowSize, ColumnSize> && RowSize > 1) &&
				std::floating_point<T>
			{
				// TODO: Handle invalid cases, such as linearly dependent inputs.
				using Vector = Vector<RowSize, T>;

				Matrix<RowSize, ColumnSize, T> orthogonalised;
				orthogonalised.SetRow(0, GetRow(0).Normalise());

				for (int i = 1; i < RowSize; ++i)
				{
					const Vector currentRow = GetRow(i);

					// "...subtract off the portion of that vector that is parallel to the proceeding basis vectors."
					// - 3D Math Primer for Graphics and Game Development
					Vector newRow = currentRow;
					for (int j = 0; j < i; ++j)
					{
						const Vector previousRow = orthogonalised.GetRow(j);
						newRow -= Vector::Project(currentRow, previousRow);
					}

					orthogonalised.SetRow(i, newRow.Normalise());
				}

				return orthogonalised;
			}

			/*
			 * Getters / Setters
			 */
			/// @param row The row to return, indexed from zero/
			/// @return A row vector.
			constexpr Vector<ColumnSize, T> GetRow(std::size_t row) const
			{
				assert(row < RowSize);
				Vector<ColumnSize, T> vector;
				for (std::size_t column = 0; column < ColumnSize; ++column) { vector[column] = (*this)(row, column); }
				return vector;
			}

			constexpr void SetRow(std::size_t row, const Vector<RowSize, T>& values)
			{
				assert(row < RowSize);
				for (std::size_t column = 0; column < ColumnSize; ++column) { (*this)(row, column) = values[column]; }
			}

			/// @param column The column to return, indexed from zero.
			/// @return A column vector.
			constexpr Vector<RowSize, T> GetColumn(std::size_t column) const
			{
				assert(column < ColumnSize);
				Vector<RowSize, T> vector;
				for (std::size_t row = 0; row < RowSize; ++row) { vector[row] = (*this)(row, column); }
				return vector;
			}

			constexpr void SetColumn(std::size_t column, const Vector<ColumnSize, T>& values)
			{
				assert(column < ColumnSize);
				for (std::size_t row = 0; row < ColumnSize; ++row) { (*this)(column, row) = values[row]; }
			}

			/*
			 * Operators
			 */
			// I would like to use [row, column] for zero indexing and operator() for 1 indexed to follow matrix conventions,
			// but there's a lack of support for it in gcc and I prefer zero indexing.
			/// @param row The row to retrieve the element, indexed from zero.
			/// @param column The column to retrieve the element, indexed from zero.
			/// @return The element at \p row and \p column.
			constexpr T& operator()(std::size_t row, std::size_t column)
			{
				assert(row < RowSize);
				assert(column < ColumnSize);
				return (*this)[ColumnSize * row + column];
			}

			/// @param row The row to retrieve the element, indexed from zero.
			/// @param column The column to retrieve the element, indexed from zero.
			/// @return The element at \p row and \p column.
			constexpr const T& operator()(std::size_t row, std::size_t column) const
			{
				assert(row < RowSize);
				assert(column < ColumnSize);
				return (*this)[ColumnSize * row + column];
			}

			/*
			 * Scalar Maths Operators
			 */
			/// Multiplies each element in the matrix by the scalar value \p other.
			/// @return A reference to the now altered matrix for further operations.
			constexpr Matrix<RowSize, ColumnSize, T>& operator*=(T rhs)
			{
				for (T& element : *this) { element *= rhs; }
				return static_cast<Matrix<RowSize, ColumnSize, T>&>(*this);
			}

			/// Multiplies each element in the copied \p lhs by the scalar value \p rhs.
			// Intentional Matrix copy.
			constexpr friend auto operator*(Matrix<RowSize, ColumnSize, T> lhs, T rhs) { return lhs *= rhs; }

			/// Multiplies each element in \p right by the scalar value \p left.
			constexpr friend auto operator*(T lhs, const Matrix<RowSize, ColumnSize, T>& rhs) { return rhs * lhs; }

			/// Divides each element in the matrix by \p rhs in place.
			/// @return A reference to the now altered matrix for further operations.
			constexpr Matrix<RowSize, ColumnSize, T>& operator/=(T rhs)
			{
				for (T& element : *this) { element /= rhs; }
				return static_cast<Matrix<RowSize, ColumnSize, T>&>(*this);
			}

			/// Divides each element in \p lhs by the scalar value \p rhs.
			// Intentional Matrix copy.
			constexpr friend auto operator/(Matrix<RowSize, ColumnSize, T> lhs, T rhs) { return lhs /= rhs; }

			/*
			 * Matrix Maths Operations
			 */
			/// Takes the dot product of each row in \p lhs by each column in \p rhs to create a new matrix.
			/// @param lhs A matrix with a number of columns equal to the number of rows in \p rhs.
			/// @param rhs A matrix with a number of rows equal to the number of columns in \p lhs.
			/// @return A matrix with the same number of rows as \p lhs and the same number of columns as \p rhs.
			template <std::size_t OtherColumnSize>
			constexpr friend auto operator*(const Matrix<RowSize, ColumnSize, T>& lhs,
			                                const Matrix<ColumnSize, OtherColumnSize, T>& rhs)
			{
				Matrix<RowSize, OtherColumnSize, T> result;
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
								lhs.GetRow(row), rhs.GetColumn(column)
							);
					}
				}

				return result;
			}

			/// Multiplies a row vector by a matrix.
			/// @param lhs A row vector with a number of columns equal to the number of rows in \p rhs.
			/// @param rhs A matrix with a number of rows equal to the size of \p lhs.
			/// @return A row vector with a size equal to the number of columns in \p rhs.
			constexpr friend auto operator*(const Vector<RowSize, T>& lhs,
			                                const Matrix<RowSize, ColumnSize, T>& rhs)
			{
				Vector<ColumnSize, T> rowVector;
				for (std::size_t column = 0; column < rowVector.size(); ++column)
				{
					rowVector[column] = Vector<RowSize, T>::DotProduct(lhs, rhs.GetColumn(column));
				}

				return rowVector;
			}

			/// Multiplies a column vector by a matrix.
			/// @param lhs A matrix with a number of columns equal to the size of \p rhs.
			/// @param rhs A column vector with a number of rows equal to the number of columns in \p lhs.
			/// @return A column vector with a size equal to the number of rows in \p lhs.
			constexpr friend auto operator*(const Matrix<RowSize, ColumnSize, T>& lhs,
			                                const Vector<ColumnSize, T>& rhs)
			{
				Vector<RowSize, T> columnVector;

				for (std::size_t row = 0; row < columnVector.size(); ++row)
				{
					columnVector[row] = Vector<ColumnSize, T>::DotProduct(lhs.GetRow(row), rhs);
				}

				return columnVector;
			}

			/*
			 * Comparison Operators
			 */
			/// Checks whether each element in the two matrices are equal.
			constexpr friend bool operator==(const Matrix<RowSize, ColumnSize, T>& lhs,
			                                 const Matrix<RowSize, ColumnSize, T>& rhs)
			{
				for (std::size_t i = 0; i < lhs.size(); ++i) { if (lhs[i] != rhs[i]) { return false; } }
				return true;
			}

			/// Checks whether any element in two matrices differ.
			constexpr friend bool operator!=(const Matrix<RowSize, ColumnSize, T>& lhs,
			                                 const Matrix<RowSize, ColumnSize, T>& rhs) { return !(lhs == rhs); }

			// If these aren't deleted it makes it possible to perform comparisons like Matrix<4, 3>{} == Matrix<3, 4>{},
			// as well as to arrays of the same type and size.
			// auto val = Matrix<4, 3>{} == Matrix<3, 4>{}; // Shouldn't compile
			// auto val1 = Matrix<4, 3>{} == std::array<float, 12>{}; //  Shouldn't compile
			// auto val2 = Vector<3>{} == std::array<float, 3>{}; // Shouldn't compile.
			// auto val3 = std::array<float, 3>{} == std::array<float, 3>{}; // Should compule.
			// auto val4 = std::array<float, 12>{} == Matrix<4, 3>{}; //  Shouldn't compile
			auto operator==(const std::array<T, RowSize * ColumnSize>& rhs) = delete;

			auto operator<=>(const std::array<T, RowSize * ColumnSize>& rhs) = delete;

			/*
			 * Linear Transformations (3x3 and 4x4)
			 */
			static constexpr Matrix<RowSize, ColumnSize, T> RotationAboutX(T radians)
				requires std::floating_point<T> &&
				IsSquare<RowSize, ColumnSize> &&
				IsValidDimensions<RowSize, 3, 4>
			{
				Matrix matrix{Matrix<RowSize, ColumnSize, T>::IdentityMatrix()};
				matrix(1, 1) = std::cos(radians);
				matrix(1, 2) = std::sin(radians);
				matrix(2, 1) = -std::sin(radians);
				matrix(2, 2) = std::cos(radians);

				return matrix;
			}

			static constexpr Matrix<RowSize, ColumnSize, T> RotationAboutY(T radians)
				requires std::floating_point<T> &&
				IsSquare<RowSize, ColumnSize> &&
				IsValidDimensions<RowSize, 3, 4>
			{
				Matrix matrix{Matrix<RowSize, ColumnSize, T>::IdentityMatrix()};
				matrix(0, 0) = std::cos(radians);
				matrix(0, 2) = -std::sin(radians);
				matrix(2, 0) = std::sin(radians);
				matrix(2, 2) = std::cos(radians);

				return matrix;
			}

			static constexpr Matrix<RowSize, ColumnSize, T> RotationAboutZ(T radians)
				requires std::floating_point<T> &&
				IsSquare<RowSize, ColumnSize> &&
				IsValidDimensions<RowSize, 3, 4>
			{
				Matrix matrix{Matrix<RowSize, ColumnSize, T>::IdentityMatrix()};
				matrix(0, 0) = std::cos(radians);
				matrix(0, 1) = std::sin(radians);
				matrix(1, 0) = -std::sin(radians);
				matrix(1, 1) = std::cos(radians);

				return matrix;
			}

			/// @param axis The axis to rotate about as a unit vector.
			/// @param radians The number of degrees in radians to rotate about \p axis.
			static constexpr Matrix<RowSize, ColumnSize, T> RotationAboutAxis(const Vector<3, T>& axis, T radians)
				requires std::floating_point<T> &&
				IsSquare<RowSize, ColumnSize> &&
				IsValidDimensions<RowSize, 3, 4>
			{
				assert(axis.IsUnit());

				// Some convenience
				using namespace std;
				const T x = axis.X();
				const T y = axis.Y();
				const T z = axis.Z();

				Matrix matrix{Matrix<RowSize, ColumnSize, T>::IdentityMatrix()};
				matrix(0, 0) = x * x * (1 - cos(radians)) + cos(radians);
				matrix(0, 1) = x * y * (1 - cos(radians)) + z * sin(radians);
				matrix(0, 2) = x * z * (1 - cos(radians)) - y * sin(radians);
				matrix(1, 0) = x * y * (1 - cos(radians)) - z * sin(radians);
				matrix(1, 1) = y * y * (1 - cos(radians)) + cos(radians);
				matrix(1, 2) = y * z * (1 - cos(radians)) + x * sin(radians);
				matrix(2, 0) = x * z * (1 - cos(radians)) + y * sin(radians);
				matrix(2, 1) = y * z * (1 - cos(radians)) - x * sin(radians);
				matrix(2, 2) = z * z * (1 - cos(radians)) + cos(radians);

				return matrix;
			}

			static constexpr Matrix<RowSize, ColumnSize, T> ScalingAlongCardinalAxes(T x, T y, T z)
				requires IsSquare<RowSize, ColumnSize> &&
				IsValidDimensions<RowSize, 3, 4>
			{
				Matrix matrix{Matrix<RowSize, ColumnSize, T>::IdentityMatrix()};
				matrix(0, 0) = x;
				matrix(1, 1) = y;
				matrix(2, 2) = z;

				return matrix;
			}

			/// @param axis A unit vector.
			/// @param k A scale factor.
			/// @return A matrix that when multiplied by resulting in a scaling of \p k along \p axis.
			static constexpr Matrix<RowSize, ColumnSize, T> ScalingAlongAxis(const Vector<3, T>& axis, T k)
				requires IsSquare<RowSize, ColumnSize> &&
				IsValidDimensions<RowSize, 3, 4>
			{
				assert(axis.IsUnit());

				// Some convenience
				using namespace std;
				const T x = axis.X();
				const T y = axis.Y();
				const T z = axis.Z();

				Matrix matrix{Matrix<RowSize, ColumnSize, T>::IdentityMatrix()};
				matrix(0, 0) = 1 + (k - 1) * x * x;
				matrix(0, 1) = (k - 1) * x * y;
				matrix(0, 2) = (k - 1) * x * z;
				matrix(1, 0) = (k - 1) * x * y;
				matrix(1, 1) = 1 + (k - 1) * y * y;
				matrix(1, 2) = (k - 1) * y * z;
				matrix(2, 0) = (k - 1) * x * z;
				matrix(2, 1) = (k - 1) * y * z;
				matrix(2, 2) = 1 + (k - 1) * z * z;

				return matrix;
			}

			static constexpr Matrix<RowSize, ColumnSize, T> ProjectionOntoPlaneXY()
				requires IsSquare<RowSize, ColumnSize> &&
				IsValidDimensions<RowSize, 3, 4>
			{
				Matrix matrix{Matrix<RowSize, ColumnSize, T>::IdentityMatrix()};
				matrix(2, 2) = 0;

				return matrix;
			}

			static constexpr Matrix<RowSize, ColumnSize, T> ProjectionOntoPlaneXZ()
				requires IsSquare<RowSize, ColumnSize> &&
				IsValidDimensions<RowSize, 3, 4>
			{
				Matrix matrix{Matrix<RowSize, ColumnSize, T>::IdentityMatrix()};
				matrix(1, 1) = 0;

				return matrix;
			}

			static constexpr Matrix<RowSize, ColumnSize, T> ProjectionOntoPlaneYZ()
				requires IsSquare<RowSize, ColumnSize> &&
				IsValidDimensions<RowSize, 3, 4>
			{
				Matrix matrix{Matrix<RowSize, ColumnSize, T>::IdentityMatrix()};
				matrix(0, 0) = 0;

				return matrix;
			}

			/// @param axis A unit vector
			/// @return A matrix that when multiplied by results in an orthographic projection onto \p axis.
			static constexpr Matrix<RowSize, ColumnSize, T> ProjectionOntoPlane(const Vector<3, T>& axis)
				requires IsSquare<RowSize, ColumnSize> &&
				IsValidDimensions<RowSize, 3, 4>
			{
				// akin to ScalingAlongAxis(axis, 0)
				assert(axis.IsUnit());

				// Some convenience
				using namespace std;
				const T x = axis.X();
				const T y = axis.Y();
				const T z = axis.Z();

				Matrix matrix{Matrix<RowSize, ColumnSize, T>::IdentityMatrix()};
				matrix(0, 0) = 1 - x * x;
				matrix(0, 1) = -x * y;
				matrix(0, 2) = -x * z;
				matrix(1, 0) = -x * y;
				matrix(1, 1) = 1 - y * y;
				matrix(1, 2) = -y * z;
				matrix(2, 0) = -x * z;
				matrix(2, 1) = -y * z;
				matrix(2, 2) = 1 - z * z;

				return matrix;
			}

			static constexpr Matrix<RowSize, ColumnSize, T> Reflection(const Vector<3, T>& axis)
				requires IsSquare<RowSize, ColumnSize> &&
				IsValidDimensions<RowSize, 3, 4>
			{
				// akin to ScalingAlongAxis(axis, -1)
				assert(axis.IsUnit());

				// Some convenience
				const T x = axis.X();
				const T y = axis.Y();
				const T z = axis.Z();

				Matrix matrix{Matrix<RowSize, ColumnSize, T>::IdentityMatrix()};
				matrix(0, 0) = 1 - 2 * x * x;
				matrix(0, 1) = -2 * x * y;
				matrix(0, 2) = -2 * x * z;
				matrix(1, 0) = -2 * x * y;
				matrix(1, 1) = 1 - 2 * y * y;
				matrix(1, 2) = -2 * y * z;
				matrix(2, 0) = -2 * x * z;
				matrix(2, 1) = -2 * y * z;
				matrix(2, 2) = 1 - 2 * z * z;

				return matrix;
			}

			constexpr static Matrix<RowSize, ColumnSize, T> Shearing(
				float xy, float xz, float yx, float yz, float zx, float zy)
				requires IsSquare<RowSize, ColumnSize> &&
				IsValidDimensions<RowSize, 3, 4>
			{
				Matrix shear{Matrix<RowSize, ColumnSize, T>::IdentityMatrix()};
				shear(0, 1) = xy;
				shear(0, 2) = xz;
				shear(1, 0) = yx;
				shear(1, 2) = yz;
				shear(2, 0) = zx;
				shear(2, 1) = zy;

				return shear;
			}
		};
	}

	// Row first, then column to follow normal matrix conventions.
	/// Matrix with its elements stored in row-major order.
	///	Linear transformations assume row vectors.
	/// @tparam RowSize The vertical size of the matrix.
	/// @tparam ColumnSize The horizontal size of the matrix.
	/// @tparam T The type of each element stored in the matrix.
	template <std::size_t RowSize, std::size_t ColumnSize = RowSize, Number T = float>
	struct Matrix final : Detail::MatrixBase<RowSize, ColumnSize, T> {};

	// Row first, then column to follow normal matrix conventions.
	// Could move some duplicated code into the shared inherited object,
	// but for interface reason such as names or parameters it's nicer to
	// re-implement.
	/// Matrix with its elements stored in row-major order.
	///	Linear transformations assume row vectors.
	/// @tparam RowSize The vertical size of the matrix.
	/// @tparam ColumnSize The horizontal size of the matrix.
	/// @tparam T The type of each element stored in the matrix.
	template <Number T>
	struct Matrix<2, 2, T> final : Detail::MatrixBase<2, 2, T>
	{
		/// @return A matrix composed of the world's basis vectors.
		static constexpr Matrix World()
		{
			return {
				Vector<2>::Right().X(), Vector<2>::Right().Y(),
				Vector<2>::Up().X(), Vector<2>::Up().Y()
			};
		}

		/// Rotation matrix around the Z-Axis.
		static constexpr Matrix Rotation(T radians) requires std::floating_point<T>
		{
			Matrix matrix{Matrix::IdentityMatrix()};
			matrix(0, 0) = std::cos(radians);
			matrix(0, 1) = std::sin(radians);
			matrix(1, 0) = -std::sin(radians);
			matrix(1, 1) = std::cos(radians);

			return matrix;
		}

		static constexpr Matrix ScalingAlongCardinalAxes(T x, T y)
		{
			return {
				x, 0,
				0, y
			};
		}

		static constexpr Matrix ScalingAlongAxis(const Vector<2, T>& axis, T k)
		{
			assert(axis.IsUnit());

			// Some convenience
			const T x = axis.X();
			const T y = axis.Y();

			Matrix matrix;
			matrix(0, 0) = 1 + (k - 1) * x * x;
			matrix(0, 1) = (k - 1) * x * y;
			matrix(1, 0) = (k - 1) * x * y;
			matrix(1, 1) = 1 + (k - 1) * y * y;

			return matrix;
		}

		static constexpr Matrix ProjectionOntoAxisX()
		{
			return
			{
				1, 0,
				0, 0
			};
		}

		static constexpr Matrix ProjectionOntoAxisY()
		{
			return
			{
				0, 0,
				0, 1
			};
		}

		/// @return A matrix that when multiplied by results in an orthographic projection onto \p axis.
		static constexpr Matrix ProjectionOntoAxis(const Vector<2, T>& axis)
		{
			// akin to ScalingAlongAxis(axis, 0)
			assert(axis.IsUnit());

			// Some convenience
			const T x = axis.X();
			const T y = axis.Y();

			Matrix matrix;

			// Handle shared.
			matrix(0, 0) = 1 - x * x;
			matrix(0, 1) = -x * y;
			matrix(1, 0) = -x * y;
			matrix(1, 1) = 1 - y * y;

			return matrix;
		}

		static constexpr Matrix Reflection(const Vector<2, T>& axis)
		{
			// akin to ScalingAlongAxis(axis, -1)
			assert(axis.IsUnit());

			// Some convenience
			const T x = axis.X();
			const T y = axis.Y();

			return
			{
				1 - 2 * x * x, -2 * x * y,
				-2 * x * y, 1 - 2 * y * y
			};
		}

		static constexpr Matrix Shearing(T xy, T yx)
		{
			return
			{
				1, yx,
				xy, 1
			};
		}

		/* Methods */
		constexpr T Determinant()
		{
			const Matrix& m = *this;
			return {m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0)};
		}
	};

	// Row first, then column to follow normal matrix conventions.
	/// Matrix with its elements stored in row-major order.
	///	Linear transformations assume row vectors.
	/// @tparam RowSize The vertical size of the matrix.
	/// @tparam ColumnSize The horizontal size of the matrix.
	/// @tparam T The type of each element stored in the matrix.
	template <Number T>
	struct Matrix<3, 3, T> final : Detail::MatrixBase<3, 3, T>
	{
		/// @return A matrix composed of the world's basis vectors.
		static constexpr Matrix World()
		{
			return {
				Vector<3>::Right().X(), Vector<3>::Right().Y(), Vector<3>::Right().Z(),
				Vector<3>::Up().X(), Vector<3>::Up().Y(), Vector<3>::Up().Z(),
				Vector<3>::Forward().X(), Vector<3>::Forward().Y(), Vector<3>::Forward().Z(),

			};
		}

		/* Methods */
		constexpr T Determinant()
		{
			const Matrix& m = *this;
			return
			{
				m(0, 0) * (m(1, 1) * m(2, 2) - m(1, 2) * m(2, 1)) +
				m(0, 1) * (m(1, 2) * m(2, 0) - m(1, 0) * m(2, 2)) +
				m(0, 2) * (m(1, 0) * m(2, 1) - m(1, 1) * m(2, 0))
			};
		}
	};

	// Row first, then column to follow normal matrix conventions.
	/// Matrix with its elements stored in row-major order.
	///	Linear transformations assume row vectors.
	/// @tparam RowSize The vertical size of the matrix.
	/// @tparam ColumnSize The horizontal size of the matrix.
	/// @tparam T The type of each element stored in the matrix.
	template <Number T>
	struct Matrix<4, 4, T> final : Detail::MatrixBase<4, 4, T>
	{
		/// @return A matrix composed of the world's basis vectors.
		static constexpr Matrix World()
		{
			return {
				Vector<3>::Right().X(), Vector<3>::Right().Y(), Vector<3>::Right().Z(), 0,
				Vector<3>::Up().X(), Vector<3>::Up().Y(), Vector<3>::Up().Z(), 0,
				Vector<3>::Forward().X(), Vector<3>::Forward().Y(), Vector<3>::Forward().Z(), 0,
				0, 0, 0, 1

			};
		}

		static constexpr Matrix Translation(T dx, T dy, T dz)
		{
			return
			{
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				dx, dy, dz, 1
			};
		}

		// TODO: Perspective projection.
	};
}
