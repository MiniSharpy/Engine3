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

	TEST(Matrix3x3Test, IdentityMatrix)
	{
		Matrix<3> expected =
		{
			1, 0, 0,
			0, 1, 0,
			0, 0, 1
		};

		ASSERT_EQ(Matrix<3>::IdentityMatrix(), expected);
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

	TEST(Matrix3x3Test, Transposed)
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

	TEST(Matrix4x3Test, Transposed)
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

	TEST(Matrix3x3Test, ScalarMultiplicationAssignment)
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

	TEST(Matrix3x3Test, ScalarMultiplicationRight)
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

	TEST(Matrix3x3Test, ScalarMultiplicationLeft)
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

	TEST(Matrix3x3Test, ScalarDivisionAssignment)
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

	TEST(Matrix3x3Test, ScalarDivisionRight)
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

	TEST(MatrixSquareTest, GetRow)
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

	TEST(MatrixSquareTest, GetColumn)
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

	TEST(Matrix3x3Test, MatrixMultiplication)
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

	TEST(MatrixNonSquareTest, MatrixMultiplication2x4By4x3)
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

	TEST(Matrix2x2Test, MatrixMultiplication)
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

	TEST(Matrix3x3Test, MatrixMultiplicationBy3x3)
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

	TEST(Matrix2x2Test, MatrixMultiplicationBy2x2)
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

	TEST(MatrixNonSquareTest, RowVectorMultiplication)
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

	TEST(MatrixNonSquareTest, ColumnVectorMultiplication)
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

	TEST(Matrix4x4FloatTest, ScalingCardinal)
	{
		Matrix<4> actual = Matrix<4>::ScalingAlongCardinalAxes(2, 2, 2, 2);
		Matrix<4> expected
		{
			2, 0, 0, 0,
			0, 2, 0, 0,
			0, 0, 2, 0,
			0, 0, 0, 2
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
}
