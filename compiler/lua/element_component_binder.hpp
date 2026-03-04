#pragma once
#include "sol/state.hpp"
#include "sol/table.hpp"
#include "util.hpp"

namespace rox::lua {

    class ElementComponentBinder;
    template <typename T>
    concept HasSubBind = requires(ElementComponentBinder& r) {
        { T::bind_dependencies(r) };
    };

    class ElementComponentBinder {
    public:
        explicit ElementComponentBinder(sol::state& table) : m_env(table) {}
        ElementComponentBinder(const ElementComponentBinder&) = delete;
        ElementComponentBinder& operator=(const ElementComponentBinder&) = delete;
        ElementComponentBinder(ElementComponentBinder&&) = delete;
        ElementComponentBinder& operator=(ElementComponentBinder&&) = delete;

        template <typename... Args>
        void bind_dependencies() {
            (this->bind_dependencies_impl<Args>(), ...);
        }

    private:
        template <typename T>
        void bind_dependencies_impl() {
            const auto name = T::component_name();

            this->m_env.set_function(name, [](sol::table table) {
                const auto kind = T::component_name();
                table[ElementTag] = kind;
                return table;
            });

            if constexpr (HasSubBind<T>) {
                T::bind_dependencies(*this);
            }
        }

    private:
        sol::state& m_env;
    };

} // namespace rox::lua
