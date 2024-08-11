#include "../src/Maths/Matrix.h"
#include <gtest/gtest.h>

namespace Engine3
{
	/// Primarily a test to validate row-major order is kept.
	TEST(MatrixTest, FunctionCallOperatorSquare)
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

	TEST(MatrixTest, FunctionCallOperatorNonSquare)
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

	TEST(MatrixTest, FunctionCallOperatorNonSquareOtherAspectRatiio)
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

	TEST(MatrixTest, IdentityMatrix3)
	{
		Matrix<3> expected =
		{
			1, 0, 0,
			0, 1, 0,
			0, 0, 1
		};

		ASSERT_EQ(Matrix<3>::IdentityMatrix(), expected);
	}

	TEST(MatrixTest, IdentityMatrix4)
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

	TEST(MatrixTest, Transpose3)
	{
		Matrix<3> actual
		{
			1, 2, 3,
			4, 5, 6,
			7, 8, 9
		};
		actual.Transpose();

		Matrix<3> expected
		{
			1, 4, 7,
			2, 5, 8,
			3, 6, 9
		};

		ASSERT_EQ(expected, actual);
	}

	TEST(MatrixTest, Transpose4x3)
	{
		Matrix<3, 4> actual =
			Matrix<4, 3>
			{
				1, 2, 3,
				4, 5, 6,
				7, 8, 9,
				10, 11, 12
			}.Transposed();

		Matrix<3, 4> expected
		{
			1, 4, 7, 10,
			2, 5, 8, 11,
			3, 6, 9, 12
		};

		ASSERT_EQ(expected, actual);
	}

	TEST(MatrixTest, ScalarMultiplicationAssignment)
	{
		Matrix<3> actual
		{
			1, 2, 3,
			4, 5, 6,
			7, 8, 9
		};

		actual *= 10;

		Matrix<3> expected
		{
			10, 20, 30,
			40, 50, 60,
			70, 80, 90
		};
		ASSERT_EQ(expected, actual);
	}

	TEST(MatrixTest, ScalarMultiplicationRight)
	{
		Matrix<3> actual = Matrix<3>
		{
			1, 2, 3,
			4, 5, 6,
			7, 8, 9
		} * 10;

		Matrix<3> expected
		{
			10, 20, 30,
			40, 50, 60,
			70, 80, 90
		};
		//ASSERT_EQ(expected, actual);
	}

	TEST(MatrixTest, ScalarMultiplicationLeft)
	{
		Matrix<3> actual = 10 * Matrix<3>
		{
			1, 2, 3,
			4, 5, 6,
			7, 8, 9
		};

		Matrix<3> expected
		{
			10, 20, 30,
			40, 50, 60,
			70, 80, 90
		};
		ASSERT_EQ(expected, actual);
	}

	TEST(MatrixTest, ScalarDivisionAssignment)
	{
		Matrix<3> actual
		{
			10, 20, 30,
			40, 50, 60,
			70, 80, 90
		};
		actual /= 10;

		Matrix<3> expected
		{
			1, 2, 3,
			4, 5, 6,
			7, 8, 9
		};
		ASSERT_EQ(expected, actual);
	}

	TEST(MatrixTest, ScalarDivisionRight)
	{
		Matrix<3> actual = Matrix<3>
		{
			10, 20, 30,
			40, 50, 60,
			70, 80, 90
		} / 10;

		Matrix<3> expected
		{
			1, 2, 3,
			4, 5, 6,
			7, 8, 9
		};
		ASSERT_EQ(expected, actual);
	}

	TEST(MatrixTest, GetRow)
	{
		Vector<3> actual = Matrix<3>
		{
			1, 2, 3,
			4, 5, 6,
			7, 8, 9
		}.GetRow(1);

		Vector<3> expected{4, 5, 6};
		ASSERT_EQ(expected, actual);
	}

	TEST(MatrixTest, GetColumn)
	{
		Vector<4> actual = Matrix<4>
		{
			1, 2, 3, 4,
			5, 6, 7, 8,
			9, 10, 11, 12,
			13, 14, 15, 16
		}.GetColumn(1);

		Vector<4> expected{2, 6, 10, 14};
		ASSERT_EQ(expected, actual);
	}

	TEST(MatrixTest, MatrixMultiplication3x3)
	{
		Matrix<3> a =
		{
			1, -5, 3,
			0, -2, 6,
			7, 2, -4
		};

		Matrix<3> b =
		{
			-8, 6, 1,
			7, 0, -3,
			2, 4, 5
		};

		Matrix<3> ab = a * b;

		Matrix<3> expected =
		{
			-37, 18, 31,
			-2, 24, 36,
			-50, 26, -19
		};
		ASSERT_EQ(expected, ab);
	}

	TEST(MatrixTest, MatrixMultiplicationNonSquare2x4by4x3)
	{
		Matrix<2, 4> a =
		{
			1, 2, 3, 4,
			5, 6, 7, 8
		};

		Matrix<4, 3> b =
		{
			1, 2, 3,
			4, 5, 6,
			7, 8, 9,
			10, 11, 12
		};

		Matrix<2, 3> ab = a * b;

		Matrix<2, 3> expected =
		{
			70, 80, 90,
			158, 184, 210
		};
		ASSERT_EQ(expected, ab);
	}

	TEST(MatrixTest, MatrixMultiplication2x2)
	{
		Matrix<2> a
		{
			1, -2,
			5, 0
		};

		Matrix<2> b
		{
			-3, 7,
			4, 1.f / 3.f
		};

		Matrix<2> ab = a * b;

		Matrix<2> expected =
		{
			-11, 6.f + 1.f / 3.f, -15, 35
		};
		ASSERT_EQ(expected, ab);
	}

	TEST(MatrixTest, MatrixMultiplicationVector3By3x3)
	{
		Vector<3> a
		{
			3, -1, 4
		};

		Matrix<3> b
		{
			-2, 6, 3,
			5, 7, -6,
			1, -4, 2
		};

		Vector<3> ab = a * b;

		Vector<3> expected =
		{
			-7, -5, 23
		};

		ASSERT_EQ(expected, ab);
	}

	TEST(MatrixTest, MatrixMultiplicationVector2By2x2)
	{
		Vector<2> a
		{
			3, 3
		};

		Matrix<2> b
		{
			6, -7,
			-4, 5
		};

		Vector<2> ab = a * b;

		Vector<2> expected =
		{
			6, -6
		};

		ASSERT_EQ(expected, ab);
	}

	TEST(MatrixTest, RowVectorMultiplication)
	{
		Matrix<2, 3> matrix =
		{
			1, 2, 3,
			4, 5, 6,
		};

		Vector<2> rowVector =
		{
			1, 2
		};

		Vector<3> actual = rowVector * matrix;

		Vector<3> expected =
		{
			9, 12, 15
		};
		ASSERT_EQ(expected, actual);
	}

	TEST(MatrixTest, ColumnVectorMultiplication)
	{
		Matrix<4, 3> matrix =
		{
			1, 2, 3,
			4, 5, 6,
			7, 8, 9,
			10, 11, 12
		};

		Vector<3> rowVector =
		{
			1, 2, 3
		};

		Vector<4> actual = matrix * rowVector;

		Vector<4> expected =
		{
			14, 32, 50, 68
		};
		ASSERT_EQ(expected, actual);
	}
}
