#include "../../src/Utility/BitFlags.h"
#include <gtest/gtest.h>

// Test enum
enum class RenderPass : uint8_t
{
	// Shouldn't need none because all bitwise ops should be through BitFlags<RenderPass>.
	Geometry = 1 << 0,
	Lighting = 1 << 1,
	Particles = 1 << 2,
	PostProcess = 1 << 3,
};


namespace Engine3
{
	TEST(BitFlagsTestEnum, Constructor_Empty)
	{
		constexpr uint8_t actual = BitFlags<RenderPass>{}.ToUnderlyingBaseType();
		constexpr uint8_t expected = 0b0;

		EXPECT_EQ(actual, expected);
	}

	TEST(BitFlagsTestEnum, Constructor_SingleParameter)
	{
		constexpr uint8_t actual = BitFlags{RenderPass::Lighting}.ToUnderlyingBaseType();
		constexpr uint8_t expected = 0b10;

		EXPECT_EQ(actual, expected);
	}

	TEST(BitFlagsTestEnum, Constructor_MutlipleParameters)
	{
		constexpr uint8_t actual = BitFlags{RenderPass::Particles, RenderPass::PostProcess}.ToUnderlyingBaseType();
		constexpr uint8_t expected = 0b1100;

		EXPECT_EQ(actual, expected);
	}

	TEST(BitFlagsTestEnum, FromUnderlyingBaseType)
	{
		constexpr uint8_t flags = 0b1100;
		constexpr BitFlags actual = BitFlags<RenderPass>::FromUnderlyingBaseType(flags);
		constexpr BitFlags expected = BitFlags{RenderPass::Particles, RenderPass::PostProcess};

		EXPECT_EQ(actual, expected);
	}

	TEST(BitFlagsTestEnum, IsSet)
	{
		constexpr BitFlags flags{RenderPass::Particles, RenderPass::PostProcess};

		EXPECT_TRUE(flags.IsSet(RenderPass::Particles));
		EXPECT_FALSE(flags.IsSet(RenderPass::Geometry));
		EXPECT_FALSE(flags.IsSet(RenderPass::Lighting));
	}

	TEST(BitFlagsTestEnum, Set_Enum)
	{
		BitFlags actual{RenderPass::Particles, RenderPass::PostProcess};
		actual.Set(RenderPass::Lighting);

		BitFlags expected{RenderPass::Lighting, RenderPass::Particles, RenderPass::PostProcess};
		EXPECT_EQ(actual, expected);
	}

	TEST(BitFlagsTestEnum, Set_BitFlags)
	{
		BitFlags<RenderPass> actual;
		actual.Set({RenderPass::Particles, RenderPass::PostProcess});

		BitFlags expected{RenderPass::Particles, RenderPass::PostProcess};
		EXPECT_EQ(actual, expected);
	}

	TEST(BitFlagsTestEnum, Unset_Enum)
	{
		BitFlags actual{RenderPass::Particles, RenderPass::PostProcess};
		actual.Unset(RenderPass::Particles);

		BitFlags expected{RenderPass::PostProcess};
		EXPECT_EQ(actual, expected);
	}

	TEST(BitFlagsTestEnum, Unset_BitFlags)
	{
		BitFlags actual{RenderPass::Particles, RenderPass::PostProcess};
		actual.Unset({RenderPass::Particles, RenderPass::PostProcess});

		BitFlags<RenderPass> expected;
		EXPECT_EQ(actual, expected);
	}

	TEST(BitFlagsTestEnum, Clear)
	{
		BitFlags actual{RenderPass::Particles, RenderPass::PostProcess};
		actual.Clear();

		BitFlags<RenderPass> expected;
		EXPECT_EQ(actual, expected);
	}

	TEST(BitFlagsTestEnum, Count)
	{
		uint8_t actual = BitFlags{RenderPass::Particles, RenderPass::PostProcess}.Count();

		uint8_t expected = 2;
		EXPECT_EQ(actual, expected);
	}

	TEST(BitFlagsTestEnum, IsAllSet)
	{
		BitFlags flags0{RenderPass::Lighting};
		BitFlags flags1
		{
			RenderPass::Lighting,
			RenderPass::Particles,
			RenderPass::PostProcess
		};

		ASSERT_TRUE(flags1.IsAllSet(flags0));
		ASSERT_FALSE(flags0.IsAllSet(flags1));
	}

	TEST(BitFlagsTestEnum, IsAnySet)
	{
		BitFlags flags0{RenderPass::Lighting};
		BitFlags flags1
		{
			RenderPass::Lighting,
			RenderPass::Particles,
			RenderPass::PostProcess
		};
		BitFlags<RenderPass> flags2;

		EXPECT_TRUE(flags0.IsAnySet(flags1));
		EXPECT_TRUE(flags1.IsAnySet(flags0));
		EXPECT_FALSE(flags2.IsAnySet(flags0));
	}

	TEST(BitFlagsTestEnum, IsNoneSet)
	{
		BitFlags flags0{RenderPass::Lighting};
		BitFlags flags1
		{
			RenderPass::Particles,
			RenderPass::PostProcess
		};
		BitFlags flags2
		{
			RenderPass::Lighting,
			RenderPass::Geometry,
		};

		EXPECT_TRUE(flags0.IsNoneSet(flags1));
		EXPECT_TRUE(flags1.IsNoneSet(flags0));
		EXPECT_TRUE(flags2.IsNoneSet(flags1));
		EXPECT_FALSE(flags2.IsNoneSet(flags0));
	}

