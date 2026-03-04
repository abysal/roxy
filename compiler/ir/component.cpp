#include "component.hpp"

namespace rox::ir {
    Component::Component() = default;
    Component::Component(const Component&) = default;
    Component::Component(Component&&) noexcept = default;
    Component& Component::operator=(const Component&) = default;
    Component& Component::operator=(Component&&) noexcept = default;
    Component::~Component() = default;
    Component::Component(std::string name, Object fields) : name(std::move(name)), fields(std::move(fields)) {}

} // namespace rox::ir

namespace rox::ir::detail {

    inline void indent(std::format_context::iterator& out, int level) {
        for (int i = 0; i < level; ++i)
            out = std::format_to(out, " ");
    }

    std::format_context::iterator format_value(const Value& v, std::format_context::iterator out, int indent_level) {
        if (v.is_num()) {
            out = std::format_to(out, "{}", v.as_num());
        } else if (v.is_str()) {
            out = std::format_to(out, "\"{}\"", v.as_str());
        } else if (v.is_bool()) {
            out = std::format_to(out, "{}", v.as_bool() ? "true" : "false");
        } else if (v.is_args()) {
            const auto& args = v.as_args();
            out = std::format_to(out, "[\n");

            for (const auto& arg : args) {
                indent(out, indent_level + 2);
                out = format_value(arg, out, indent_level + 2);
                out = std::format_to(out, ",\n");
            }

            indent(out, indent_level);
            out = std::format_to(out, "]");
        } else if (v.is_component()) {
            const auto& c = v.as_component();
            out = std::format_to(out, "{} {{\n", c.name);

            for (const auto& [key, field] : c.fields) {
                indent(out, indent_level + 2);
                out = std::format_to(out, "{}: ", key);
                out = format_value(field, out, indent_level + 2);
                out = std::format_to(out, "\n");
            }

            indent(out, indent_level);
            out = std::format_to(out, "}}");
        } else {
            // monostate / nil
            out = std::format_to(out, "null");
        }

        return out;
    }

} // namespace rox::ir::detail
