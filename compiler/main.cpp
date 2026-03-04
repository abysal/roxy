#include "lua/element_engine.hpp"
#include "lua/error.hpp"
#include "repr/pack_repr.hpp"

#include <print>
int main() {
    rox::lua::ElementEngine engine{};
    rox::ir::PackInfo compiler{};
    engine.init_compiler(compiler);
    engine.init_engine();

    try {
        engine.compile_element("example/lamp.lua");
    } catch (rox::lua::Error& err) {
        std::println("Element error: {}", err.what());
    } catch (sol::error& err) {
        std::println("Lua error: {}", err.what());
    }

    const auto repr = compiler.emit_pack_repr();
    return 0;
}