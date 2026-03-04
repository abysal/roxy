#pragma once
#include "ir/pack_info.hpp"
#include "lua/element_component_binder.hpp"
#include "sol/table.hpp"

#include <string>
#include <string_view>

namespace rox::lua {

    struct BlockBuilderState {
        std::string identifier{};
    };

    class BlockElement {
    public:
        static std::string_view element_name() { return "block"; }

        static BlockBuilderState bind_identifier(const std::string_view identifier) {
            return BlockBuilderState{ std::string(identifier) };
        }

        static void compile_element(ir::PackInfo& compiler, BlockBuilderState& state, sol::table);
    };

    struct LightingComponent {
        static std::string_view component_name() { return "lighting"; }
    };

    struct CullingComponent {
        static std::string_view component_name() { return "culling"; }
    };

    struct TexturesComponent {
        static std::string_view component_name() { return "textures"; }
    };

    struct GeometryComponent {
        static std::string_view component_name() { return "geometry"; }

        struct BoneVisibility {
            static std::string_view component_name() { return "bone_visibility"; }
        };

        static void bind_dependencies(ElementComponentBinder& b) {
            b.bind_dependencies<BoneVisibility>(); //
        }
    };

} // namespace rox::lua
