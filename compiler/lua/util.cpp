#include "util.hpp"

#include <ranges>

namespace rox::lua {
    bool is_array(sol::table table) {
        for (const auto& [key, value] : table) {
            if (!key.is<double>()) {
                return false;
            }
        }
        return true;
    }
} // namespace rox::lua