	TEST(BitFlagsTestEnum, Any)
	{
		BitFlags<RenderPass> flags0;
		BitFlags flags1
		{
			RenderPass::Particles,
			RenderPass::PostProcess
		};

		EXPECT_FALSE(flags0.Any());
		EXPECT_TRUE(flags1.Any());
	}

	TEST(BitFlagsTestEnum, None)
	{
		BitFlags<RenderPass> flags0;
		BitFlags flags1
		{
			RenderPass::Particles,
			RenderPass::PostProcess
		};

		EXPECT_TRUE(flags0.None());
		EXPECT_FALSE(flags1.None());
	}

	TEST(BitFlagsTestEnum, BitwiseNOT)
	{
		uint8_t actual = (~BitFlags{RenderPass::Particles, RenderPass::PostProcess}).ToUnderlyingBaseType();

		uint8_t expected = 0b11110011;
		EXPECT_EQ(actual, expected);
	}

	TEST(BitFlagsTestEnum, BitwiseAND_Enum)
	{
		BitFlags actual = BitFlags{RenderPass::Particles, RenderPass::PostProcess} & RenderPass::Particles;

		BitFlags expected = BitFlags{RenderPass::Particles};
		EXPECT_EQ(actual, expected);
	}

	TEST(BitFlagsTestEnum, BitwiseAND_BitFlags)
	{
		BitFlags actual = BitFlags{RenderPass::Particles, RenderPass::PostProcess} &
			BitFlags{RenderPass::Particles, RenderPass::Geometry};

		BitFlags expected = BitFlags{RenderPass::Particles};
		EXPECT_EQ(actual, expected);
	}

	TEST(BitFlagsTestEnum, BitwiseAND_Assignment_Enum)
	{
		BitFlags actual = BitFlags{RenderPass::Particles, RenderPass::PostProcess};
		actual &= RenderPass::Particles;

		BitFlags expected = BitFlags{RenderPass::Particles};
		EXPECT_EQ(actual, expected);
	}

	TEST(BitFlagsTestEnum, BitwiseAND_Assignment_BitFlags)
	{
		BitFlags actual = BitFlags{RenderPass::Particles, RenderPass::PostProcess};
		actual &= BitFlags{RenderPass::Particles, RenderPass::Geometry};

		BitFlags expected = BitFlags{RenderPass::Particles};
		EXPECT_EQ(actual, expected);
	}

	TEST(BitFlagsTestEnum, BitwiseOR_Enum)
	{
		BitFlags actual = BitFlags{RenderPass::Particles, RenderPass::PostProcess} | RenderPass::Geometry;
		BitFlags expected = BitFlags{RenderPass::Particles, RenderPass::PostProcess, RenderPass::Geometry};

		EXPECT_EQ(actual, expected);
	}

	TEST(BitFlagsTestEnum, BitwiseOR_BitFlags)
	{
		BitFlags actual = BitFlags{RenderPass::Particles, RenderPass::PostProcess} |
			BitFlags{RenderPass::Particles, RenderPass::Geometry};
		BitFlags expected = BitFlags{RenderPass::Particles, RenderPass::PostProcess, RenderPass::Geometry};

		EXPECT_EQ(actual, expected);
	}

	TEST(BitFlagsTestEnum, BitwiseOR_Assignment_Enum)
	{
		BitFlags actual = BitFlags{RenderPass::Particles, RenderPass::PostProcess};
		actual |= RenderPass::Geometry;

		BitFlags expected = BitFlags{RenderPass::Particles, RenderPass::PostProcess, RenderPass::Geometry};
		EXPECT_EQ(actual, expected);
	}

	TEST(BitFlagsTestEnum, BitwiseOR_Assignment_BitFlags)
	{
		BitFlags actual = BitFlags{RenderPass::Particles, RenderPass::PostProcess};
		actual |= BitFlags{RenderPass::Particles, RenderPass::Geometry};

		BitFlags expected = BitFlags{RenderPass::Particles, RenderPass::PostProcess, RenderPass::Geometry};
		EXPECT_EQ(actual, expected);
	}

	TEST(BitFlagsTestEnum, BitwiseXOR_Enum)
	{
		BitFlags actual = BitFlags{RenderPass::Particles, RenderPass::PostProcess} ^ RenderPass::Particles;
		BitFlags expected = BitFlags{RenderPass::PostProcess};

		EXPECT_EQ(actual, expected);
	}

	TEST(BitFlagsTestEnum, BitwiseXOR_BitFlags)
	{
		BitFlags actual = BitFlags{RenderPass::Particles, RenderPass::PostProcess} ^
			BitFlags{RenderPass::Particles, RenderPass::Geometry};
		BitFlags expected = BitFlags{RenderPass::PostProcess, RenderPass::Geometry};

		EXPECT_EQ(actual, expected);
	}

	TEST(BitFlagsTestEnum, BitwiseXOR_Assignment_Enum)
	{
		BitFlags actual = BitFlags{RenderPass::Particles, RenderPass::PostProcess};
		actual ^= RenderPass::Particles;

		BitFlags expected = BitFlags{RenderPass::PostProcess};
		EXPECT_EQ(actual, expected);
	}

	TEST(BitFlagsTestEnum, BitwiseXOR_Assignment_BitFlags)
	{
		BitFlags actual = BitFlags{RenderPass::Particles, RenderPass::PostProcess};
		actual ^= BitFlags{RenderPass::Particles, RenderPass::Geometry};

		BitFlags expected = BitFlags{RenderPass::PostProcess, RenderPass::Geometry};
		EXPECT_EQ(actual, expected);
	}
}
