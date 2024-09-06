#include "../src/Maths/Vector.h"
#include <gtest/gtest.h>

namespace Engine3
{
	TEST(VectorTest, ZeroVector)
	{
		Vector<2> actual{0.f, 0.f};

		ASSERT_EQ(Vector<2>::Zero(), actual);
	}

	TEST(VectorTest, Right2)
	{
		Vector<2> actual{1.f, 0.f};

		ASSERT_EQ(Vector<2>::Right(), actual);
	}

	TEST(VectorTest, Left2)
	{
		Vector<2> actual{-1.f, 0.f};

		ASSERT_EQ(Vector<2>::Left(), actual);
	}

	TEST(VectorTest, Up2)
	{
		Vector<2> actual{0.f, 1.f};

		ASSERT_EQ(Vector<2>::Up(), actual);
	}

	TEST(VectorTest, Down2)
	{
		Vector<2> actual{0.f, -1.f};

		ASSERT_EQ(Vector<2>::Down(), actual);
	}

	TEST(VectorTest, Right3)
	{
		Vector<3> actual{1.f, 0.f, 0.f};

		ASSERT_EQ(Vector<3>::Right(), actual);
	}

	TEST(VectorTest, Left3)
	{
		Vector<3> actual{-1.f, 0.f, 0.f};

		ASSERT_EQ(Vector<3>::Left(), actual);
	}

	TEST(VectorTest, Up3)
	{
		Vector<3> actual{0.f, 1.f, 0.f};

		ASSERT_EQ(Vector<3>::Up(), actual);
	}

	TEST(VectorTest, Down3)
	{
		Vector<3> actual{0.f, -1.f, 0.f};

		ASSERT_EQ(Vector<3>::Down(), actual);
	}

	TEST(VectorTest, Forward)
	{
		Vector<3> actual{0.f, 0.f, 1.f};

		ASSERT_EQ(Vector<3>::Forward(), actual);
	}

	TEST(VectorTest, Back)
	{
		Vector<3> actual{0.f, 0.f, -1.f};

		ASSERT_EQ(Vector<3>::Back(), actual);
	}

	TEST(VectorTest, Negation)
	{
		constexpr Vector<2> vector{3.f, 7.f};
		constexpr Vector<2> actual = -vector;

		constexpr Vector<2> expected{-3.f, -7.f};
		ASSERT_EQ(expected, actual);
	}

	TEST(VectorTest, NegationZero)
	{
		constexpr Vector<2> vector{0.f, 5.f};
		constexpr Vector<2> actual = -vector;

		constexpr Vector<2> expected{0.f, -5.f};
		ASSERT_EQ(expected, actual);
	}

	TEST(VectorTest, Magnitude)
	{
		Vector<2> vector{-12.f, 5.f};
		float actual = vector.Length();

		float expected = 13.f;
		ASSERT_EQ(expected, actual);
	}

	TEST(VectorTest, Magnitude3)
	{
		Vector<3> vector{8.f, -3.f, 0.5f};
		float actual = vector.Length();

		float expected = 8.5586213843118448815764761472147f;
		ASSERT_FLOAT_EQ(expected, actual);
	}

	TEST(VectorTest, ScalarMultiplicationAssignment)
	{
		Vector<3> actual{4.f, -7.f, 0.f};
		actual *= 3;

		Vector<3> expected{12.f, -21.f, 0.f};
		ASSERT_EQ(expected, actual);
	}

	TEST(VectorTest, ScalarMultiplicationRight)
	{
		Vector<3> actual = Vector<3>{4.f, -7.f, 0.f} * 3;

		Vector<3> expected{12.f, -21.f, 0.f};
		ASSERT_EQ(expected, actual);
	}

	TEST(VectorTest, ScalarMultiplicationLeft)
	{
		Vector<3> actual = 3 * Vector<3>{4.f, -7.f, 0.f};

		Vector<3> expected{12.f, -21.f, 0.f};
		ASSERT_EQ(expected, actual);
	}

	TEST(VectorTest, ScalarDivisionAssignment)
	{
		Vector<2> actual{4.f, 5.f};
		actual /= 2;

		Vector<2> expected{2.f, 2.5f};
		ASSERT_EQ(expected, actual);
	}

