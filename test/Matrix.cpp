#include "../src/Maths/Matrix.h"
#include "../src/Maths/Maths.h"
#include <gtest/gtest.h>

namespace Engine3
{
	/// Primarily a test to validate row-major order is kept.
	TEST(MatrixSquareTest, FunctionCallOperator)
	{
		Matrix<3> actual{};
		actual(2, 1) = 5;

		Matrix<3> expected =
		{
			0, 0, 0,
			0, 0, 0,
			0, 5, 0
		};
		ASSERT_EQ(expected, actual);
	}

	TEST(MatrixNonSquareTest, FunctionCallOperator)
	{
		Matrix<3, 4> actual{};
		actual(2, 1) = 5;

		Matrix<3, 4> expected =
		{
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 5, 0, 0
		};
		ASSERT_EQ(expected, actual);
	}

	TEST(MatrixNonSquareTest, FunctionCallOperatorOtherAspectRatiio)
	{
		Matrix<4, 3> actual{};
		actual(2, 1) = 5;

		Matrix<4, 3> expected =
		{
			0, 0, 0,
			0, 0, 0,
			0, 5, 0,
			0, 0, 0
		};
		ASSERT_EQ(expected, actual);
	}

	TEST(Matrix4x4Test, IdentityMatrix)
	{
		Matrix<4> expected =
		{
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};

		ASSERT_EQ(Matrix<4>::IdentityMatrix(), expected);
	}

	TEST(Matrix4x3Test, Transposed)
	{
		constexpr Matrix<3, 4> actual =
			Matrix<4, 3>
			{
				1, 2, 3,
				4, 5, 6,
				7, 8, 9,
				10, 11, 12
			}.Transposed();

		constexpr Matrix<3, 4> expected
		{
			1, 4, 7, 10,
			2, 5, 8, 11,
			3, 6, 9, 12
		};

		ASSERT_EQ(expected, actual);
	}

	TEST(MatrixSquareTest, GetRow)
	{
		constexpr Vector<3> actual = Matrix<3>
		{
			1, 2, 3,
			4, 5, 6,
			7, 8, 9
		}.GetRow(1);

		constexpr Vector<3> expected{4, 5, 6};
		ASSERT_EQ(expected, actual);
	}

	TEST(MatrixSquareTest, GetColumn)
	{
		constexpr Vector<4> actual = Matrix<4>
		{
			1, 2, 3, 4,
			5, 6, 7, 8,
			9, 10, 11, 12,
			13, 14, 15, 16
		}.GetColumn(1);

		constexpr Vector<4> expected{2, 6, 10, 14};
		ASSERT_EQ(expected, actual);
	}

	TEST(MatrixNonSquareTest, MatrixMultiplication2x4By4x3)
	{
		constexpr Matrix<2, 4> a =
		{
			1, 2, 3, 4,
			5, 6, 7, 8
		};

		constexpr Matrix<4, 3> b =
		{
			1, 2, 3,
			4, 5, 6,
			7, 8, 9,
			10, 11, 12
		};

		constexpr Matrix<2, 3> ab = a * b;

		constexpr Matrix<2, 3> expected =
		{
			70, 80, 90,
			158, 184, 210
		};
		ASSERT_EQ(expected, ab);
	}

	TEST(Matrix2x2Test, MatrixMultiplication)
	{
		constexpr Matrix<2> a
		{
			1, -2,
			5, 0
		};

		constexpr Matrix<2> b
		{
			-3, 7,
			4, 1.f / 3.f
		};

		constexpr Matrix<2> ab = a * b;

		constexpr Matrix<2> expected =
		{
			-11, 6.f + 1.f / 3.f, -15, 35
		};
		ASSERT_EQ(expected, ab);
	}

	TEST(Matrix2x2Test, VectorMultiplication)
	{
		constexpr Vector<2> a
		{
			3, 3
		};

		constexpr Matrix<2> b
		{
			6, -7,
			-4, 5
		};

		constexpr Vector<2> ab = a * b;

		constexpr Vector<2> expected =
		{
			6, -6
		};

		ASSERT_EQ(expected, ab);
	}

	TEST(MatrixNonSquareTest, RowVectorMultiplication)
	{
		constexpr Matrix<2, 3> matrix =
		{
			1, 2, 3,
			4, 5, 6,
		};

		constexpr Vector<2> rowVector =
		{
			1, 2
		};

		constexpr Vector<3> actual = rowVector * matrix;

		constexpr Vector<3> expected =
		{
			9, 12, 15
		};
		ASSERT_EQ(expected, actual);
	}

	TEST(MatrixNonSquareTest, ColumnVectorMultiplication)
	{
		constexpr Matrix<4, 3> matrix =
		{
			1, 2, 3,
			4, 5, 6,
			7, 8, 9,
			10, 11, 12
		};

		constexpr Vector<3> rowVector =
		{
			1, 2, 3
		};

		constexpr Vector<4> actual = matrix * rowVector;

		constexpr Vector<4> expected =
		{
			14, 32, 50, 68
		};
		ASSERT_EQ(expected, actual);
	}

	TEST(Matrix2x2FloatTest, RotationZ)
	{
		Matrix actual{Matrix<2>::Rotation(DegreesToRadians(45.f))};
		Matrix<2> expected
		{
			0.707106782f, 0.7071067812f,
			-0.7071067812f, 0.7071067812f
		};
		ASSERT_EQ(expected, actual);
	}

