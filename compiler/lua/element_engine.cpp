#include "element_engine.hpp"
#include "element/block_element.hpp"
#include "error.hpp"
#include "ir/pack_info.hpp"
#include <fstream>
#include <utility>

namespace rox::lua {

    // language=lua
    constexpr static std::string_view main_api_code = R"(
function rgb(r, g, b)
    return {r, g, b}
end

function hex(hex)
    hex = hex:gsub("#", "")
    if #hex ~= 6 then
        error("Hex: " .. hex .. ", is not a valid hex string")
    end

    local r = tonumber(hex:sub(1, 2), 16)
    local g = tonumber(hex:sub(3, 4), 16)
    local b = tonumber(hex:sub(5, 6), 16)

    return {r, g, b}
end
)";

    void ElementEngine::init_engine() {
        this->m_lua_state.open_libraries(
            sol::lib::base,
            sol::lib::package,
            sol::lib::coroutine,
            sol::lib::string,
            sol::lib::os,
            sol::lib::math,
            sol::lib::table,
            sol::lib::debug,
            sol::lib::bit32,
            sol::lib::io
        );

        this->m_lua_state.script(main_api_code);

        this->register_elements();
    }

    void ElementEngine::compile_element(const std::filesystem::path& lua_file_path) {
        std::ifstream ifs(lua_file_path);

        std::stringstream ss{};
        ss << ifs.rdbuf();
        const auto lua_code = ss.str();

        const auto result = this->m_lua_state.safe_script(lua_code);

        if (!result.valid()) {
            const sol::error err = result;
            const auto what = err.what();
            throw Error(what);
        }
    }

    template <typename T>
    concept ElementConcept = requires(T t, std::string_view id, ir::PackInfo& compiler, sol::table tbl) {
        { T::element_name() } -> std::same_as<std::string_view>;

        { t.bind_identifier(id) };

        requires requires(T u) {
            {
                u.compile_element(
                    compiler, std::declval<std::remove_reference_t<decltype(u.bind_identifier(id))>&>(), tbl
                )
            };
        };
    };

    template <ElementConcept T>
    void new_element(sol::state& s, ir::PackInfo& compiler) {
        s.set_function(T::element_name(), [comp = std::ref(compiler)](std::string_view identifier) {
            T instance{};
            auto state = instance.bind_identifier(identifier);
            return [i = std::move(instance), c = std::ref(comp), st = std::move(state)](sol::table table) mutable {
                i.compile_element(c, st, std::move(table));
            };
        });
    }

    void ElementEngine::register_elements() {
        //
        assert(this->m_compiler);
        new_element<BlockElement>(this->m_lua_state, *this->m_compiler);

        ElementComponentBinder binder{ this->m_lua_state };

        binder.bind_dependencies<LightingComponent, CullingComponent, TexturesComponent, GeometryComponent>();
    }
} // namespace rox::lua