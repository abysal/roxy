#include "block.hpp"

#include "block_components/basic_components.hpp"

namespace rox::repr {
    void Block::add_component(std::unique_ptr<BaseComponent> component) {
        const auto id = component->name();

        this->m_components.insert({ id, std::move(component) });
    }

    void Block::post_process() {
        for (const auto& [name, comp] : this->m_components) {
            if (name == CategoryComponent::component_name()) {
                const auto& category_comp = comp->as<CategoryComponent>();
                this->m_inv_info.category = category_comp.value;
                continue;
            }

            if (name == DisplayNameComponent::component_name()) {
                const auto& display_comp = comp->as<DisplayNameComponent>();
                this->m_inv_info.display_name = display_comp.value;
                continue;
            }
        }
    }
} // namespace rox::repr