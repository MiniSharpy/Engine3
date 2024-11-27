#include "../../src/Maths/Maths.h"
#include <gtest/gtest.h>

namespace Engine3
{
	TEST(Angle, WrapAngle_InRange)
	{
		float actual = WrapAngle(DegreesToRadians(170.f));
		float expected = DegreesToRadians(170.f);

		EXPECT_FLOAT_EQ(actual, expected);
	}

	TEST(Angle, WrapAngle_ExceedUpperBound)
	{
		float actual = WrapAngle(DegreesToRadians(190.f));
		float expected = DegreesToRadians(-170.f);

		EXPECT_FLOAT_EQ(actual, expected);
	}

	TEST(Angle, WrapAngle_ExceedLowerBound)
	{
		float actual = WrapAngle(DegreesToRadians(-190.f));
		float expected = DegreesToRadians(170.f);

		EXPECT_FLOAT_EQ(actual, expected);
	}

	TEST(Angle, WrapAngle_AtLowerBound)
	{
		float actual = WrapAngle(DegreesToRadians(-180.f));
		float expected = DegreesToRadians(180.f);

		EXPECT_FLOAT_EQ(actual, expected);
	}

	TEST(Angle, WrapAngle_AtUpperBound)
	{
		float actual = WrapAngle(DegreesToRadians(180.f));
		float expected = DegreesToRadians(180.f);

		EXPECT_FLOAT_EQ(actual, expected);
	}

	TEST(Angle, WrapAngle_Difference)
	{
		float actual = WrapAngle(
			DegreesToRadians(45.f) -
			DegreesToRadians(720.f));
		float expected = DegreesToRadians(45.f);

		// Some annoying floating point precision errors occur that exceed the epsilon for EXPECT_FLOAT_EQ.
		EXPECT_NEAR(actual, expected, 0.00001);
	}
}
