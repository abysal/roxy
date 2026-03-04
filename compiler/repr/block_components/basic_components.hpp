#pragma once
#include "repr/component_base.hpp"
#include "rfl/Literal.hpp"

#include <optional>

namespace rox::repr {
    struct LightingComponent {
        static std::string component_name() { return "lighting"; }

        std::optional<Range<uint8_t, 0, 15>> emission{};
        std::optional<Range<uint8_t, 0, 15>> dampening{};
    };

    struct GeometryComponent {
        static std::string component_name() { return "geometry"; }

        std::string model{};
    };

    template <rfl::internal::StringLiteral name, typename storage>
    struct GenericSingleValComponent {
        constexpr static bool single_value{ true };
        static std::string component_name() { return std::string(name.str()); }
        storage value{};
    };

    using CategoryComponent = GenericSingleValComponent<"category", std::string>;
    using DisplayNameComponent = GenericSingleValComponent<"display_name", std::string>;
    using MapColorComponent = GenericSingleValComponent<"map_color", std::array<uint8_t, 3>>;

    // TODO: Add support for the other settings related to textures, such as render methods

    struct TexturesComponent {
        constexpr static bool single_value{ true };
        static std::string component_name() { return "textures"; }
        std::unordered_map<std::string, std::string> value{};
    };

} // namespace rox::repr