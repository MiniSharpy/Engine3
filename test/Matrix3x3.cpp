#include "../src/Maths/Maths.h"
#include "../src/Maths/Matrix.h"
#include <gtest/gtest.h>

namespace Engine3
{
	TEST(Matrix3x3FloatTest, IdentityMatrix)
	{
		Matrix<3> expected =
		{
			1, 0, 0,
			0, 1, 0,
			0, 0, 1
		};

		ASSERT_EQ(Matrix<3>::IdentityMatrix(), expected);
	}

	TEST(Matrix3x3FloatTest, Transpose)
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

	TEST(Matrix3x3FloatTest, ScalarMultiplicationAssignment)
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

	TEST(Matrix3x3FloatTest, ScalarMultiplicationRight)
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
		ASSERT_EQ(expected, actual);
	}

	TEST(Matrix3x3FloatTest, ScalarMultiplicationLeft)
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

	TEST(Matrix3x3FloatTest, ScalarDivisionAssignment)
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

	TEST(Matrix3x3FloatTest, ScalarDivisionRight)
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

	TEST(Matrix3x3FloatTest, MatrixMultiplication)
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

	TEST(Matrix3x3FloatTest, VeectorMultiplication)
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

	TEST(Matrix3x3FloatTest, RotationX)
	{
		Matrix actual{Matrix<3>::RotationAboutX(DegreesToRadians(-22.f))};
		Matrix<3> expected
		{
			1.f, 0.f, 0.f,
			0.f, 0.9271838546f, -0.3746065934f,
			0.f, 0.3746065934f, 0.9271838546f
		};
		ASSERT_EQ(expected, actual);
	}

	TEST(Matrix3x3FloatTest, RotationY)\
	{
		Matrix actual{Matrix<3>::RotationAboutY(DegreesToRadians(30.f))};
		Matrix<3> expected
		{
			0.8660254038f, 0, -0.5f,
			0, 1, 0,
			0.5, 0, 0.8660254038f
		};
		ASSERT_EQ(expected, actual);
	}

	TEST(Matrix3x3FloatTest, RotationZ)
	{
		Matrix actual{Matrix<3>::RotationAboutZ(DegreesToRadians(45.f))};
		Matrix<3> expected
		{
			0.707106782f, 0.7071067812f, 0,
			-0.7071067812f, 0.7071067812f, 0,
			0, 0, 1
		};
		ASSERT_EQ(expected, actual);
	}

	TEST(Matrix3x3FloatTest, RotationArbitraryAxis)
	{
		Vector<3> axis{0.267f, -0.535f, 0.802f};
		axis.Normalise();
		Matrix<3> actual{Matrix<3>::RotationAboutAxis(axis, DegreesToRadians(-15.f))};

		Matrix<3> expected
		{
			0.968f, -0.212f, -0.131f,
			0.203f, 0.976f, -0.084f,
			0.146f, 0.054f, 0.988f
		};

		EXPECT_NEAR(actual[0], expected[0], 0.001);
		EXPECT_NEAR(actual[1], expected[1], 0.001);
		EXPECT_NEAR(actual[2], expected[2], 0.001);
		EXPECT_NEAR(actual[3], expected[3], 0.001);
		EXPECT_NEAR(actual[4], expected[4], 0.001);
		EXPECT_NEAR(actual[5], expected[5], 0.001);
		EXPECT_NEAR(actual[6], expected[6], 0.001);
		EXPECT_NEAR(actual[7], expected[7], 0.001);
		EXPECT_NEAR(actual[8], expected[8], 0.001);
	}

	TEST(Matrix3x3FloatTest, ScalingCardinal)
	{
		Matrix<3> actual = Matrix<3>::ScalingAlongCardinalAxes(2, 2, 2);
		Matrix<3> expected
		{
			2, 0, 0,
			0, 2, 0,
			0, 0, 2
		};

		ASSERT_EQ(actual, expected);
	}

	TEST(Matrix3x3FloatTest, ScalingAxis)
	{
		Vector<3> axis{0.267f, -0.535f, 0.802f};
		axis.Normalise();
		Matrix<3> actual = Matrix<3>::ScalingAlongAxis(axis, 5);
		Matrix<3> expected
		{
			1.285f, -0.571f, 0.857f,
			-0.571f, 2.145f, -1.716f,
			0.857f, -1.716f, 3.573f
		};

		EXPECT_NEAR(actual[0], expected[0], 0.01);
		EXPECT_NEAR(actual[1], expected[1], 0.01);
		EXPECT_NEAR(actual[2], expected[2], 0.01);
		EXPECT_NEAR(actual[3], expected[3], 0.01);
		EXPECT_NEAR(actual[4], expected[4], 0.01);
		EXPECT_NEAR(actual[5], expected[5], 0.01);
		EXPECT_NEAR(actual[6], expected[6], 0.01);
		EXPECT_NEAR(actual[7], expected[7], 0.01);
		EXPECT_NEAR(actual[8], expected[8], 0.01);
	}

	TEST(Matrix3x3FloatTest, OrthographicProjectXY)
	{
		Matrix<3> actual = Matrix<3>::ProjectionOntoPlaneXY();
		Matrix<3> expected =
		{
			1, 0, 0,
			0, 1, 0,
			0, 0, 0
		};

		ASSERT_EQ(actual, expected);
	}

	TEST(Matrix3x3FloatTest, OrthographicProjectXZ)
	{
		Matrix<3> actual = Matrix<3>::ProjectionOntoPlaneXZ();
		Matrix<3> expected =
		{
			1, 0, 0,
			0, 0, 0,
			0, 0, 1
		};

		ASSERT_EQ(actual, expected);
	}

	TEST(Matrix3x3FloatTest, OrthographicProjectYZ)
	{
		Matrix<3> actual = Matrix<3>::ProjectionOntoPlaneYZ();
		Matrix<3> expected =
		{
			0, 0, 0,
			0, 1, 0,
			0, 0, 1
		};

		ASSERT_EQ(actual, expected);
	}

	TEST(Matrix3x3FloatTest, OrthographicProjectAxis)
	{
		Vector<3> axis{0.267f, -0.535f, 0.802f};
		axis.Normalise();
		Matrix<3> actual = Matrix<3>::ProjectionOntoPlane(axis);
		Matrix<3> expected =
		{
			0.928711f, 0.142845f, -0.214134f,
			0.142845f, 0.713775f, 0.42907f,
			-0.214134f, 0.42907f, 0.356796f
		};

		EXPECT_NEAR(actual[0], expected[0], 0.01);
		EXPECT_NEAR(actual[1], expected[1], 0.01);
		EXPECT_NEAR(actual[2], expected[2], 0.01);
		EXPECT_NEAR(actual[3], expected[3], 0.01);
		EXPECT_NEAR(actual[4], expected[4], 0.01);
		EXPECT_NEAR(actual[5], expected[5], 0.01);
		EXPECT_NEAR(actual[6], expected[6], 0.01);
		EXPECT_NEAR(actual[7], expected[7], 0.01);
		EXPECT_NEAR(actual[8], expected[8], 0.01);
	}

	TEST(Matrix3x3FloatTest, Reflection)
	{
		Vector<3> axis{0.267f, -0.535f, 0.802f};
		axis.Normalise();
		Matrix<3> actual = Matrix<3>::Reflection(axis);
		Matrix<3> expected =
		{
			0.857422f, 0.28569f, -0.428278f,
			0.28569f, 0.42755f, 0.85814f,
			-0.428268f, 0.85814f, -0.286408f
		};

		EXPECT_NEAR(actual[0], expected[0], 0.01);
		EXPECT_NEAR(actual[1], expected[1], 0.01);
		EXPECT_NEAR(actual[2], expected[2], 0.01);
		EXPECT_NEAR(actual[3], expected[3], 0.01);
		EXPECT_NEAR(actual[4], expected[4], 0.01);
		EXPECT_NEAR(actual[5], expected[5], 0.01);
		EXPECT_NEAR(actual[6], expected[6], 0.01);
		EXPECT_NEAR(actual[7], expected[7], 0.01);
		EXPECT_NEAR(actual[8], expected[8], 0.01);
	}

	TEST(Matrix3x3FloatTest, ShearXInProportionToY)
	{
		Matrix<3> transform = Matrix<3>::Shearing(1, 0, 0, 0, 0, 0);
		Vector<3> point{2, 3, 4};

		Vector<3> actual{5, 3, 4};
		ASSERT_EQ(transform * point, actual);
	}

	TEST(Matrix3x3FloatTest, ShearXInProportionToZ)
	{
		Matrix<3> transform = Matrix<3>::Shearing(0, 1, 0, 0, 0, 0);
		Vector<3> point{2, 3, 4};

		Vector<3> actual{6, 3, 4};
		ASSERT_EQ(transform * point, actual);
	}

	TEST(Matrix3x3FloatTest, ShearYInProportionToX)
	{
		Matrix<3> transform = Matrix<3>::Shearing(0, 0, 1, 0, 0, 0);
		Vector<3> point{2, 3, 4};

		Vector<3> actual{2, 5, 4};
		ASSERT_EQ(transform * point, actual);
	}

	TEST(Matrix3x3FloatTest, ShearYInProportionToZ)
	{
		Matrix<3> transform = Matrix<3>::Shearing(0, 0, 0, 1, 0, 0);
		Vector<3> point{2, 3, 4};

		Vector<3> actual{2, 7, 4};
		ASSERT_EQ(transform * point, actual);
	}

	TEST(Matrix3x3FloatTest, ShearZInProportionToX)
	{
		Matrix<3> transform = Matrix<3>::Shearing(0, 0, 0, 0, 1, 0);
		Vector<3> point{2, 3, 4};

		Vector<3> actual{2, 3, 6};
		ASSERT_EQ(transform * point, actual);
	}

	TEST(Matrix3x3FloatTest, ShearZInProportionToY)
	{
		Matrix<3> transform = Matrix<3>::Shearing(0, 0, 0, 0, 0, 1);
		Vector<3> point{2, 3, 4};

		Vector<3> actual{2, 3, 7};
		ASSERT_EQ(transform * point, actual);
	}

	TEST(Matrix3x3FloatTest, Determinant)
	{
		Matrix<3> matrix
		{
			3, -2, 0,
			1, 4, 0,
			0, 0, 2
		};
		float actual = matrix.Determinant();

		float expected = 28;
		ASSERT_EQ(expected, actual);
	}

	TEST(Matrix3x3FloatTest, Submatrix)
	{
		Matrix<3> matrix
		{
			3, -2, 0,
			1, 4, 0,
			0, 0, 2
		};
		Matrix<2> actual = matrix.Submatrix(1, 1);

		Matrix<2> expected
		{
			3, 0,
			0, 2
		};
		ASSERT_EQ(expected, actual);
	}

	TEST(Matrix3x3FloatTest, CofactorMatrix)
	{
		Matrix<3> actual =
			Matrix<3>
			{
				3, -2, 0,
				1, 4, 0,
				0, 0, 2
			}.CofactorMatrix();

		Matrix<3> expected
		{
			8, -2, 0,
			4, 6, 0,
			0, 0, 14
		};

		ASSERT_EQ(expected, actual);
	}

	TEST(Matrix3x3FloatTest, Adjoint)
	{
		Matrix<3> actual =
			Matrix<3>
			{
				3, -2, 0,
				1, 4, 0,
				0, 0, 2
			}.Adjoint();

		Matrix<3> expected
		{
			8, 4, 0,
			-2, 6, 0,
			0, 0, 14
		};

		ASSERT_EQ(expected, actual);
	}

	TEST(Matrix3x3FloatTest, Inverted)
	{
		Matrix<3> actual =
			Matrix<3>
			{
				3, -2, 0,
				1, 4, 0,
				0, 0, 2
			}.Inverted();

		Matrix<3> expected
		{
			8 / 28.f, 4 / 28.f, 0,
			-2 / 28.f, 6 / 28.f, 0,
			0, 0, 14 / 28.f
		};

		ASSERT_EQ(expected, actual);
	}

	TEST(Matrix3x3FloatTest, Inverse)
	{
		Matrix<3> actual =
			Matrix<3>
			{
				3, -2, 0,
				1, 4, 0,
				0, 0, 2
			};
		actual.Invert();

		Matrix<3> expected
		{
			8 / 28.f, 4 / 28.f, 0,
			-2 / 28.f, 6 / 28.f, 0,
			0, 0, 14 / 28.f
		};

		ASSERT_EQ(expected, actual);
	}

	TEST(Matrix3x3FloatTest, IsOrthogonal)
	{
		constexpr Vector<3> row1 = Vector<3>::Up();
		constexpr Vector<3> row2 = Vector<3>::Right();
		constexpr Vector<3> row3 = Vector<3>::Forward();

		bool actual = Matrix<3>
		{
			row1.X(), row1.Y(), row1.Z(),
			row2.X(), row2.Y(), row2.Z(),
			row3.X(), row3.Y(), row3.Z(),
		}.IsOrthogonal();

		bool expected = true;
		ASSERT_EQ(expected, actual);
	}
}
