#pragma once
#include "rfl/Literal.hpp"
#include "rfl/internal/StringLiteral.hpp"

#include <optional>
#include <type_traits>

namespace rox::repr {

    template <typename>
    struct is_optional_impl : std::false_type {};

    template <typename U>
    struct is_optional_impl<std::optional<U>> : std::true_type {};

    template <typename>
    struct is_array_impl : std::false_type {};

    template <typename U, size_t N>
    struct is_array_impl<std::array<U, N>> : std::true_type {};

    template <typename>
    struct array_type;

    template <typename U, size_t N>
    struct array_type<std::array<U, N>> {
        using type = U;
    };

    template <typename T>
    constexpr bool is_optional() {
        return is_optional_impl<std::remove_cv_t<std::remove_reference_t<T>>>::value;
    }

    template <typename T>
    constexpr bool is_array() {
        return is_array_impl<std::remove_cv_t<std::remove_reference_t<T>>>::value;
    }

    template <typename T>
    struct no_opt {
        using type = T;
    };

    template <typename T>
    struct no_opt<std::optional<T>> {
        using type = T;
    };

    template <typename T>
    using no_opt_t = no_opt<T>::type;

    template <typename T>
    struct first_literal;

    template <rfl::internal::StringLiteral First, rfl::internal::StringLiteral... Rest>
    struct first_literal<rfl::Literal<First, Rest...>> {
        static constexpr auto value = First;
    };

    template <typename>
    struct is_single_value : std::false_type {};

    template <typename T>
    concept SingleValue = requires {
        { T::single_value } -> std::same_as<bool>;
    };

    template <typename T>
        requires(T::single_value == true)
    struct is_single_value<T> : std::true_type {};

    template <typename T>
    constexpr bool is_single_value_v = is_single_value<T>::value;

    template <typename>
    struct is_complex : std::false_type {};

    template <typename T>
        requires(T::complex == true)
    struct is_complex<T> : std::true_type {};

    template <typename T>
    constexpr bool is_complex_v = is_complex<T>::value;

} // namespace rox::repr