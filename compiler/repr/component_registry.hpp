#pragma once
#include "component_base.hpp"
#include "ir/component.hpp"
#include "rfl/fields.hpp"
#include "rfl/internal/all_fields.hpp"
#include "rfl/internal/variant/result_t.hpp"
#include "rfl/to_view.hpp"
#include "util.hpp"
#include "util/type.hpp"

#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>

namespace rox::repr {
    class BaseComponent {
    public:
        virtual ~BaseComponent() = default;
        [[nodiscard]] virtual std::string name() const = 0;

        template <typename T>
        T& as() {
            constexpr auto hash = type_hash<T>();
            auto* data = this->get_raw_data_with_validation(hash);
            return *static_cast<T*>(data);
        }

    protected:
        virtual void* get_raw_data_with_validation(TypeHash comp) = 0;
    };

    struct AbstractComponentFactory {
        virtual ~AbstractComponentFactory() = default;
        virtual std::unique_ptr<BaseComponent> parse_from(const ir::Value&) = 0;
    };

    class ComponentRegistry {
    public:
        ComponentRegistry() = default;

        template <typename T>
        void register_component() {
            struct TWrapper : BaseComponent {
                TWrapper(T&& value) : value(std::move(value)) {}
                T value;
                [[nodiscard]] std::string name() const override { return T::component_name(); }

                void* get_raw_data_with_validation(TypeHash comp) override {
                    assert(type_hash<T>() == comp);
                    return &this->value;
                }
            };

            struct TFactory : AbstractComponentFactory {
                ~TFactory() override = default;

                std::unique_ptr<BaseComponent> parse_from(const ir::Value& value) override {
                    T instance{};
                    auto view = rfl::to_view(instance);

                    view.apply([&]<typename raw_type>(const raw_type& f) {
                        using member_type = std::remove_pointer_t<typename std::decay_t<raw_type>::Type>;
                        using no_opt_member_type = no_opt_t<member_type>;
                        using member_name = std::decay_t<raw_type>::Name;
                        using get_type = get_type_t<no_opt_member_type>;

                        const auto name = std::string(f.name());
                        if constexpr (is_single_value_v<T>) {
                            assert(f.name() == "value");
                        } else {
                            const auto& obj = value.as_component();
                            if (!obj.fields.contains(name)) {
                                if constexpr (!is_optional<member_type>()) {
                                    throw std::logic_error(std::format("Missing required field: {}", f.name()));
                                } else {
                                    return;
                                }
                            }
                        }

                        const auto raw_value = [&] -> const get_type& {
                            if constexpr (is_single_value_v<T>) {
                                return value.get<get_type>();
                            } else {
                                const auto& obj = value.as_component();
                                return obj.fields.at(name).get<get_type>();
                            }
                        }();

                        validate<no_opt_member_type>(raw_value);

                        const auto result = translate_data<get_type, cast_type_t<no_opt_member_type>>(raw_value);
                        *view.template get<first_literal<member_name>::value>() = result;
                    });

                    return std::make_unique<TWrapper>(std::move(instance));
                }
            };

            auto name = T::component_name();

            std::unique_ptr<AbstractComponentFactory> factory = std::make_unique<TFactory>();
            this->m_registered_components.insert({ name, std::move(factory) });
        }

        auto parse_from(const std::string& id, const ir::Value& value) const {
            assert(this->m_registered_components.contains(id));
            return this->m_registered_components.at(id)->parse_from(value);
        }

    private:
        std::unordered_map<std::string, std::unique_ptr<AbstractComponentFactory>> m_registered_components{};
    };

} // namespace rox::repr
