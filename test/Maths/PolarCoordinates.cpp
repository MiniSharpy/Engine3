#include "../CustomMatchers.h"
#include "../../src/Maths/PolarCoordinates.h"
#include "gtest/gtest.h"
using testing::Pointwise;

namespace Engine3
{
	TEST(PolarCoordinatesFloat, CanonicalForm_ZeroDistance)
	{
		constexpr PolarCoordinates actual = PolarCoordinates{0.f, 100.f}.CanonicalForm();
		constexpr PolarCoordinates expected{0.f, 0.f};

		EXPECT_EQ(actual, expected);
	}

	TEST(PolarCoordinatesFloat, CanonicalForm_NegativeDistance)
	{
		PolarCoordinates actual = PolarCoordinates{-10.f, 0.f}.CanonicalForm();
		PolarCoordinates expected{10.f, std::numbers::pi_v<float>};

		EXPECT_NEAR(actual.Distance, expected.Distance, 0.00001);
		EXPECT_NEAR(actual.Angle, expected.Angle, 0.00001);
	}

	TEST(PolarCoordinatesFloat, CanonicalForm_AngleOutOfRange)
	{
		PolarCoordinates actual = PolarCoordinates{4.f, DegreesToRadians(207.f)}.CanonicalForm();
		PolarCoordinates expected{4.f, DegreesToRadians(-153.f)};

		EXPECT_NEAR(actual.Distance, expected.Distance, 0.00001);
		EXPECT_NEAR(actual.Angle, expected.Angle, 0.00001);
	}

	TEST(PolarCoordinatesFloat, CanonicalForm_NegativeDistanceAndAngleOutOfRange)
	{
		PolarCoordinates actual = PolarCoordinates{-5.f, DegreesToRadians(-720.f)}.CanonicalForm();
		PolarCoordinates expected{5.f, DegreesToRadians(180.f)};

		EXPECT_NEAR(actual.Distance, expected.Distance, 0.00001);
		EXPECT_NEAR(actual.Angle, expected.Angle, 0.00001);
	}

	TEST(PolarCoordinatesFloat, ToCartesian)
	{
		Vector<2> actual = PolarCoordinates{5.f, DegreesToRadians(180.f)}.ToVector2();
		Vector<2> expected{-5, 0};
		EXPECT_THAT(actual, Pointwise(NearWithPrecision(1e-05), expected));
	}

	TEST(PolarCoordinatesFloat, ToCartesian_ZeroAngle)
	{
		Vector<2> actual = PolarCoordinates{3.f, 0.f}.ToVector2();
		Vector<2> expected{3, 0};

		EXPECT_EQ(actual, expected);
	}

	TEST(PolarCoordinatesFloat, ToCartesian_ZeroDistance)
	{
		Vector<2> actual = PolarCoordinates{0.f, DegreesToRadians(-720.f)}.ToVector2();
		Vector<2> expected{0, 0};

		EXPECT_EQ(actual, expected);
	}

	TEST(PolarCoordinatesFloat, ToCartesian_NonCanoncial)
	{
		Vector<2> actual = PolarCoordinates{-5.f, DegreesToRadians(-720.f)}.ToVector2();
		Vector<2> expected{-5, 0};

		EXPECT_THAT(actual, Pointwise(NearWithPrecision(1e-05), expected));
	}
}
