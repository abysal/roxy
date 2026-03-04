#pragma once
#include "block.hpp"
#include "ir/block.hpp"

#include <string>
#include <unordered_map>

namespace rox::repr {
    class PackRepr {
    public:
        PackRepr();
        PackRepr(const PackRepr&) = delete;
        PackRepr& operator=(const PackRepr&) = delete;
        PackRepr(PackRepr&&) = default;
        PackRepr& operator=(PackRepr&&) = default;

        void compile_block(const ir::Block& block);

    private:
        ComponentRegistry m_block_component_registry{};
        std::unordered_map<std::string, Block> m_blocks{};
    };
} // namespace rox::repr
