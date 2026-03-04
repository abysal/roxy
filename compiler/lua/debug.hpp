#pragma once

#include "sol/table.hpp"

#include <format>

namespace rox::lua {}

template <>
struct std::formatter<sol::table> {
    constexpr auto parse(std::format_parse_context& ctx) {
        auto pos = ctx.begin();
        while (pos != ctx.end() && *pos != '}') {
            if (*pos == 'P' || *pos == 'p') {
                this->pretty = true;
            }
            ++pos;
        }
        return pos;
    }

    [[nodiscard]] std::string as_string(sol::table table, int indent = 1) const {
        std::string result{};

        std::format_to(std::back_inserter(result), "{{");
        if (pretty) {
            std::format_to(std::back_inserter(result), "\n");
        }

        for (const auto& [key, val] : table) {
            std::string line{};
            if (key.is<double>()) {
                std::format_to(std::back_inserter(line), "{}: ", key.as<double>());
            } else if (key.is<std::string>()) {
                std::format_to(std::back_inserter(line), "{}: ", key.as<std::string_view>());
            } else {
                std::format_to(std::back_inserter(line), "Unknown Type: ");
            }

            if (val.is<double>()) {
                std::format_to(std::back_inserter(line), "{}", val.as<double>());
            } else if (val.is<std::string>()) {
                std::format_to(std::back_inserter(line), "{}", val.as<std::string_view>());
            } else if (val.is<sol::table>()) {
                std::format_to(std::back_inserter(line), "{}", as_string(val.as<sol::table>(), indent + 1));
            }

            if (this->pretty) {
                for (int x = 0; x < indent; ++x) {
                    result.append("\t");
                }

                line.append("\n");
            } else {
                std::format_to(std::back_inserter(line), " ");
            }

            result.append(line);
        }

        if (this->pretty) {
            for (int x = 1; x < indent; ++x) {
                result.append("\t");
            }
        }

        std::format_to(std::back_inserter(result), "}}");

        if (this->pretty) {
            std::format_to(std::back_inserter(result), "\n");
        } else {
            std::format_to(std::back_inserter(result), " ");
        }

        return result;
    }

    auto format(const sol::table& value, std::format_context& ctx) const {
        auto out = ctx.out();

        if (pretty) {
            out = std::format_to(out, "Table: ");
        }

        out = std::format_to(out, "{}", as_string(value));

        return out;
    }

    bool pretty{ false };
};