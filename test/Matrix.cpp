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
		Matrix<3> actual =
		{
			1, 0, 0,
			0, 1, 0,
			0, 0, 1
		};

		ASSERT_EQ(Matrix<3>::IdentityMatrix(), actual);
	}

	TEST(MatrixTest, IdentityMatrix4)
	{
		Matrix<4> actual =
		{
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};

		ASSERT_EQ(Matrix<4>::IdentityMatrix(), actual);
	}
}
