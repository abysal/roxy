#pragma once
#include <optional>
#include <span>
#include <string_view>

namespace rox {

    template <typename Val>
    class Stream {
    public:
        Stream() = default;
        Stream(const Stream&) = default;
        Stream(Stream&&) = default;
        Stream& operator=(const Stream&) = default;
        Stream& operator=(Stream&&) = default;

        explicit Stream(std::string_view data) {
            static_assert(
                std::same_as<Val, char>,
                "A stream can only be a constructed with a string view if the data type is a char"
            );
            this->m_stream_data = std::span<const Val>{ data.data(), data.size() };
        }

        explicit Stream(std::span<const Val> data) { this->m_stream_data = data; }

        [[nodiscard]] size_t current_index() const { return m_current_index; }

        std::optional<Val> consume() noexcept {
            if (this->m_current_index >= m_stream_data.size()) {
                return std::nullopt;
            }

            return this->m_stream_data[this->m_current_index++];
        }

        std::optional<Val> peek(size_t offset = 0) const noexcept {
            if (this->m_current_index + offset >= m_stream_data.size()) {
                return std::nullopt;
            }
            return this->m_stream_data[this->m_current_index + offset];
        }

        void skip(size_t offset = 1) noexcept { this->m_current_index += offset; }

        auto slice(size_t start, size_t size) {
            const auto* data = this->m_stream_data.data() + start;

            if constexpr (std::same_as<Val, char>) {
                return std::string_view{ data, size };
            } else {
                return std::span{ data, size };
            }
        }

        auto slice(size_t size) {
            const auto* data = this->m_stream_data.data() + this->m_current_index;
            if constexpr (std::same_as<Val, char>) {
                if (this->m_current_index + size >= m_stream_data.size()) {
                    return std::optional<std::string_view>{};
                }
                return std::optional{
                    std::string_view{ data, size }
                };
            } else {
                if (this->m_current_index + size >= m_stream_data.size()) {
                    return std::optional<std::span<const Val>>{};
                }
                return std::optional{
                    std::span{ data, size }
                };
            }
        }

    private:
        std::span<const Val> m_stream_data{};
        size_t m_current_index{};
    };

} // namespace rox
