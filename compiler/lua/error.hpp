#pragma once
#include <stdexcept>

namespace rox::lua {

    class Error : public std::runtime_error {
    public:
        explicit Error(const std::string& what) : runtime_error(what) {}
    };
} // namespace rox::lua
