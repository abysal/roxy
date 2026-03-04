#include "component.hpp"
#include "lua/util.hpp"

#include <ranges>

namespace rox::lua {
    ir::Value into_value(sol::object object) {
        if (object.is<double>()) {
            return ir::Value(object.as<double>());
        }

        if (object.is<std::string>()) {
            return ir::Value(object.as<std::string>());
        }

        if (object.is<bool>()) {
            return ir::Value(object.as<bool>());
        }

        if (!object.is<sol::table>()) {
            throw std::runtime_error{ "BAD" };
        }

        const auto table = object.as<sol::table>();
        if (is_array(table)) {
            std::vector<ir::Value> values{};
            // Ranges values aren't used here because sol crashes :sob:
            for (const auto& [_key, value] : table) {
                values.emplace_back(into_value(value));
            }
            return ir::Value(std::move(values));
        }

        if (table.get_or<std::string>(ElementTag, "").empty()) {
            throw std::runtime_error{ "No element tag found in object" };
        }

        ir::Object object_data{};

        auto component_name = table.get<std::string>(ElementTag);

        for (const auto& [key, value] : table) {
            auto val = into_value(value);
            if (key.is<double>()) {
                assert(val.is_component());
                const auto name = val.as_component().name;
                object_data.insert({ name, std::move(val) });

            } else {

                if (key.as<std::string>() == ElementTag) {
                    continue;
                }

                object_data.insert({ key.as<std::string>(), std::move(val) });
            }
        }

        return ir::Value{
            ir::Component{ std::move(component_name), std::move(object_data) }
        };
    }
} // namespace rox::lua