#pragma once
#include <format>
#include <string>

namespace rox {

    class Identifier {
    public:
        Identifier() = default;
        Identifier(std::string namespace_, std::string name)
            : m_namespace(std::move(namespace_)), m_name(std::move(name)) {
            this->m_whole = std::format("{}:{}", this->m_namespace, this->m_name);
        }

        const auto& get_namespace() const noexcept { return m_namespace; }
        const auto& get_name() const noexcept { return m_name; }
        const auto& get_whole() const noexcept { return this->m_whole; }

    private:
        std::string m_namespace{};
        std::string m_name{};
        std::string m_whole{};
    };

} // namespace rox
