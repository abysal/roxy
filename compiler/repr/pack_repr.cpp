#include "pack_repr.hpp"

#include "block_components/basic_components.hpp"

namespace rox {
    repr::PackRepr::PackRepr() {
        this->m_block_component_registry.register_component<LightingComponent>();
        this->m_block_component_registry.register_component<GeometryComponent>();
        this->m_block_component_registry.register_component<DisplayNameComponent>();
        this->m_block_component_registry.register_component<CategoryComponent>();
        this->m_block_component_registry.register_component<MapColorComponent>();
        this->m_block_component_registry.register_component<TexturesComponent>();
    }

    void repr::PackRepr::compile_block(const ir::Block& block) {
        auto& blk = this->m_blocks[block.id];

        for (const auto& [id, data] : block.object) {
            auto component = this->m_block_component_registry.parse_from(id, data);
            assert(component);
            blk.add_component(std::move(component));
        }

        blk.post_process();
    }
} // namespace rox