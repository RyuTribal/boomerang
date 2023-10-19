#pragma once
#include <string>
#include "Core/ECS/Component.h"

namespace Boomerang
{

    // Don't think there was another component that was generic across all cards, even region was different
    class NameComponent : public Component
    {
    public:
        NameComponent(std::string name) : value{name}{}
        std::string value;

        std::string ToString() override
        {
            return "Name: " + value;
        }
    };
}
