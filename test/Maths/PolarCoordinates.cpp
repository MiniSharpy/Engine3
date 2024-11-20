#include "../../src/Maths/PolarCoordinates.h"
#include "../CustomMatchers.h"
#include "gtest/gtest.h"
using testing::Pointwise;

namespace
{
	constexpr float Pi = std::numbers::pi_v<float>;
}

namespace Engine3
{
	TEST(PolarCoordinatesFloat, CanonicalForm_ZeroDistance)
	{
		constexpr PolarCoordinates2D actual = PolarCoordinates2D{0.f, 100.f}.CanonicalForm();
		constexpr PolarCoordinates2D expected{0.f, 0.f};

		EXPECT_EQ(actual, expected);
	}

	TEST(PolarCoordinatesFloat, CanonicalForm_NegativeDistance)
	{
		PolarCoordinates2D actual = PolarCoordinates2D{-10.f, 0.f}.CanonicalForm();
		PolarCoordinates2D expected{10.f, Pi};

		EXPECT_NEAR(actual.Radius, expected.Radius, 0.00001);
		EXPECT_NEAR(actual.Angle, expected.Angle, 0.00001);
	}

	TEST(PolarCoordinatesFloat, CanonicalForm_AngleOutOfRange)
	{
		PolarCoordinates2D actual = PolarCoordinates2D{4.f, DegreesToRadians(207.f)}.CanonicalForm();
		PolarCoordinates2D expected{4.f, DegreesToRadians(-153.f)};

		EXPECT_NEAR(actual.Radius, expected.Radius, 0.00001);
		EXPECT_NEAR(actual.Angle, expected.Angle, 0.00001);
	}

	TEST(PolarCoordinatesFloat, CanonicalForm_NegativeDistanceAndAngleOutOfRange)
	{
		PolarCoordinates2D actual = PolarCoordinates2D{-5.f, DegreesToRadians(-720.f)}.CanonicalForm();
		PolarCoordinates2D expected{5.f, DegreesToRadians(180.f)};

		EXPECT_NEAR(actual.Radius, expected.Radius, 0.00001);
		EXPECT_NEAR(actual.Angle, expected.Angle, 0.00001);
	}

	TEST(PolarCoordinatesFloat, ToCartesian)
	{
		Vector<2> actual = PolarCoordinates2D{5.f, DegreesToRadians(180.f)}.ToVector2();
		Vector<2> expected{-5, 0};
		EXPECT_THAT(actual, Pointwise(NearWithPrecision(1e-05), expected));
	}

	TEST(PolarCoordinatesFloat, ToCartesian_ZeroAngle)
	{
		Vector<2> actual = PolarCoordinates2D{3.f, 0.f}.ToVector2();
		Vector<2> expected{3, 0};

		EXPECT_EQ(actual, expected);
	}

	TEST(PolarCoordinatesFloat, ToCartesian_ZeroDistance)
	{
		Vector<2> actual = PolarCoordinates2D{0.f, DegreesToRadians(-720.f)}.ToVector2();
		Vector<2> expected{0, 0};

		EXPECT_EQ(actual, expected);
	}

	TEST(PolarCoordinatesFloat, ToCartesian_NonCanoncial)
	{
		Vector<2> actual = PolarCoordinates2D{-5.f, DegreesToRadians(-720.f)}.ToVector2();
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
		CylindricalCoordinates expected{10.f, Pi, 5.f};

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

	TEST(SphericalCoordinatesFloat, CanonicalForm_ZeroDistance)
	{
		constexpr SphericalCoordinates actual = SphericalCoordinates{0.f, 100.f, 100.f}.CanonicalForm();
		constexpr SphericalCoordinates expected{0.f, 0.f, 0.f};

		EXPECT_EQ(actual, expected);
	}

	TEST(SphericalCoordinatesFloat, CanonicalForm_NegativeDistance)
	{
		SphericalCoordinates actual = SphericalCoordinates{-10.f, 0.f, Pi / 2}.CanonicalForm();
		SphericalCoordinates expected{10.f, Pi, -Pi / 2};

		EXPECT_NEAR(actual.Radius, expected.Radius, 0.00001);
		EXPECT_NEAR(actual.Heading, expected.Heading, 0.00001);
		EXPECT_NEAR(actual.Pitch, expected.Pitch, 0.00001);
	}

	TEST(SphericalCoordinatesFloat, CanonicalForm_PitchOutOfRange)
	{
		SphericalCoordinates actual = SphericalCoordinates
		{
			4.f,
			Pi / 3,
			(3 * Pi) / 4,
		}.CanonicalForm();

		SphericalCoordinates expected
		{
			4.f,
			-2 * Pi / 3,
			Pi / 4
		};

		EXPECT_NEAR(actual.Radius, expected.Radius, 0.00001);
		EXPECT_NEAR(actual.Heading, expected.Heading, 0.00001);
		EXPECT_NEAR(actual.Pitch, expected.Pitch, 0.00001);
	}

	TEST(SphericalCoordinatesFloat, CanonicalForm_HeadingOutOfRange)
	{
		SphericalCoordinates actual = SphericalCoordinates
		{
			8.f,
			(9 * Pi) / 4,
			Pi / 6,
		}.CanonicalForm();

		SphericalCoordinates expected
		{
			8.f,
			Pi / 4,
			Pi / 6
		};

		EXPECT_NEAR(actual.Radius, expected.Radius, 0.00001);
		EXPECT_NEAR(actual.Heading, expected.Heading, 0.00001);
		EXPECT_NEAR(actual.Pitch, expected.Pitch, 0.00001);
	}

	TEST(SphericalCoordinatesFloat, ToCartesian)
	{
		Vector<3> actual = SphericalCoordinates{4.f, Pi / 3, 3 * Pi / 4}.ToVector3();
		Vector<3> expected{-std::sqrt(6.f), -2.f * std::sqrt(2.f), -std::sqrt(2.f)};
		EXPECT_THAT(actual, Pointwise(NearWithPrecision(1e-05), expected));
	}
}
