#pragma once
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace rox::repr {

    class TextureReference {
    public:
        TextureReference() = default;
        explicit TextureReference(const std::string& t) : m_texture_path(t) {};

    private:
        friend class TextureTracker;
        std::string m_texture_path{};
        std::string_view m_texture_name{};
    };

    class TextureTracker {
    public:
        void add_texture(TextureReference&);

    private:
        static std::string flatten_ref_path(const TextureReference& ref);

    private:
        std::unordered_map<std::string, std::string> m_texture_remap{};
        std::unordered_set<std::string> m_used_names{};
    };

} // namespace rox::repr
