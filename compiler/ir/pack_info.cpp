#include "pack_info.hpp"

namespace rox::ir {
    void PackInfo::add_block(Block&& block) { m_blocks.push_back(std::move(block)); }

    repr::PackRepr PackInfo::emit_pack_repr() const {
        repr::PackRepr repr{};

        for (const auto& block : m_blocks) {
            repr.compile_block(block);
        }

        return repr;
    }
} // namespace rox::ir