#include "../CustomMatchers.h"
#include "../../src/Maths/Maths.h"
#include "../../src/Maths/Matrix.h"
#include <numbers>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
using testing::Pointwise;

namespace Engine3
{
	TEST(Matrix4x4Float, Invert)
	{
		Matrix<4> actual =
			Matrix<4>{
				-0.1495f, -0.1986f, -0.9685f, 0.f,
				-0.8256f, 0.5640f, 0.0117f, 0.f,
				-0.5439f, -0.8015f, 0.2484f, 0.f,
				1.7928f, -5.3116f, 8.0151f, 1.f
			}.Invert();

		Matrix<4> expected
		{
			-0.1495f, -0.8258f, -0.5440f, 0.f,
			-0.1988f, 0.5640f, -0.8015f, 0.f,
			-0.9687f, 0.0118f, 0.2483f, 0.f,
			6.9764f, 4.3817f, -5.2724f, 1.f
		};

		EXPECT_THAT(actual, Pointwise(NearWithPrecision(0.0001), expected));
	}

	TEST(Matrix4x4Float, Inverted)
	{
		Matrix<4> actual =
			Matrix<4>{
				-0.1495f, -0.1986f, -0.9685f, 0.f,
				-0.8256f, 0.5640f, 0.0117f, 0.f,
				-0.5439f, -0.8015f, 0.2484f, 0.f,
				1.7928f, -5.3116f, 8.0151f, 1.f
			}.Inverted();

		Matrix<4> expected
		{
			-0.1495f, -0.8258f, -0.5440f, 0.f,
			-0.1988f, 0.5640f, -0.8015f, 0.f,
			-0.9687f, 0.0118f, 0.2483f, 0.f,
			6.9764f, 4.3817f, -5.2724f, 1.f
		};

		EXPECT_THAT(actual, Pointwise(NearWithPrecision(0.0001), expected));
	}

	TEST(Matrix4x4Float, Translation)
	{
		Matrix<4> actual = Matrix<4>::Translation(4, 2, 3);
		Matrix<4> expected =
		{
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			4, 2, 3, 1
		};

		EXPECT_EQ(actual, expected);
	}

	TEST(Matrix4x4Float, RotationAndTranslation)
	{
		Matrix<4> actual = Matrix<4>::RotationAboutX(DegreesToRadians(20.f)) * Matrix<4>::Translation(4, 2, 3);
		Matrix<4> expected =
		{
			1.f, 0.f, 0.f, 0.f,
			0.f, 0.940f, 0.342f, 0.f,
			0.f, -0.342f, 0.940f, 0.f,
			4.f, 2.f, 3.f, 1.f
		};

		EXPECT_THAT(actual, Pointwise(NearWithPrecision(0.001), expected));
	}

	TEST(Matrix4x4Float, TranslationAndRotation)
	{
		Matrix<4> actual = Matrix<4>::Translation(4, 2, 3) * Matrix<4>::RotationAboutX(DegreesToRadians(20.f));
		Matrix<4> expected =
		{
			1.f, 0.f, 0.f, 0.f,
			0.f, 0.940f, 0.342f, 0.f,
			0.f, -0.342f, 0.940f, 0.f,
			4.f, 0.853f, 3.503f, 1.f
		};

		EXPECT_THAT(actual, Pointwise(NearWithPrecision(0.001), expected));
	}
}
