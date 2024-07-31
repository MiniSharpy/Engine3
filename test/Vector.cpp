#include "../src/Maths/Vector.h"
#include <gtest/gtest.h>

namespace Engine3
{
	TEST(VectorTest, ZeroVector)
	{
		Vector<2> actual{0, 0};

		ASSERT_EQ(Vector<2>::Zero(), actual);
	}

	TEST(VectorTest, Right2)
	{
		Vector<2> actual{1, 0};

		ASSERT_EQ(Vector<2>::Right(), actual);
	}

	TEST(VectorTest, Left2)
	{
		Vector<2> actual{-1, 0};

		ASSERT_EQ(Vector<2>::Left(), actual);
	}

	TEST(VectorTest, Up2)
	{
		Vector<2> actual{0, 1};

		ASSERT_EQ(Vector<2>::Up(), actual);
	}

	TEST(VectorTest, Down2)
	{
		Vector<2> actual{0, -1};

		ASSERT_EQ(Vector<2>::Down(), actual);
	}

	TEST(VectorTest, Right3)
	{
		Vector<3> actual{1, 0, 0};

		ASSERT_EQ(Vector<3>::Right(), actual);
	}

	TEST(VectorTest, Left3)
	{
		Vector<3> actual{-1, 0, 0};

		ASSERT_EQ(Vector<3>::Left(), actual);
	}

	TEST(VectorTest, Up3)
	{
		Vector<3> actual{0, 1, 0};

		ASSERT_EQ(Vector<3>::Up(), actual);
	}

	TEST(VectorTest, Down3)
	{
		Vector<3> actual{0, -1, 0};

		ASSERT_EQ(Vector<3>::Down(), actual);
	}

	TEST(VectorTest, Forward)
	{
		Vector<3> actual{0, 0, 1};

		ASSERT_EQ(Vector<3>::Forward(), actual);
	}

	TEST(VectorTest, Back)
	{
		Vector<3> actual{0, 0, -1};

		ASSERT_EQ(Vector<3>::Back(), actual);
	}

	TEST(VectorTest, Negation)
	{
		Vector<2> vector{3, 7};
		Vector<2> actual = -vector;

		Vector<2> expected{-3, -7};
		ASSERT_EQ(expected, actual);
	}

	TEST(VectorTest, NegationZero)
	{
		Vector<2> vector{0, 5};
		Vector<2> actual = -vector;

		Vector<2> expected{0, -5};
		ASSERT_EQ(expected, actual);
	}

	TEST(VectorTest, Magnitude)
	{
		Vector<2> vector{-12, 5};
		float actual = vector.Length();

		float expected = 13;
		ASSERT_EQ(expected, actual);
	}

	TEST(VectorTest, Magnitude3)
	{
		Vector<3> vector{8, -3, 0.5};
		float actual = vector.Length();

		float expected = 8.5586213843118448815764761472147f;
		ASSERT_FLOAT_EQ(expected, actual);
	}

	TEST(VectorTest, ScalarMultiplicationAssignment)
	{
		Vector<3> actual{4, -7, 0};
		actual *= 3;

		Vector<3> expected{12, -21, 0};
		ASSERT_EQ(expected, actual);
	}

	TEST(VectorTest, ScalarMultiplicationRight)
	{
		Vector<3> actual = Vector<3>{4, -7, 0} * 3;

		Vector<3> expected{12, -21, 0};
		ASSERT_EQ(expected, actual);
	}

	TEST(VectorTest, ScalarMultiplicationLeft)
	{
		Vector<3> actual = 3 * Vector<3>{4, -7, 0};

		Vector<3> expected{12, -21, 0};
		ASSERT_EQ(expected, actual);
	}

	TEST(VectorTest, ScalarDivisionAssignment)
	{
		Vector<2> actual{4, 5};
		actual /= 2;

		Vector<2> expected{2, 2.5};
		ASSERT_EQ(expected, actual);
	}

	TEST(VectorTest, ScalarDivisionRight)
	{
		Vector<2> actual = Vector<2>{4, 5} / 2;

		Vector<2> expected{2, 2.5};
		ASSERT_EQ(expected, actual);
	}

	TEST(VectorTest, Normalise)
	{
		Vector<2> actual = Vector<2>{12, 5};
		actual.Normalise();

		Vector<2> expected{12 / 13.f, 5 / 13.f};
		ASSERT_FLOAT_EQ(actual.X(), expected.X());
		ASSERT_FLOAT_EQ(actual.Y(), expected.Y());
	}

	TEST(VectorTest, NormalisedCopy)
	{
		Vector<2> original = Vector<2>{0, 743.632};
		Vector<2> actual = original.Normalised();

		Vector<2> expected{0, 1};
		ASSERT_FLOAT_EQ(actual.X(), expected.X());
		ASSERT_FLOAT_EQ(actual.Y(), expected.Y());

		// Ensure original has not changed.
		ASSERT_FLOAT_EQ(original.X(), 0);
		ASSERT_FLOAT_EQ(original.Y(), 743.632f);
	}

	TEST(VectorTest, Normalise3)
	{
		Vector<3> actual = Vector<3>{8, -3, 0.5}.Normalised();

		Vector<3> expected
		{
			0.93472998053917759798787452802344,
			-0.35052374270219159924545294800879,
			0.05842062378369859987424215800146
		};
		ASSERT_FLOAT_EQ(actual.X(), expected.X());
		ASSERT_FLOAT_EQ(actual.Y(), expected.Y());
		ASSERT_FLOAT_EQ(actual.Z(), expected.Z());
	}

