#pragma once
#include "sol/table.hpp"

#define ElementTag "internal_component_type_tag"

namespace rox::lua {
    bool is_array(sol::table);
}