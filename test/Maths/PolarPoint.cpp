#include "../CustomMatchers.h"
#include "../../src/Maths/PolarCoordinates.h"
#include "gtest/gtest.h"
using testing::Pointwise;

namespace Engine3
{
	TEST(PolarPointFloat, CanonicalForm_ZeroDistance)
	{
		constexpr PolarPoint actual = PolarPoint{0.f, 100.f}.CanonicalForm();
		constexpr PolarPoint expected{0.f, 0.f};

		EXPECT_EQ(actual, expected);
	}

	TEST(PolarPointFloat, CanonicalForm_NegativeDistance)
	{
		PolarPoint actual = PolarPoint{-10.f, 0.f}.CanonicalForm();
		PolarPoint expected{10.f, std::numbers::pi_v<float>};

		EXPECT_NEAR(actual.Distance, expected.Distance, 0.00001);
		EXPECT_NEAR(actual.Angle, expected.Angle, 0.00001);
	}

	TEST(PolarPointFloat, CanonicalForm_AngleOutOfRange)
	{
		PolarPoint actual = PolarPoint{4.f, DegreesToRadians(207.f)}.CanonicalForm();
		PolarPoint expected{4.f, DegreesToRadians(-153.f)};

		EXPECT_NEAR(actual.Distance, expected.Distance, 0.00001);
		EXPECT_NEAR(actual.Angle, expected.Angle, 0.00001);
	}

	TEST(PolarPointFloat, CanonicalForm_NegativeDistanceAndAngleOutOfRange)
	{
		PolarPoint actual = PolarPoint{-5.f, DegreesToRadians(-720.f)}.CanonicalForm();
		PolarPoint expected{5.f, DegreesToRadians(180.f)};

		EXPECT_NEAR(actual.Distance, expected.Distance, 0.00001);
		EXPECT_NEAR(actual.Angle, expected.Angle, 0.00001);
	}

	TEST(PolarPointFloat, ToCartesian)
	{
		Vector<2> actual = PolarPoint{5.f, DegreesToRadians(180.f)}.ToCartesian();
		Vector<2> expected{-5, 0};
		EXPECT_THAT(actual, Pointwise(NearWithPrecision(1e-05), expected));
	}

	TEST(PolarPointFloat, ToCartesian_ZeroAngle)
	{
		Vector<2> actual = PolarPoint{3.f, 0.f}.ToCartesian();
		Vector<2> expected{3, 0};

		EXPECT_EQ(actual, expected);
	}

	TEST(PolarPointFloat, ToCartesian_ZeroDistance)
	{
		Vector<2> actual = PolarPoint{0.f, DegreesToRadians(-720.f)}.ToCartesian();
		Vector<2> expected{0, 0};

		EXPECT_EQ(actual, expected);
	}

	TEST(PolarPointFloat, ToCartesian_NonCanoncial)
	{
		Vector<2> actual = PolarPoint{-5.f, DegreesToRadians(-720.f)}.ToCartesian();
		Vector<2> expected{-5, 0};

		EXPECT_THAT(actual, Pointwise(NearWithPrecision(1e-05), expected));
	}

	TEST(PolarPointFloat, FromCartesian)
	{
		PolarPoint actual{Vector<2>{-3, 4}};
		PolarPoint expected{5.f, DegreesToRadians(126.87f)};

		EXPECT_NEAR(actual.Distance, expected.Distance, 0.001);
		EXPECT_NEAR(actual.Angle, expected.Angle, 0.001);
	}

	TEST(PolarPointFloat, FromCartesian_Origin)
	{
		PolarPoint actual{Vector<2>{0, 0}};
		PolarPoint expected{0.f, 0.f};

		EXPECT_EQ(actual, expected);
	}
}
