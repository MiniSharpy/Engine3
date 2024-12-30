#pragma once
#include <bit>
#include <type_traits>
#include <utility>

// https://voithos.io/articles/type-safe-enum-class-bit-flags/
// iostream, std::print
template <class Enum>
	requires (std::is_enum_v<Enum>)
class BitFlags
{
	using UnderlyingBaseType = std::underlying_type_t<Enum>;

	// By storing the flags in the underlying type, bitwise operators can be used without any boilerplate.
	UnderlyingBaseType Flags{};

	/* Constructors */
	constexpr explicit BitFlags(UnderlyingBaseType flags) : Flags{flags} {}

public:
	constexpr BitFlags() = default;

	constexpr explicit BitFlags(Enum flag) : Flags{std::to_underlying(flag)} {}

	constexpr BitFlags(std::initializer_list<Enum> flags)
	{
		for (Enum flag : flags) { Flags |= std::to_underlying(flag); }
	}

	/* Utility */
	/// @return \p true if the corresponding mask
	constexpr bool IsSet(Enum mask) const { return Flags & std::to_underlying(mask); }

	/// Set the corresponding flag to \p true.
	constexpr void Set(Enum mask) { Flags |= std::to_underlying(mask); }

	/// Set the corresponding flags to \p true.
	constexpr void Set(BitFlags mask) { Flags |= mask.Flags; }

	/// Set the corresponding flag to \p false.
	constexpr void Unset(Enum mask) { Flags &= ~std::to_underlying(mask); }

	/// Set the corresponding flags to \p true.
	constexpr void Unset(BitFlags mask) { Flags &= ~mask.Flags; }

	/// Sets all bits to \p false.
	constexpr void Clear() { Flags &= static_cast<UnderlyingBaseType>(0); }

	/// @return Returns the number of true bits in \p this.
	constexpr UnderlyingBaseType Count() const { return std::popcount(Flags); }

	/// @return \p true if all the bits in \p mask are in \p this, otherwise \p false;
	constexpr bool IsAllSet(BitFlags mask) const { return (Flags & mask.Flags) == mask.Flags; }

	/// @return \p true if any of the bits in \p mask are in \p this, otherwise \p false;
	constexpr bool IsAnySet(BitFlags mask) const { return Flags & mask.Flags; }

	/// @return \p true if none of the bits in \p mask are in \p this, otherwise \p false;
	constexpr bool IsNoneSet(BitFlags mask) const { return !static_cast<bool>(Flags & mask.Flags); }

	// Don't know if this method makes sense from a practical standpoint. There would need to be some sort of check
	// to determine if all defined enum flags are set.
	/// @return \p true if all bits are set to \p true, otherwise \p false.
	//constexpr bool All() const { return (Flags ^ ~static_cast<UnderlyingBaseType>(0)) == 0; }

	/// @return \p true if any of the bits are set to \p true, otherwise \p false.
	constexpr bool Any() const { return Flags != 0; }

	/// @return \p true if none of the bits are set to \p true, otherwise \p false.
	constexpr bool None() const { return Flags == 0; }

	static constexpr BitFlags FromUnderlyingBaseType(UnderlyingBaseType flags) { return BitFlags(flags); }

	constexpr UnderlyingBaseType ToUnderlyingBaseType() const { return Flags; }

	/* Bitwise NOT */
	/// Negated BitFlags can result in true bits where there isn't an enum representation.
	friend constexpr BitFlags operator~(BitFlags flags) { return BitFlags(~flags.Flags); }

	/* Bitwise AND */
	friend constexpr BitFlags operator&(BitFlags lhs, Enum rhs)
	{
		return BitFlags(lhs.Flags & std::to_underlying(rhs));
	}

	friend constexpr BitFlags operator&(BitFlags lhs, BitFlags rhs) { return BitFlags(lhs.Flags & rhs.Flags); }

	friend constexpr BitFlags& operator&=(BitFlags& lhs, Enum rhs)
	{
		lhs.Flags &= std::to_underlying(rhs);
		return lhs;
	}

	friend constexpr BitFlags& operator&=(BitFlags& lhs, BitFlags rhs)
	{
		lhs.Flags &= rhs.Flags;
		return lhs;
	}

	/* Bitwise OR */
	friend constexpr BitFlags operator|(BitFlags lhs, Enum rhs)
	{
		return BitFlags(lhs.Flags | std::to_underlying(rhs));
	}

	friend constexpr BitFlags operator|(BitFlags lhs, BitFlags rhs) { return BitFlags(lhs.Flags | rhs.Flags); }

	friend constexpr BitFlags& operator|=(BitFlags& lhs, BitFlags rhs)
	{
		lhs.Flags |= rhs.Flags;
		return lhs;
	}

	friend constexpr BitFlags& operator|=(BitFlags& lhs, Enum rhs)
	{
		lhs.Flags |= std::to_underlying(rhs);
		return lhs;
	}

	/* Bitwise XOR  */
	friend constexpr BitFlags operator^(BitFlags lhs, Enum rhs)
	{
		return BitFlags(lhs.Flags ^ std::to_underlying(rhs));
	}

	friend constexpr BitFlags operator^(BitFlags lhs, BitFlags rhs) { return BitFlags(lhs.Flags ^ rhs.Flags); }

	friend constexpr BitFlags& operator^=(BitFlags& lhs, BitFlags rhs)
	{
		lhs.Flags ^= rhs.Flags;
		return lhs;
	}

	friend constexpr BitFlags& operator^=(BitFlags& lhs, Enum rhs)
	{
		lhs.Flags ^= std::to_underlying(rhs);
		return lhs;
	}

	/* Comparator */
	friend bool operator==(const BitFlags& lhs, const BitFlags& rhs) { return lhs.Flags == rhs.Flags; }
	friend bool operator!=(const BitFlags& lhs, const BitFlags& rhs) { return !(lhs == rhs); }
};
