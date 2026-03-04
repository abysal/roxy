#pragma once
#include "block.hpp"
#include "repr/pack_repr.hpp"

namespace rox::ir {

    class PackInfo {
    public:
        PackInfo() = default;
        PackInfo(const PackInfo&) = delete;
        PackInfo& operator=(const PackInfo&) = delete;
        PackInfo(PackInfo&&) = delete;
        PackInfo& operator=(PackInfo&&) = delete;

        void add_block(Block&& block);

        [[nodiscard]] repr::PackRepr emit_pack_repr() const;

    private:
        std::vector<Block> m_blocks{};
    };

} // namespace rox::ir
