#include "texture_tracker.hpp"

namespace rox {
    void repr::TextureTracker::add_texture(TextureReference& ref) {
        if (!this->m_texture_remap.contains(ref.m_texture_path)) {
            auto flattened = this->flatten_ref_path(ref);

            size_t remove_count{ 0 };
            size_t i{ 0 };
            while (this->m_used_names.contains(flattened)) {
                for (int x = 0; x < remove_count; x++) {
                    flattened.pop_back();
                }

                const auto before = flattened.size();
                flattened += i++;
                const auto after = flattened.size();
                remove_count = after - before;
            }

            this->m_texture_remap[ref.m_texture_path] = std::move(flattened);
        }

        ref.m_texture_name = this->m_texture_remap[ref.m_texture_path];
    }

    std::string repr::TextureTracker::flatten_ref_path(const TextureReference& ref) {
        std::string result{};
        result.reserve(ref.m_texture_path.size());

        for (const auto c : ref.m_texture_path) {
            if (isalnum(c)) {
                result += c;
            } else if (c == '/') {
                result += '_';
            } else if (c == ' ') {
                result += '_';
            }
        }

        return result;
    }
} // namespace rox