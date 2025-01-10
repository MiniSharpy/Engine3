#include "../../src/Maths/Quaternion.h"
#include "gtest/gtest.h"

namespace Engine3
{
	TEST(Quaternion_Float, Construction)
	{
		constexpr Quaternion actual = Quaternion<float>::Identity();

		EXPECT_FLOAT_EQ(0, actual.X);
		EXPECT_FLOAT_EQ(0, actual.Y);
		EXPECT_FLOAT_EQ(0, actual.Z);
		EXPECT_FLOAT_EQ(1, actual.W);
	}
}
