#pragma once
#include <compare>
#include <cstddef>
#include <cstdint>
#include <string_view>

#if defined(_MSC_VER)
#define CURRENT_FUNCTION __FUNCSIG__
#elif defined(__clang__) || defined(__GNUC__)
#define CURRENT_FUNCTION __PRETTY_FUNCTION__
#else
#define CURRENT_FUNCTION __func__
#endif

namespace rox {

    namespace impl {
        template <typename>
        consteval std::string_view dummy_function() {
            return std::string_view(CURRENT_FUNCTION);
        }

        constexpr uint64_t fnv1a(const std::string_view str) {
            uint64_t hash = 14695981039346656037ull; // FNV offset basis
            for (const char c : str) {
                hash ^= static_cast<uint64_t>(c);
                hash *= 1099511628211ull; // FNV prime
            }
            return hash;
        }
    } // namespace impl

    struct TypeHash {
        size_t hash{};

        constexpr bool operator==(const TypeHash&) const noexcept = default;
        constexpr bool operator!=(const TypeHash&) const noexcept = default;
        constexpr auto operator<=>(const TypeHash&) const noexcept = default;
    };

    template <typename T>
    constexpr TypeHash type_hash() {
        constexpr auto hash_target = impl::dummy_function<T>();
        constexpr auto hash = impl::fnv1a(hash_target);
        return TypeHash{ hash };
    }
} // namespace rox