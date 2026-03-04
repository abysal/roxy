#pragma once
#include "ir/pack_info.hpp"

#include <filesystem>
#include <sol/sol.hpp>

namespace rox::lua {

    class ElementEngine {
    public:
        ElementEngine() = default;
        ElementEngine(const ElementEngine&) = delete;
        ElementEngine& operator=(const ElementEngine&) = delete;
        ElementEngine(ElementEngine&&) = delete;
        ElementEngine& operator=(ElementEngine&&) = delete;

        void init_compiler(ir::PackInfo& compiler) { this->m_compiler = &compiler; }

        void init_engine();
        void compile_element(const std::filesystem::path& lua_file_path);

    private:
        void register_elements();

    private:
        sol::state m_lua_state{};
        ir::PackInfo* m_compiler{};
    };

} // namespace rox::lua
