#pragma once
#include "component_registry.hpp"

namespace rox::repr {

    struct InventoryInfo {
        std::string category{};
        std::string group{};
        std::string display_name{};
        bool hidden{ false };
    };

    class Block {
    public:
        Block() = default;
        Block(const Block&) = delete;
        Block(Block&&) = delete;
        Block& operator=(const Block&) = delete;
        Block& operator=(Block&&) = delete;

        void add_component(std::unique_ptr<BaseComponent> component);

        void post_process();

    private:
        std::unordered_map<std::string, std::unique_ptr<BaseComponent>> m_components{};
        InventoryInfo m_inv_info{};
    };

} // namespace rox::repr
