#include "../../src/Maths/PolarCoordinates.h"
#include "../CustomMatchers.h"
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

		EXPECT_NEAR(actual.Radius, expected.Radius, 0.00001);
		EXPECT_NEAR(actual.Angle, expected.Angle, 0.00001);
	}

	TEST(PolarCoordinatesFloat, CanonicalForm_AngleOutOfRange)
	{
		PolarCoordinates actual = PolarCoordinates{4.f, DegreesToRadians(207.f)}.CanonicalForm();
		PolarCoordinates expected{4.f, DegreesToRadians(-153.f)};

		EXPECT_NEAR(actual.Radius, expected.Radius, 0.00001);
		EXPECT_NEAR(actual.Angle, expected.Angle, 0.00001);
	}

	TEST(PolarCoordinatesFloat, CanonicalForm_NegativeDistanceAndAngleOutOfRange)
	{
		PolarCoordinates actual = PolarCoordinates{-5.f, DegreesToRadians(-720.f)}.CanonicalForm();
		PolarCoordinates expected{5.f, DegreesToRadians(180.f)};

		EXPECT_NEAR(actual.Radius, expected.Radius, 0.00001);
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

	TEST(CylindricalCoordinatesFloat, CanonicalForm_ZeroDistance)
	{
		constexpr CylindricalCoordinates actual = CylindricalCoordinates{0.f, 100.f, 5.f}.CanonicalForm();
		constexpr CylindricalCoordinates expected{0.f, 0.f, 5.f};

		EXPECT_EQ(actual, expected);
	}

	TEST(CylindricalCoordinatesFloat, CanonicalForm_NegativeDistance)
	{
		CylindricalCoordinates actual = CylindricalCoordinates{-10.f, 0.f, 5.f}.CanonicalForm();
		CylindricalCoordinates expected{10.f, std::numbers::pi_v<float>, 5.f};

		EXPECT_NEAR(actual.Radius, expected.Radius, 0.00001);
		EXPECT_NEAR(actual.Angle, expected.Angle, 0.00001);
		EXPECT_NEAR(actual.Z, expected.Z, 0.00001);
	}

	TEST(CylindricalCoordinatesFloat, CanonicalForm_AngleOutOfRange)
	{
		CylindricalCoordinates actual = CylindricalCoordinates{4.f, DegreesToRadians(207.f), 5.f}.CanonicalForm();
		CylindricalCoordinates expected{4.f, DegreesToRadians(-153.f), 5.f};

		EXPECT_NEAR(actual.Radius, expected.Radius, 0.00001);
		EXPECT_NEAR(actual.Angle, expected.Angle, 0.00001);
		EXPECT_NEAR(actual.Z, expected.Z, 0.00001);
	}

	TEST(CylindricalCoordinatesFloat, CanonicalForm_NegativeRadiusAndAngleOutOfRange)
	{
		CylindricalCoordinates actual = CylindricalCoordinates{-5.f, DegreesToRadians(-720.f), 5.f}.CanonicalForm();
		CylindricalCoordinates expected{5.f, DegreesToRadians(180.f), 5.f};

		EXPECT_NEAR(actual.Radius, expected.Radius, 0.00001);
		EXPECT_NEAR(actual.Angle, expected.Angle, 0.00001);
		EXPECT_NEAR(actual.Z, expected.Z, 0.00001);
	}

	TEST(CylindricalCoordinatesFloat, ToCartesian)
	{
		Vector<3> actual = CylindricalCoordinates{5.f, DegreesToRadians(180.f), 5.f}.ToVector3();
		Vector<3> expected{-5, 0, 5.f};
		EXPECT_THAT(actual, Pointwise(NearWithPrecision(1e-05), expected));
	}

	TEST(CylindricalCoordinatesFloat, ToCartesian_ZeroAngle)
	{
		Vector<3> actual = CylindricalCoordinates{3.f, 0.f, 5.f}.ToVector3();
		Vector<3> expected{3, 0, 5.f};

		EXPECT_EQ(actual, expected);
	}

	TEST(CylindricalCoordinatesFloat, ToCartesian_ZeroRadius)
	{
		Vector<3> actual = CylindricalCoordinates{0.f, DegreesToRadians(-720.f), 5.f}.ToVector3();
		Vector<3> expected{0, 0, 5.f};

		EXPECT_EQ(actual, expected);
	}

	TEST(CylindricalCoordinatesFloat, ToCartesian_NonCanoncial)
	{
		Vector<3> actual = CylindricalCoordinates{-5.f, DegreesToRadians(-720.f), 5.f}.ToVector3();
		Vector<3> expected{-5, 0, 5.f};

		EXPECT_THAT(actual, Pointwise(NearWithPrecision(1e-05), expected));
	}
}
