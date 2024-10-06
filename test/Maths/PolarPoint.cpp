#include "../../src/Maths/PolarPoint.h"
#include "../CustomMatchers.h"
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

		ASSERT_NEAR(actual.Distance, expected.Distance, 0.00001);
		ASSERT_NEAR(actual.Angle, expected.Angle, 0.00001);
	}

	TEST(PolarPointFloat, CanonicalForm_AngleOutOfRange)
	{
		PolarPoint actual = PolarPoint{4.f, DegreesToRadians(207.f)}.CanonicalForm();
		PolarPoint expected{4.f, DegreesToRadians(-153.f)};

		ASSERT_NEAR(actual.Distance, expected.Distance, 0.00001);
		ASSERT_NEAR(actual.Angle, expected.Angle, 0.00001);
	}

	TEST(PolarPointFloat, CanonicalForm_NegativeDistanceAndAngleOutOfRange)
	{
		PolarPoint actual = PolarPoint{-5.f, DegreesToRadians(-720.f)}.CanonicalForm();
		PolarPoint expected{5.f, DegreesToRadians(180.f)};

		ASSERT_NEAR(actual.Distance, expected.Distance, 0.00001);
		ASSERT_NEAR(actual.Angle, expected.Angle, 0.00001);
	}
}
