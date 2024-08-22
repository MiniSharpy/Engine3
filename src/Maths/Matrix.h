#pragma once
#include "Vector.h"
#include <algorithm>
#include <array>
#include <concepts>
#include <cstddef>
#include <print>
#include <tuple>
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

	template <class T>
	concept Number = std::is_floating_point_v<T> || std::is_integral_v<T>;

	template <std::size_t a, std::size_t b>
	concept IsSquare = (a == b);

	template <std::size_t RowSize, std::size_t ColumnSize, Number T>
	struct Matrix;

	namespace Detail
	{
		template <std::size_t RowSize, std::size_t ColumnSize, Number T>
		struct MatrixBase : std::array<T, RowSize * ColumnSize>
		{
			/// @return A square matrix that's zeroed except for its diagonal elements, which are all one.
			static constexpr auto IdentityMatrix() requires (IsSquare<RowSize, ColumnSize>)
			{
				Matrix<RowSize, ColumnSize, T> identityMatrix{};
				for (std::size_t i = 0; i < RowSize; ++i) { identityMatrix[(RowSize + 1) * i] = 1; }

				return identityMatrix;
			}

			/* Methods */
			/// Flips a square matrix diagonally, in place. TODO: Should the function return a reference to this to allow chaining methods?
			template <typename Self>
			void Transpose(this Self& self) requires (RowSize == ColumnSize) { self = self.Transposed(); }

			/// Flips a matrix diagonally.
			[[nodiscard]] Matrix<ColumnSize, RowSize, T> Transposed()
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

			/// @param row The row to return, indexed from zero/
			/// @return A row vector.
			constexpr Vector<ColumnSize, T> GetRow(std::size_t row) const
			{
				assert(row < RowSize);
				Vector<ColumnSize, T> vector;
				for (std::size_t column = 0; column < ColumnSize; ++column) { vector[column] = (*this)(row, column); }
				return vector;
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

			/* Operators */
			// I would like to use [row, column] for zero indexing and operator() for 1 indexed to follow matrix conventions,
			// but there's a lack of support for it in gcc and I prefer zero indexing.
			/// @param row The row to retrieve the element, indexed from zero.
			/// @param column The column to retrieve the element, indexed from zero.
			/// @return The element at \p row and \p column.
			T& operator()(std::size_t row, std::size_t column)
			{
				assert(row < RowSize);
				assert(column < ColumnSize);
				return (*this)[ColumnSize * row + column];
			}

			/// @param row The row to retrieve the element, indexed from zero.
			/// @param column The column to retrieve the element, indexed from zero.
			/// @return The element at \p row and \p column.
			const T& operator()(std::size_t row, std::size_t column) const
			{
				assert(row < RowSize);
				assert(column < ColumnSize);
				return (*this)[ColumnSize * row + column];
			}

			// Scalar multiplications.
			/// Multiplies each element in the matrix by the scalar value \p other.
			/// @return A reference to the now altered matrix for further operations.
			template <typename Self>
			constexpr Self& operator*=(this Self& self, T rhs)
			{
				for (std::size_t i = 0; i < self.size(); ++i) { self[i] *= rhs; }
				return self;
			}

			/// Multiplies each element in the copied \p lhs by the scalar value \p rhs.
			// Intentional Matrix copy.
			constexpr friend auto operator*(Matrix<RowSize, ColumnSize, T> lhs, T rhs) { return lhs *= rhs; }

			/// Multiplies each element in \p right by the scalar value \p left.
			constexpr friend auto operator*(T lhs, const Matrix<RowSize, ColumnSize, T>& rhs) { return rhs * lhs; }

			/// Divides each element in the matrix by \p rhs in place.
			/// @return A reference to the now altered matrix for further operations.
			template <typename Self>
			constexpr Self& operator/=(this Self& self, T rhs)
			{
				for (std::size_t i = 0; i < self.size(); ++i) { self[i] /= rhs; }
				return self;
			}

			/// Divides each element in \p lhs by the scalar value \p rhs.
			// Intentional Matrix copy.
			constexpr friend auto operator/(Matrix<RowSize, ColumnSize, T> lhs, T rhs) { return lhs /= rhs; }

			/// Takes the dot product of each row in \p lhs by each column in \p rhs to create a new matrix.
			/// @param lhs A matrix with a number of columns equal to the number of rows in \p rhs.
			/// @param rhs A matrix with a number of rows equal to the number of columns in \p lhs.
			/// @return A matrix with the same number of rows as \p lhs and the same number of columns as \p rhs.
			template <std::size_t OtherColumnSize>
			constexpr friend auto operator*(const Matrix<RowSize, ColumnSize, T>& lhs,
			                                const Matrix<ColumnSize, OtherColumnSize, T>&
			                                rhs)
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

			/// Checks whether each element in the two matrices are equal.
			friend bool operator==(const Matrix<RowSize, ColumnSize, T>& lhs, const Matrix<RowSize, ColumnSize, T>& rhs)
			{
				for (std::size_t i = 0; i < lhs.size(); ++i) { if (lhs[i] != rhs[i]) { return false; } }
				return true;
			}

			/// Checks whether any element in two matrices differ.
			friend bool operator!=(const Matrix<RowSize, ColumnSize, T>& lhs, const Matrix<RowSize, ColumnSize, T>& rhs)
			{
				return !(lhs == rhs);
			}

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

	// Row first, then column to follow normal matrix conventions.
	/// Matrix with its elements stored in row-major order.
	/// @tparam RowSize The vertical size of the matrix.
	/// @tparam ColumnSize The horizontal size of the matrix.
	/// @tparam T The type of each element stored in the matrix.
	template <std::size_t RowSize, std::size_t ColumnSize = RowSize, Number T = float>
	struct Matrix final : Detail::MatrixBase<RowSize, ColumnSize, T> {};

	// Row first, then column to follow normal matrix conventions.
	/// Matrix with its elements stored in row-major order.
	/// @tparam RowSize The vertical size of the matrix.
	/// @tparam ColumnSize The horizontal size of the matrix.
	/// @tparam T The type of each element stored in the matrix.
	template <Number T>
	struct Matrix<2, 2, T> final : Detail::MatrixBase<2, 2, T>
	{
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
	};

	// Row first, then column to follow normal matrix conventions.
	/// Matrix with its elements stored in row-major order.
	/// @tparam RowSize The vertical size of the matrix.
	/// @tparam ColumnSize The horizontal size of the matrix.
	/// @tparam T The type of each element stored in the matrix.
	template <Number T>
	struct Matrix<3, 3, T> final : Detail::MatrixBase<3, 3, T>
	{
		static constexpr Matrix RotationAboutX(T radians) requires std::floating_point<T>
		{
			Matrix matrix{Matrix::IdentityMatrix()};
			matrix(1, 1) = std::cos(radians);
			matrix(1, 2) = std::sin(radians);
			matrix(2, 1) = -std::sin(radians);
			matrix(2, 2) = std::cos(radians);

			return matrix;
		}

		static constexpr Matrix RotationAboutY(T radians) requires std::floating_point<T>
		{
			Matrix matrix{Matrix::IdentityMatrix()};
			matrix(0, 0) = std::cos(radians);
			matrix(0, 2) = -std::sin(radians);
			matrix(2, 0) = std::sin(radians);
			matrix(2, 2) = std::cos(radians);

			return matrix;
		}

		static constexpr Matrix RotationAboutZ(T radians) requires std::floating_point<T>
		{
			Matrix matrix{Matrix::IdentityMatrix()};
			matrix(0, 0) = std::cos(radians);
			matrix(0, 1) = std::sin(radians);
			matrix(1, 0) = -std::sin(radians);
			matrix(1, 1) = std::cos(radians);

			return matrix;
		}

		/// @param axis The axis to rotate about as a unit vector.
		/// @param radians The number of degrees in radians to rotate about \p axis.
		static constexpr Matrix RotationAboutAxis(const Vector<3, T>& axis, T radians) requires std::floating_point<T>
		{
			assert(axis.IsUnit());

			// Some convenience
			using namespace std;
			const T x = axis.X();
			const T y = axis.Y();
			const T z = axis.Z();

			Matrix matrix{Matrix::IdentityMatrix()};
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

		static constexpr Matrix ScalingAlongCardinalAxes(T x, T y, T z)
		{
			return {
				x, 0, 0,
				0, y, 0,
				0, 0, z
			};
		}

		/// 
		/// @param axis A unit vector.
		/// @param k A scale factor.
		/// @return A matrix that when multiplied by resulting in a scaling of \p k along \p axis.
		static constexpr Matrix ScalingAlongAxis(const Vector<3, T>& axis, T k)
		{
			assert(axis.IsUnit());

			// Some convenience
			using namespace std;
			const T x = axis.X();
			const T y = axis.Y();
			const T z = axis.Z();

			Matrix matrix;
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

		static constexpr Matrix ProjectionOntoPlaneXY()
		{
			return {
				1, 0, 0,
				0, 1, 0,
				0, 0, 0
			};
		}

		static constexpr Matrix ProjectionOntoPlaneXZ()
		{
			return {
				1, 0, 0,
				0, 0, 0,
				0, 0, 1
			};
		}

		static constexpr Matrix ProjectionOntoPlaneYZ()
		{
			return {
				0, 0, 0,
				0, 1, 0,
				0, 0, 1
			};
		}

		/// @param axis A unit vector
		/// @return A matrix that when multiplied by results in an orthographic projection onto \p axis.
		static constexpr Matrix ProjectionOntoPlane(const Vector<3, T>& axis)
		{
			// akin to ScalingAlongAxis(axis, 0)
			assert(axis.IsUnit());

			// Some convenience
			using namespace std;
			const T x = axis.X();
			const T y = axis.Y();
			const T z = axis.Z();

			Matrix matrix;
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

		static constexpr Matrix Reflection(const Vector<3, T>& axis)
		{
			// akin to ScalingAlongAxis(axis, -1)
			assert(axis.IsUnit());

			// Some convenience
			const T x = axis.X();
			const T y = axis.Y();
			const T z = axis.Z();

			return
			{
				1 - 2 * x * x, -2 * x * y, -2 * x * z,
				-2 * x * y, 1 - 2 * y * y, -2 * y * z,
				-2 * x * z, -2 * y * z, 1 - 2 * z * z
			};
		}

		constexpr static Matrix Shearing(float xy, float xz, float yx, float yz, float zx, float zy)
		{
			Matrix shear = Matrix::IdentityMatrix();
			shear(0, 1) = xy;
			shear(0, 2) = xz;
			shear(1, 0) = yx;
			shear(1, 2) = yz;
			shear(2, 0) = zx;
			shear(2, 1) = zy;

			return shear;
		}
	};

	// Row first, then column to follow normal matrix conventions.
	/// Matrix with its elements stored in row-major order.
	/// @tparam RowSize The vertical size of the matrix.
	/// @tparam ColumnSize The horizontal size of the matrix.
	/// @tparam T The type of each element stored in the matrix.
	template <Number T>
	struct Matrix<4, 4, T> final : Detail::MatrixBase<4, 4, T>
	{
		static constexpr Matrix ScalingAlongCardinalAxes(T x, T y, T z, T w)
		{
			return {
				x, 0, 0, 0,
				0, y, 0, 0,
				0, 0, z, 0,
				0, 0, 0, w
			};
		}
	};
}
