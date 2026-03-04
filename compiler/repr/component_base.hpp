#pragma once
#include "ir/component.hpp"
#include "util.hpp"

#include <ranges>
#include <stdexcept>

namespace rox::repr {

    struct FieldValidator {};

    template <typename T, T min, T max, bool inclusive = true>
        requires(std::is_integral_v<T> || std::is_floating_point_v<T>)
    struct Range : FieldValidator {
        T value;
        using GetType = double;
        using CastType = T;

        Range(T value) : value(value) {}
        Range() = default;

        static bool validate(T value) {
            if constexpr (inclusive) {
                return min <= value && value <= max;
            } else {
                return min < value && value < max;
            }
        }
    };

    template <typename T, typename U>
    void validate(const U& value) {
        if constexpr (std::is_base_of_v<FieldValidator, T>) {
            const auto result = T::validate(value);

            if (!result) {
                throw std::runtime_error("Failed to validate");
            }
        }
    }

    template <typename M>
    concept map_like = requires(M m, typename M::key_type key, typename M::mapped_type value) {
        typename M::key_type;
        typename M::mapped_type;
        typename M::value_type;

        { m.find(key) } -> std::same_as<typename M::iterator>;
        { m.insert(std::pair<typename M::key_type, typename M::mapped_type>{}) };
        { m[key] } -> std::same_as<typename M::mapped_type&>;

        { m.begin() } -> std::same_as<typename M::iterator>;
        { m.end() } -> std::same_as<typename M::iterator>;
        { m.reserve(static_cast<size_t>(0)) } -> std::same_as<void>;
    };

    static_assert(map_like<std::unordered_map<std::string, std::string>>);

    template <typename T>
    struct get_type {
        using type = T;
    };

    template <typename T>
        requires(std::is_base_of_v<FieldValidator, T>)
    struct get_type<T> {
        using type = T::GetType;
    };

    template <typename T>
        requires(std::is_integral_v<T> || std::is_floating_point_v<T>)
    struct get_type<T> {
        using type = double;
    };

    template <typename T, size_t N>
    struct get_type<std::array<T, N>> {
        using type = ir::Args;
    };

    template <map_like T>
    struct get_type<T> {
        using type = ir::Component;
    };

    template <typename T>
    using get_type_t = get_type<T>::type;

    static_assert(std::same_as<get_type_t<std::unordered_map<std::string, std::string>>, ir::Component>);

    template <typename T>
    struct cast_type {
        using type = T;
    };

    template <typename T>
        requires(std::is_base_of_v<FieldValidator, T>)
    struct cast_type<T> {
        using type = T::CastType;
    };

    template <typename T>
    using cast_type_t = cast_type<T>::type;

    template <typename T>
    struct process_type : get_type<T> {};

    template <map_like T>
    struct process_type<T> {
        using type = T;
    };

    template <typename T>
    using process_type_t = process_type<T>::type;

    template <typename from, typename to>
    to translate_data(const from& data) {
        if constexpr (is_array<to>()) {
            const ir::Args& values = data;
            to result{};

            if (values.size() != result.size()) {
                throw std::runtime_error(
                    std::format("Expected: {} args but got {} args", result.size(), values.size())
                );
            }

            for (int x = 0; x < result.size(); x++) {
                using arr_type = array_type<to>::type;
                using get_type = get_type_t<arr_type>;
                auto val = values[x].get<get_type>();
                auto casted = translate_data<get_type, cast_type_t<arr_type>>(val);
                result[x] = std::move(casted);
            }
            return result;
        } else if constexpr (map_like<to>) {
            static_assert(std::same_as<typename to::key_type, std::string>);
            const ir::Component& values = data;
            using get_type = get_type_t<typename to::mapped_type>;
            using to_type = to::mapped_type;
            to result{};
            result.reserve(values.fields.size());

            for (const auto& [key, value] : values.fields) {
                auto val = translate_data<to_type, get_type>(value.get<get_type>());

                result[key] = std::move(val);
            }
            return result;
        } else {
            return static_cast<to>(data);
        }
    }
} // namespace rox::repr
