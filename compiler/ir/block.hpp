#pragma once
#include "component.hpp"

namespace rox::ir {
    struct Block {
        std::string id{};
        Object object{};
    };
} // namespace rox::ir