#pragma once
#include <memory>
#include <typeindex>
#include <unordered_map>
#include "Component.h"
#include "Util/Log.h"

namespace Boomerang
{
    class Entity {

    public:
        template<typename T, typename... Args>
        void AddComponent(Args&&... args) {
            static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
            components[std::type_index(typeid(T))] = std::make_shared<T>(std::forward<Args>(args)...);
        }

        template<typename T>
        T* GetComponent() {
            auto it = components.find(std::type_index(typeid(T)));
            if (it != components.end()) {
                return dynamic_cast<T*>(it->second.get());
            }
            return nullptr;
        }

        template<typename T>
        auto& GetValueOfComponent() {
            auto component = GetComponent<T>();
            if(component == nullptr)
            {
                HANDLE_ERROR(ErrorType::BOOMERANG, "Attempted to access a non existent component");
            }
            return component->value;
        }

        // String based approach, less type safe and not recommended unless necessary

        Component* GetComponentByName(const std::string& name) const {
            for (const auto& comp : components) {
                if (comp.second->type_name() == name) {
                    return comp.second.get();
                }
            }
            return nullptr;
        }

        bool HasComponent(std::string name) {
            return GetComponentByName(name) != nullptr;
        }

        std::unordered_map<std::type_index, std::shared_ptr<Component>>& GetComponents() { return components; }

        virtual std::string ToString() { return ""; }
        virtual std::string type_name() = 0;

    private:
        std::unordered_map<std::type_index, std::shared_ptr<Component>> components;
    };
}
