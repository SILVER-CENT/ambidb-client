#pragma once
#include <cstdint>
#include <ranges>
#include <filesystem>
#include <expected>
#include <span>

/**
 * MACRO USAGE GUIDE
 * =================
 *
 * These macros provide clean, readable ways to control object copying and moving
 * behavior. Use them in the public section of your class to make your intent clear.
 */

/**
 * MAKE_NONCOPYABLE(c)
 *
 * WHEN TO USE:
 * - Resource management classes (file handles, network connections, locks)
 * - Classes with unique identity that shouldn't be duplicated
 * - Classes managing non-copyable resources (mutexes, unique_ptr members)
 * - Base classes that shouldn't be copied
 *
 * EXAMPLES:
 * - Database connections
 * - File handlers
 * - Network sockets
 * - Thread-safe containers
 * - Singleton-like classes
 *
 * PLACEMENT: Use in the public section of your class
 */
#define MAKE_NONCOPYABLE(c) \
	c (c const&) = delete;  \
	c& operator= (c const&) = delete

/**
 * MAKE_NONMOVABLE(c)
 *
 * WHEN TO USE:
 * - Classes with resources tied to specific memory locations
 * - Classes with internal pointers that would become invalid if moved
 * - Classes that are part of a larger object graph
 * - Classes with static or global state
 *
 * EXAMPLES:
 * - Classes with member pointers to themselves
 * - Classes embedded in unions
 * - Classes with static member variables
 * - Classes that register themselves in global registries
 *
 * PLACEMENT: Use in the public section of your class
 */
#define MAKE_NONMOVABLE(c) \
	c (c&&) = delete;      \
	c& operator= (c&&) = delete

/**
 * MAKE_DEFAULT_MOVABLE(c)
 *
 * WHEN TO USE:
 * - Resource management classes that can transfer ownership
 * - Classes that are expensive to copy but cheap to move
 * - Classes that can be returned from functions efficiently
 * - Classes that can be stored in move-only containers
 *
 * EXAMPLES:
 * - Smart pointers
 * - String classes
 * - Vector-like containers
 * - RAII resource managers
 * - Classes with moveable member variables
 *
 * PLACEMENT: Use in the public section of your class
 */
#define MAKE_DEFAULT_MOVABLE(c) \
	c (c&&) = default;          \
	c& operator= (c&&) = default

/**
 * MAKE_DEFAULT_COPYABLE(c)
 *
 * WHEN TO USE:
 * - Value types that should be freely copyable
 * - Small, simple classes that are cheap to copy
 * - Classes that represent data rather than resources
 * - Classes that can be safely duplicated
 *
 * EXAMPLES:
 * - Point, Rectangle, Color classes
 * - Small data structures
 * - Configuration objects
 * - Mathematical types
 * - Classes with only primitive members
 *
 * PLACEMENT: Use in the public section of your class
 */
#define MAKE_DEFAULT_COPYABLE(c) \
	c (c const&) = default;      \
	c& operator= (c const&) = default

/**
 * COMMON COMBINATIONS:
 *
 * 1. NONCOPYABLE + NONMOVABLE = Completely locked down class
 *    Use for: Singletons, classes with static state
 *
 * 2. NONCOPYABLE + DEFAULT_MOVABLE = Move-only resource manager
 *    Use for: Smart pointers, unique resource handlers
 *
 * 3. DEFAULT_COPYABLE + DEFAULT_MOVABLE = Standard value type
 *    Use for: Data classes, mathematical types
 *
 * 4. DEFAULT_COPYABLE + NONMOVABLE = Copyable but not moveable
 *    Use for: Classes with self-referential pointers
 *
 * USAGE PATTERN:
 * class MyClass {
 * public:
 *     MAKE_NONCOPYABLE(MyClass);  // Prevents copying
 *     MAKE_DEFAULT_MOVABLE(MyClass);  // Allows moving
 *
 *     // ... rest of class implementation
 * };
 */
// Signed integers
using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

// Unsigned integers
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

// Floating point
using f32 = float;
using f64 = double;

