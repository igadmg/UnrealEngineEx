#pragma once

template <typename E, typename std::enable_if<std::is_enum<E>::value && !std::is_convertible<E, int>::value, int>::type = 0>
constexpr inline bool operator ! (E rhs)
{
	using T = std::underlying_type_t<E>;
	return static_cast<T>(rhs) == 0;
}

template <typename E, typename std::enable_if<std::is_enum<E>::value && !std::is_convertible<E, int>::value, int>::type = 0>
constexpr inline E operator & (typename std::underlying_type_t<E> lhs, E rhs)
{
	using T = std::underlying_type_t<E>;
	return static_cast<E>(lhs & (1 << static_cast<T>(rhs)));
}

template <typename E, typename std::enable_if<std::is_enum<E>::value && !std::is_convertible<E, int>::value, int>::type = 0>
constexpr inline E operator & (E lhs, E rhs)
{
	using T = std::underlying_type_t<E>;
	return static_cast<E>(static_cast<T>(lhs) & (1 << static_cast<T>(rhs)));
}

template <typename E, typename std::enable_if<std::is_enum<E>::value && !std::is_convertible<E, int>::value, int>::type = 0>
constexpr inline E operator | (typename std::underlying_type_t<E> lhs, E rhs)
{
	using T = std::underlying_type_t<E>;
	return static_cast<E>(lhs | (1 << static_cast<T>(rhs)));
}

template <typename E, typename std::enable_if<std::is_enum<E>::value && !std::is_convertible<E, int>::value, int>::type = 0>
constexpr inline E operator | (E lhs, E rhs)
{
	using T = std::underlying_type_t<E>;
	return static_cast<E>(static_cast<T>(lhs) | (1 << static_cast<T>(rhs)));
}

template <typename E, typename std::enable_if<std::is_enum<E>::value && !std::is_convertible<E, int>::value, int>::type = 0>
constexpr inline E operator ^ (typename std::underlying_type_t<E> lhs, E rhs)
{
	using T = std::underlying_type_t<E>;
	return static_cast<E>(lhs ^ (1 << static_cast<T>(rhs)));
}

template <typename E, typename std::enable_if<std::is_enum<E>::value && !std::is_convertible<E, int>::value, int>::type = 0>
constexpr inline E operator ^ (E lhs, E rhs)
{
	using T = std::underlying_type_t<E>;
	return static_cast<E>(static_cast<T>(lhs) ^ (1 << static_cast<T>(rhs)));
}

template <typename E, typename std::enable_if<std::is_enum<E>::value && !std::is_convertible<E, int>::value, int>::type = 0>
constexpr inline E operator + (E lhs, E rhs)
{
	using T = std::underlying_type_t<E>;
	return static_cast<E>(static_cast<T>(lhs) | static_cast<T>(rhs));
}

template <typename E, typename std::enable_if<std::is_enum<E>::value && !std::is_convertible<E, int>::value, int>::type = 0>
constexpr inline bool operator % (typename std::underlying_type_t<E> lhs, E rhs)
{
	using T = std::underlying_type_t<E>;
	return static_cast<T>(lhs & rhs) == (1 << static_cast<T>(rhs));
}

template <typename E, typename std::enable_if<std::is_enum<E>::value && !std::is_convertible<E, int>::value, int>::type = 0>
constexpr inline bool operator % (E lhs, E rhs)
{
	using T = std::underlying_type_t<E>;
	return static_cast<T>(lhs & rhs) == (1 << static_cast<T>(rhs));
}

template<typename T>
static FString EnumToString(const T value)
{
	UEnum* pEnum = StaticEnum<T>();
	return pEnum ? pEnum->GetNameStringByIndex(static_cast<uint8>(value)) : FString("Enum not found");
}
