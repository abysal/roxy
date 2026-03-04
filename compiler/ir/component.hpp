#pragma once
#include "sol/usertype_core.hpp"

#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace rox::ir {

    class Value;
    using Object = std::unordered_map<std::string, Value>;
    using Args = std::vector<Value>;
    enum class IRType {
        Nil,
        Num,
        Str,
        Bool,
        Args,
        Obj
    };

    struct Component {
        Component();
        Component(const Component&);
        Component(Component&&) noexcept;
        Component& operator=(const Component&);
        Component& operator=(Component&&) noexcept;
        ~Component();
        Component(std::string name, Object fields);

        std::string name;
        Object fields;
    };

    class Value {
    public:
        using IRStorage = std::variant<std::monostate, double, std::string, bool, Args, Component>;

        template <typename T>
            requires(!std::same_as<std::remove_cvref_t<T>, Value>)
        explicit Value(T&& value) : m_data(std::forward<T>(value)) {}

        [[nodiscard]] bool is_num() const noexcept { return std::holds_alternative<double>(this->m_data); }

        [[nodiscard]] bool is_str() const noexcept { return std::holds_alternative<std::string>(this->m_data); }

        [[nodiscard]] bool is_bool() const noexcept { return std::holds_alternative<bool>(this->m_data); }

        [[nodiscard]] bool is_args() const noexcept { return std::holds_alternative<Args>(this->m_data); }

        [[nodiscard]] bool is_component() const noexcept { return std::holds_alternative<Component>(this->m_data); }

        [[nodiscard]] double as_num() const noexcept {
            assert(is_num());
            return std::get<double>(this->m_data);
        }

        [[nodiscard]] const auto& as_str() const noexcept {
            assert(is_str());
            return std::get<std::string>(this->m_data);
        }

        [[nodiscard]] bool as_bool() const noexcept {
            assert(is_bool());
            return std::get<bool>(this->m_data);
        }

        [[nodiscard]] const auto& as_args() const noexcept {
            assert(is_args());
            return std::get<Args>(this->m_data);
        }
        [[nodiscard]] const auto& as_component() const noexcept {
            assert(is_component());
            return std::get<Component>(this->m_data);
        }

        template <typename T>
        const auto& get() const {
            assert(std::holds_alternative<T>(this->m_data));
            return std::get<T>(this->m_data);
        }

        [[nodiscard]] const auto& data() const noexcept { return this->m_data; }

    private:
        IRStorage m_data;
    };
} // namespace rox::ir

namespace rox::ir::detail {
    std::format_context::iterator format_value(const Value& v, std::format_context::iterator out, int indent_level);
} // namespace rox::ir::detail

template <>
struct std::formatter<rox::ir::Value> {
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

    auto format(const rox::ir::Value& v, std::format_context& ctx) const {
        return rox::ir::detail::format_value(v, ctx.out(), 0);
    }
};