// Pointer-sized integers
using isize = intptr_t;
using usize = uintptr_t;

// Character types
using char8 = char8_t;	// UTF-8 character
using char16 = char16_t;  // UTF-16 character
using char32 = char32_t;  // UTF-32 character


// User-defined literals for signed integers
constexpr i8
operator""_i8 (unsigned long long v) noexcept {
	return static_cast<i8> (v);
}
constexpr i16
operator""_i16 (unsigned long long v) noexcept {
	return static_cast<i16> (v);
}
constexpr i32
operator""_i32 (unsigned long long v) noexcept {
	return static_cast<i32> (v);
}
constexpr i64
operator""_i64 (unsigned long long v) noexcept {
	return static_cast<i64> (v);
}

// User-defined literals for unsigned integers
constexpr u8
operator""_u8 (unsigned long long v) noexcept {
	return static_cast<u8> (v);
}
constexpr u16
operator""_u16 (unsigned long long v) noexcept {
	return static_cast<u16> (v);
}
constexpr u32
operator""_u32 (unsigned long long v) noexcept {
	return static_cast<u32> (v);
}
constexpr u64
operator""_u64 (unsigned long long v) noexcept {
	return static_cast<u64> (v);
}

// User-defined literals for floating point
constexpr f32
operator""_f32 (long double v) noexcept {
	return static_cast<f32> (v);
}
constexpr f64
operator""_f64 (long double v) noexcept {
	return static_cast<f64> (v);
}

// User-defined literals for pointer-sized integers
constexpr isize
operator""_isize (unsigned long long v) noexcept {
	return static_cast<isize> (v);
}
constexpr usize
operator""_usize (unsigned long long v) noexcept {
	return static_cast<usize> (v);
}

template <typename T, typename E>
using result = std::expected<T, E>;
template <typename>
struct is_result_impl : std::false_type {};
template <typename U, typename Err>
struct is_result_impl<std::expected<U, Err>> : std::true_type {};

template <typename T, typename E, typename F>
[[nodiscard]] constexpr auto
operator| (result<T, E>&& r, F&& fn) {
	using ret_t = std::remove_cvref_t<std::invoke_result_t<F, T>>;
	if constexpr (!is_result_impl<ret_t>::value) {
		static_assert (false, "F must return a result");
	}
	return std::move (r).and_then (std::forward<F> (fn));
}
template <typename T, typename E, typename F>
[[nodiscard]] constexpr auto
operator>> (result<T, E>&& r, F&& fn) {
	using ret_t = std::remove_cvref_t<std::invoke_result_t<F, T>>;
	if constexpr (is_result_impl<ret_t>::value)
		static_assert (false, "F must not return a result");
	return std::move (r).transform (std::forward<F> (fn));
}

template <typename T>
using span = std::span<T>;

/**
 * NAMESPACE ALIASES
 * =================
 */

namespace rg = std::ranges;
namespace rv = std::ranges::views;
namespace fs = std::filesystem;

/**
 * STANDARD LITERALS
 * =================
 */

using namespace std::literals;

/**
 * UTILITY MACROS
 * ==============
 */

#define MOVE(x) std::move (x)
#define FWRD(x) static_cast<decltype (x)&&> (x)
#define STR(x) #x
#define CONCAT_IMPL(a, b) a##b
#define CONCAT(a, b) CONCAT_IMPL (a, b)

#define NODISCARD [[nodiscard]]
#define MAYBE_UNUSED [[maybe_unused]]

/**
 * ERROR HANDLING MACROS
 * =====================
 */

#define UNREACHABLE() std::unreachable ()

#define PANIC(msg, ...)                                                                                   \
	do {                                                                                                  \
		std::println (std::cerr, "PANIC at {}:{}: " msg, __FILE__, __LINE__ __VA_OPT__ (, ) __VA_ARGS__); \
		std::abort ();                                                                                    \
	}                                                                                                     \
	while (0)

#define TODO(msg)                                                               \
	do {                                                                        \
		std::println (std::cerr, "TODO: {} at {}:{}", msg, __FILE__, __LINE__); \
	}                                                                           \
	while (0)
