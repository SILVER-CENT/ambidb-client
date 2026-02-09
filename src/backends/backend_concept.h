#pragma once

#include <concepts>
#include <type_traits>

namespace ambidb {

/**
 * @brief Concept defining the requirements for a backend implementation.
 *
 * This concept replaces the virtual IBackend interface with compile-time
 * polymorphism using C++23 concepts. Backends must provide:
 * - InitializeBackend() -> bool
 * - InitializeImGui() -> bool
 * - Run() -> void
 * - ShutdownImGui() -> void
 * - ShutdownBackend() -> void
 * - GetName() const -> const char*
 *
 * Note: The concept only checks for the existence of methods, not their access level.
 * This allows CRTP base class to call these methods even if they're private/public in derived.
 */
template<typename T>
concept Backend = requires(T& backend, const T& const_backend) {
    // Lifecycle methods
    { backend.InitializeBackend() } -> std::same_as<bool>;
    { backend.InitializeImGui() } -> std::same_as<bool>;
    { backend.Run() } -> std::same_as<void>;
    { backend.ShutdownImGui() } -> std::same_as<void>;
    { backend.ShutdownBackend() } -> std::same_as<void>;

    // Identification
    { const_backend.GetName() } -> std::convertible_to<const char*>;
};

/**
 * @brief Type trait to check if a type satisfies the Backend concept at compile time.
 */
template<typename T>
inline constexpr bool is_backend_v = Backend<T>;

} // namespace ambidb