	TEST(VectorTest, ScalarDivisionRight)
	{
		Vector<2> actual = Vector<2>{4.f, 5.f} / 2;

		Vector<2> expected{2.f, 2.5f};
		ASSERT_EQ(expected, actual);
	}

	TEST(VectorTest, Normalise)
	{
		Vector<2> actual = Vector<2>{12.f, 5.f};
		actual.Normalise();

		Vector<2> expected{12 / 13.f, 5 / 13.f};
		EXPECT_FLOAT_EQ(actual.X(), expected.X());
		EXPECT_FLOAT_EQ(actual.Y(), expected.Y());
	}

	TEST(VectorTest, NormalisedCopy)
	{
		constexpr Vector<2> original = Vector<2>{0.f, 743.632f};
		constexpr Vector<2> actual = original.Normalised();

		constexpr Vector<2> expected{0.f, 1.f};
		EXPECT_FLOAT_EQ(actual.X(), expected.X());
		EXPECT_FLOAT_EQ(actual.Y(), expected.Y());

		// Ensure original has not changed.
		EXPECT_FLOAT_EQ(original.X(), 0.f);
		EXPECT_FLOAT_EQ(original.Y(), 743.632f);
	}

	TEST(VectorTest, Normalised3)
	{
		constexpr Vector<3> actual = Vector<3>{8.f, -3.f, 0.5f}.Normalised();
		constexpr Vector<3> expected
		{
			0.93472998053917759798787452802344f,
			-0.35052374270219159924545294800879f,
			0.05842062378369859987424215800146f
		};
		EXPECT_FLOAT_EQ(actual.X(), expected.X());
		EXPECT_FLOAT_EQ(actual.Y(), expected.Y());
		EXPECT_FLOAT_EQ(actual.Z(), expected.Z());
	}

	TEST(VectorTest, Normalised3Negatives)
	{
		constexpr Vector<3> actual = Vector<3>{-12.f, 3.f, -4.f}.Normalised();
		constexpr Vector<3> expected
		{
			-12 / 13.f,
			3 / 13.f,
			-4 / 13.f
		};
		EXPECT_FLOAT_EQ(actual.X(), expected.X());
		EXPECT_FLOAT_EQ(actual.Y(), expected.Y());
		EXPECT_FLOAT_EQ(actual.Z(), expected.Z());
	}

	TEST(VectorTest, NormaliseAllOnes)
	{
		constexpr Vector<4> actual = Vector<4>{1.f, 1.f, 1.f, 1.f}.Normalised();
		constexpr Vector<4> expected
		{
			1 / 2.f,
			1 / 2.f,
			1 / 2.f,
			1 / 2.f
		};
		EXPECT_FLOAT_EQ(actual.X(), expected.X());
		EXPECT_FLOAT_EQ(actual.Y(), expected.Y());
		EXPECT_FLOAT_EQ(actual.Z(), expected.Z());
		EXPECT_FLOAT_EQ(actual.W(), expected.W());
	}

	TEST(VectorTest, AdditionAssignment)
	{
		Vector<3> actual{7.f, -2.f, -3.f};
		actual += Vector<3>{6.f, 6.f, -4.f};
		Vector<3> expected{13.f, 4.f, -7.f};
		ASSERT_EQ(actual, expected);
	}

	TEST(VectorTest, Addition)
	{
		constexpr Vector<3> actual = Vector<3>{2.f, 9.f, -1.f} + Vector<3>{-2.f, -9.f, 1.f};
		constexpr Vector<3> expected{0.f, 0.f, 0.f};
		ASSERT_EQ(actual, expected);
	}

	TEST(VectorTest, SubtractionAssignment)
	{
		Vector<3> actual = Vector<3>{3.f, 10.f, 7.f};
		actual -= Vector<3>{8.f, -7.f, 4.f};
		Vector<3> expected{-5.f, 17.f, 3.f};
		ASSERT_EQ(actual, expected);
	}

	TEST(VectorTest, Subtraction)
	{
		constexpr Vector<3> actual = Vector<3>{4.f, 5.f, -11.f} - Vector<3>{-4.f, -5.f, 11.f};
		constexpr Vector<3> expected{8.f, 10.f, -22.f};
		ASSERT_EQ(actual, expected);
	}