	TEST(VectorTest, Normalise3Negatives)
	{
		Vector<3> actual = Vector<3>{-12, 3, -4}.Normalised();

		Vector<3> expected
		{
			-12 / 13.f,
			3 / 13.f,
			-4 / 13.f
		};
		ASSERT_FLOAT_EQ(actual.X(), expected.X());
		ASSERT_FLOAT_EQ(actual.Y(), expected.Y());
		ASSERT_FLOAT_EQ(actual.Z(), expected.Z());
	}

	TEST(VectorTest, NormaliseAllOnes)
	{
		Vector<4> actual = Vector<4>{1, 1, 1, 1}.Normalised();

		Vector<4> expected
		{
			1 / 2.f,
			1 / 2.f,
			1 / 2.f,
			1 / 2.f
		};
		ASSERT_FLOAT_EQ(actual.X(), expected.X());
		ASSERT_FLOAT_EQ(actual.Y(), expected.Y());
		ASSERT_FLOAT_EQ(actual.Z(), expected.Z());
		ASSERT_FLOAT_EQ(actual.W(), expected.W());
	}

	TEST(VectorTest, AdditionAssignment)
	{
		Vector<3> actual{7, -2, -3};
		actual += Vector<3>{6, 6, -4};
		Vector<3> expected{13, 4, -7};
		ASSERT_EQ(actual, expected);
	}

	TEST(VectorTest, Addition)
	{
		Vector<3> actual = Vector<3>{2, 9, -1} + Vector<3>{-2, -9, 1};
		Vector<3> expected{0, 0, 0};
		ASSERT_EQ(actual, expected);
	}

	TEST(VectorTest, SubtractionAssignment)
	{
		Vector<3> actual = Vector<3>{3, 10, 7};
		actual -= Vector<3>{8, -7, 4};
		Vector<3> expected{-5, 17, 3};
		ASSERT_EQ(actual, expected);
	}

	TEST(VectorTest, Subtraction)
	{
		Vector<3> actual = Vector<3>{4, 5, -11} - Vector<3>{-4, -5, 11};
		Vector<3> expected{8, 10, -22};
		ASSERT_EQ(actual, expected);
	}

	TEST(VectorTest, DistanceStatic)
	{
		float actual = Vector<2>::Distance({10, 6}, {-14, 30});
		float expected = 33.9411255f;
		ASSERT_FLOAT_EQ(actual, expected);
	}

	TEST(VectorTest, DistanceMethod)
	{
		float actual = Vector<2>{0, 0}.Distance({-12, 5});
		float expected = 13.f;
		ASSERT_FLOAT_EQ(actual, expected);
	}

	TEST(VectorTest, Distance3Int)
	{
		double actual = Vector<3, int>::Distance({3, 10, 7}, {8, -7, 4});
		double expected = std::sqrt(323);
		ASSERT_DOUBLE_EQ(actual, expected);
	}

	TEST(VectorTest, Distance3)
	{
		float actual = Vector<3>::Distance({-2, -4, 9}, {6, -7, 9.5f});
		float expected = 8.558621384f;
		ASSERT_FLOAT_EQ(actual, expected);
	}

	TEST(VectorTest, Distance4)
	{
		float actual = Vector<4>::Distance({4, -4, -4, 4}, {-6, 6, 6, -6});
		float expected = 20;
		ASSERT_FLOAT_EQ(actual, expected);
	}

	TEST(VectorTest, DotProductStatic)
	{
		float actual = Vector<2>::DotProduct({2, 6}, {-3, 8});
		float expected = 42;
		ASSERT_FLOAT_EQ(actual, expected);
	}

	TEST(VectorTest, DotProductMember)
	{
		float actual = -7 * Vector<2>{1, 2}.DotProduct({11, -4});
		float expected = -21;
		ASSERT_FLOAT_EQ(actual, expected);
	}

	TEST(VectorTest, DotProduct3)
	{
		float actual = 10 + Vector<3>{-5, 1, 3}.DotProduct({4, -13, 9});
		float expected = 4;
		ASSERT_FLOAT_EQ(actual, expected);
	}

	TEST(VectorTest, DotProduct3Again)
	{
		float actual = 3 * Vector<3>{-2, 0, 4}.DotProduct(Vector<3>{8, -2, 3 / 2.f} + Vector<3>{0, 9, 7});
		float expected = 54;
		ASSERT_FLOAT_EQ(actual, expected);
	}

	TEST(VectorTest, ParallelAndPerpendicular)
	{
		Vector<3> vector{4, 3, -1};
		Vector<3> unit{std::sqrtf(2) / 2, std::sqrtf(2) / 2, 0};

		auto parallel = Vector<3>::Project(unit, vector);
		auto perpendicular = Vector<3>::ProjectPerpendicular(unit, vector);

		ASSERT_FLOAT_EQ(parallel.X(), 3.5f);
		ASSERT_FLOAT_EQ(parallel.Y(), 3.5f);
		ASSERT_FLOAT_EQ(parallel.Z(), 0);

		ASSERT_NEAR(perpendicular.X(), 0.5f, 0.000001);
		ASSERT_NEAR(perpendicular.Y(), -0.5f, 0.000001);
		ASSERT_NEAR(perpendicular.Z(), -1, 0.000001);
	}
}