	TEST(Matrix2x2FloatTest, ScalingCardinal)
	{
		Matrix<2> actual = Matrix<2>::ScalingAlongCardinalAxes(2, 2);
		Matrix<2> expected
		{
			2, 0,
			0, 2
		};

		ASSERT_EQ(actual, expected);
	}

	TEST(Matrix4x4FloatTest, ScalingCardinal)
	{
		Matrix<4> actual = Matrix<4>::ScalingAlongCardinalAxes(2, 2, 2);
		Matrix<4> expected
		{
			2, 0, 0, 0,
			0, 2, 0, 0,
			0, 0, 2, 0,
			0, 0, 0, 1
		};

		ASSERT_EQ(actual, expected);
	}

	TEST(Matrix2x2FloatTest, ScalingAxis)
	{
		Vector<2> axis = Vector<2>::Up();
		Vector<2> actual = Vector<2>{0, 1} * Matrix<2>::ScalingAlongAxis(axis, 2);
		Vector<2> expected{0, 2};

		ASSERT_EQ(actual, expected);
	}

	TEST(Matrix2x2FloatTest, OrthographicProjectAxisX)
	{
		Matrix<2> actual = Matrix<2>::ProjectionOntoAxisX();
		Matrix<2> expected{1, 0, 0, 0};

		ASSERT_EQ(actual, expected);
	}

	TEST(Matrix2x2FloatTest, OrthographicProjectAxisY)
	{
		Matrix<2> actual = Matrix<2>::ProjectionOntoAxisY();
		Matrix<2> expected{0, 0, 0, 1};

		ASSERT_EQ(actual, expected);
	}

	TEST(Matrix2x2FloatTest, OrthographicProjectAxis)
	{
		Vector<2> axis{0.7f, -0.7f};
		axis.Normalise();
		Matrix<2> actual = Matrix<2>::ProjectionOntoAxis(axis);
		Matrix<2> expected{0.5f, 0.5f, 0.5f, 0.5f};

		EXPECT_NEAR(actual[0], expected[0], 0.01);
		EXPECT_NEAR(actual[1], expected[1], 0.01);
		EXPECT_NEAR(actual[2], expected[2], 0.01);
		EXPECT_NEAR(actual[3], expected[3], 0.01);
	}

	TEST(Matrix2x2FloatTest, Reflection)
	{
		Vector<2> axis{0.7f, 0.7f};
		axis.Normalise();
		Matrix<2> actual = Matrix<2>::Reflection(axis);
		Matrix<2> expected =
		{
			0.000302f, -0.999698f,
			-0.999698f, 0.000302f
		};

		EXPECT_NEAR(actual[0], expected[0], 0.01);
		EXPECT_NEAR(actual[1], expected[1], 0.01);
		EXPECT_NEAR(actual[2], expected[2], 0.01);
		EXPECT_NEAR(actual[3], expected[3], 0.01);
	}

	TEST(Matrix2x2FloatTest, Shearing)
	{
		Vector<2> axis{0.7f, 0.7f};
		axis.Normalise();
		Matrix<2> actual = Matrix<2>::Reflection(axis);
		Matrix<2> expected =
		{
			0.000302f, -0.999698f,
			-0.999698f, 0.000302f
		};

		EXPECT_NEAR(actual[0], expected[0], 0.01);
		EXPECT_NEAR(actual[1], expected[1], 0.01);
		EXPECT_NEAR(actual[2], expected[2], 0.01);
		EXPECT_NEAR(actual[3], expected[3], 0.01);
	}

	TEST(Matrix2x2FloatTest, Determinant)
	{
		Matrix<2> matrix{3, -2, 1, 4};
		float actual = matrix.Determinant();

		float expected = 14;
		ASSERT_EQ(expected, actual);
	}

	TEST(Matrix4x4FloatTest, Submatrix)
	{
		constexpr Matrix<4> matrix
		{
			3, -2, 0, 1,
			1, 4, 0, 1,
			0, 0, 2, 1,
			1, 1, 1, 1
		};
		constexpr Matrix<3> actual = matrix.Submatrix(1, 1);

		constexpr Matrix<3> expected
		{
			3, 0, 1,
			0, 2, 1,
			1, 1, 1
		};
		ASSERT_EQ(expected, actual);
	}

	TEST(Matrix4x4FloatTest, Minor)
	{
		constexpr Matrix<4> matrix
		{
			3, -2, 0, 1,
			1, 4, 0, 1,
			0, 0, 2, 1,
			1, 1, 1, 1
		};

		constexpr float actual = matrix.Minor(1, 1);

		constexpr float expected = 1;
		ASSERT_EQ(expected, actual);
	}

	TEST(Matrix4x4FloatTest, Cofactor)
	{
		constexpr Matrix<4> matrix
		{
			3, -2, 0, 1,
			1, 4, 0, 1,
			0, 0, 2, 1,
			1, 1, 1, 1
		};

		constexpr Matrix<4> actual = matrix.CofactorMatrix();

		constexpr Matrix<4> expected
		{
			2, 1, 3, -6,
			4, 1, 5, -10,
			6, 2, 6, -14,
			-12, -4, -14, 28
		};
		ASSERT_EQ(expected, actual);
	}

	TEST(Matrix4x4FloatTest, Determinant)
	{
		constexpr Matrix<4> matrix
		{
			3, -2, 0, 1,
			1, 4, 0, 1,
			0, 0, 2, 1,
			1, 1, 1, 1
		};

		constexpr float actual = matrix.Determinant();

		constexpr float expected = -2;
		ASSERT_EQ(expected, actual);
	}
}