	TEST(VectorTest, DistanceStatic)
	{
		constexpr float actual = Vector<2>::Distance({10.f, 6.f}, {-14.f, 30.f});
		constexpr float expected = 33.9411255f;
		ASSERT_FLOAT_EQ(actual, expected);
	}

	TEST(VectorTest, DistanceMethod)
	{
		constexpr float actual = Vector<2>{0.f, 0.f}.Distance({-12.f, 5.f});
		constexpr float expected = 13.f;
		ASSERT_FLOAT_EQ(actual, expected);
	}

	TEST(VectorTest, Distance3Int)
	{
		constexpr double actual = Vector<3, int>::Distance({3, 10, 7}, {8, -7, 4});
		double expected = std::sqrt(323);
		ASSERT_DOUBLE_EQ(actual, expected);
	}

	TEST(VectorTest, Distance3)
	{
		constexpr float actual = Vector<3>::Distance({-2.f, -4.f, 9.f}, {6.f, -7.f, 9.5f});
		constexpr float expected = 8.558621384f;
		ASSERT_FLOAT_EQ(actual, expected);
	}

	TEST(VectorTest, Distance4)
	{
		constexpr float actual = Vector<4>::Distance({4.f, -4.f, -4.f, 4.f}, {-6.f, 6.f, 6.f, -6.f});
		constexpr float expected = 20;
		ASSERT_FLOAT_EQ(actual, expected);
	}

	TEST(VectorTest, DotProductStatic)
	{
		constexpr float actual = Vector<2>::DotProduct({2.f, 6.f}, {-3.f, 8.f});
		constexpr float expected = 42;
		ASSERT_FLOAT_EQ(actual, expected);
	}

	TEST(VectorTest, DotProductMember)
	{
		constexpr float actual = -7 * Vector<2>{1.f, 2.f}.DotProduct({11.f, -4.f});
		constexpr float expected = -21.f;
		ASSERT_FLOAT_EQ(actual, expected);
	}

	TEST(VectorTest, DotProduct3)
	{
		constexpr float actual = 10 + Vector<3>{-5.f, 1.f, 3.f}.DotProduct({4.f, -13.f, 9.f});
		constexpr float expected = 4;
		ASSERT_FLOAT_EQ(actual, expected);
	}

	TEST(VectorTest, DotProduct3Again)
	{
		constexpr float actual = 3 * Vector<3>{-2.f, 0.f, 4.f}.DotProduct(Vector<3>{8.f, -2.f, 3.f / 2.f} + Vector<3>{
			0.f, 9.f, 7.f
		});
		constexpr float expected = 54;
		ASSERT_FLOAT_EQ(actual, expected);
	}

	TEST(VectorTest, ParallelAndPerpendicular)
	{
		constexpr Vector<3> vector{4.f, 3.f, -1.f};
		constexpr Vector<3> unit{0.7071067812f, 0.7071067812f, 0.f};

		constexpr auto parallel = Vector<3>::Project(vector, unit);
		constexpr auto perpendicular = Vector<3>::ProjectPerpendicular(vector, unit);

		EXPECT_FLOAT_EQ(parallel.X(), 3.5f);
		EXPECT_FLOAT_EQ(parallel.Y(), 3.5f);
		EXPECT_FLOAT_EQ(parallel.Z(), 0.f);

		EXPECT_NEAR(perpendicular.X(), 0.5f, 0.000001);
		EXPECT_NEAR(perpendicular.Y(), -0.5f, 0.000001);
		EXPECT_NEAR(perpendicular.Z(), -1, 0.000001);
	}

	TEST(VectorTest, CrossProductStaticAB)
	{
		constexpr Vector<3> a{0.f, -1.f, 0.f};
		constexpr Vector<3> b{0.f, 0.f, 1.f};
		constexpr Vector<3> actual = Vector<3>::CrossProduct(a, b);

		constexpr Vector<3> expected{-1.f, 0.f, 0.f};
		ASSERT_EQ(actual, expected);
	}

	TEST(VectorTest, CrossProductStaticBA)
	{
		constexpr Vector<3> a{0.f, -1.f, 0.f};
		constexpr Vector<3> b{0.f, 0.f, 1.f};
		constexpr Vector<3> actual = Vector<3>::CrossProduct(b, a);

		constexpr Vector<3> expected{1.f, 0.f, 0.f};
		ASSERT_EQ(actual, expected);
	}

