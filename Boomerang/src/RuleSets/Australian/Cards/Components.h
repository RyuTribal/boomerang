#pragma once
#include <string>

#include "Core/ECS/Component.h"

namespace Boomerang
{

	class SiteComponent : public Component
	{
    public:
        SiteComponent(char site) : value{ site } {}
        char value;

        std::string ToString() override
        {
            return std::to_string(value);
        }

        std::string type_name() override { return "SiteComponent"; }
	};

    class RegionComponent : public Component
    {
    public:
        RegionComponent(std::string region) : value{ region } {}

        std::string value;

        std::string ToString() override
        {
            return value;
        }
        std::string type_name() override { return "RegionComponent"; }
    };

    class NumberComponent : public Component
    {
    public:
        NumberComponent(int number) : value{ number } {}
        int value;

        std::string ToString() override
        {
            return std::to_string(value);
        }
        std::string type_name() override { return "NumberComponent"; }
    };

    class CollectionsComponent : public Component
    {
    public:
        CollectionsComponent(std::string collections) : value{ collections } {}
        std::string value;

        std::string ToString() override
        {
            return value;
        }
        std::string type_name() override { return "CollectionComponent"; }
    };

    class AnimalsComponent : public Component
    {
    public:
        AnimalsComponent(std::string animals) : value{ animals } {}
        std::string value;

        std::string ToString() override
        {
            return value;
        }
        std::string type_name() override { return "AnimalsComponent"; }
    };

    class ActivitiesComponent : public Component
    {
    public:
        ActivitiesComponent(std::string activties) : value{ activties } {}
        std::string value;

        std::string ToString() override
        {
            return value;
        }
        std::string type_name() override { return "ActivitiesComponent"; }
    };
}
