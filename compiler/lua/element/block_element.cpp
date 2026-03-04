#include "block_element.hpp"

#include "component.hpp"
#include "lua/debug.hpp"
#include <print>
#include <ranges>

namespace rox::lua {
    void BlockElement::compile_element(ir::PackInfo& compiler, BlockBuilderState& state, sol::table element) {
        ir::Block block{};
        block.id = state.identifier;
        std::println("{}, {:p}", state.identifier, element);

        // Handles all named fields
        for (const auto& [key, value] : element) {
            if (!key.is<std::string>()) {
                continue;
            }

            const auto& name = key.as<std::string>();
            auto val = into_value(value);
            block.object.insert({ name, std::move(val) });
        }

        // handles composite objects
        for (const auto& [key, value] : element) {
            if (!key.is<double>()) {
                continue;
            }
            auto val = into_value(value);
            assert(val.is_component());
            auto name = val.as_component().name;

            block.object.insert({ name, std::move(val) });
        }

        for (const auto& [key, obj] : block.object) //
            std::println("{}: {}", key, obj);

        compiler.add_block(std::move(block));
    }
} // namespace rox::lua