	TEST(VectorTest, CrossProductMember)
	{
		constexpr Vector<3> a{-2.f, 4.f, 1.f};
		constexpr Vector<3> b{1.f, -2.f, -1.f};
		constexpr Vector<3> actual = a.CrossProduct(b);

		constexpr Vector<3> expected{-2.f, -1.f, 0.f};
		ASSERT_EQ(actual, expected);
	}

	TEST(VectorTest, CrossProductParallel)
	{
		constexpr Vector<3> a{-2.f, 4.f, 0.f};
		constexpr Vector<3> b{2.f, -4.f, 0.f};
		constexpr Vector<3> actual = a.CrossProduct(b);

		constexpr Vector<3> expected = Vector<3>::Zero();
		ASSERT_EQ(actual, expected);
	}

	TEST(VectorTest, CrossProductZeroVectorA)
	{
		constexpr Vector<3> a{Vector<3>::Zero()};
		constexpr Vector<3> b{2.f, -4.f, 0.f};
		constexpr Vector<3> actual = a.CrossProduct(b);

		constexpr Vector<3> expected = Vector<3>::Zero();
		ASSERT_EQ(actual, expected);
	}

	TEST(VectorTest, CrossProductZeroVectorB)
	{
		constexpr Vector<3> a{-2.f, 4.f, 0.f};
		constexpr Vector<3> b{Vector<3>::Zero()};
		constexpr Vector<3> actual = a.CrossProduct(b);

		constexpr Vector<3> expected = Vector<3>::Zero();
		ASSERT_EQ(actual, expected);
	}

	TEST(VectorTest, IsPerpendicular)
	{
		constexpr Vector<3> up = Vector<3>::Up();
		constexpr Vector<3> right = Vector<3>::Right();
		Vector<3> forward = Vector<3>::Forward();
		Vector<3> down = Vector<3>::Down();
		Vector<3> left = Vector<3>::Left();
		Vector<3> back = Vector<3>::Back();

		static_assert(Vector<3>::IsPerpendicular(up, right) == true);
		EXPECT_TRUE(Vector<3>::IsPerpendicular(up, right));
		EXPECT_TRUE(Vector<3>::IsPerpendicular(up, forward));
		EXPECT_TRUE(Vector<3>::IsPerpendicular(forward, right));

		EXPECT_FALSE(Vector<3>::IsPerpendicular(up, up));
		EXPECT_FALSE(Vector<3>::IsPerpendicular(up, down));
		EXPECT_FALSE(Vector<3>::IsPerpendicular(left, right));
		EXPECT_FALSE(Vector<3>::IsPerpendicular(back, forward));
	}

	TEST(VectorTest, IsParallel)
	{
		constexpr Vector<3> up = Vector<3>::Up();
		constexpr Vector<3> right = Vector<3>::Right();
		Vector<3> forward = Vector<3>::Forward();
		Vector<3> down = Vector<3>::Down();
		Vector<3> left = Vector<3>::Left();
		Vector<3> back = Vector<3>::Back();

		static_assert(Vector<3>::IsParallel(up, up) == true);
		EXPECT_TRUE(Vector<3>::IsParallel(up, up));
		EXPECT_TRUE(Vector<3>::IsParallel(up, down));
		EXPECT_TRUE(Vector<3>::IsParallel(left, right));
		EXPECT_TRUE(Vector<3>::IsParallel(back, forward));

		EXPECT_FALSE(Vector<3>::IsParallel(up, right));
		EXPECT_FALSE(Vector<3>::IsParallel(up, forward));
		EXPECT_FALSE(Vector<3>::IsParallel(forward, right));
	}

	TEST(VectorTest, IsZeroVector)
	{
		constexpr Vector<3> zero{};
		static_assert(zero.IsZero() == true);

		constexpr Vector<3> nonZero{5, 1, 0};
		EXPECT_FALSE(nonZero.IsZero());
	}

	TEST(VectorTest, IsUnit)
	{
		constexpr Vector<3> unit{0, 1, 0};
		constexpr Vector<3> non_unit{0, 0, 0};

		static_assert(unit.IsUnit() == true);
		static_assert(non_unit.IsUnit() == false);
	}
}
