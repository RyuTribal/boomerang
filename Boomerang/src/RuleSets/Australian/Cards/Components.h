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
            return "Site: " + std::to_string(value);
        }
	};

    class RegionComponent : public Component
    {
    public:
        RegionComponent(std::string region) : value{ region } {}

        std::string value;

        std::string ToString() override
        {
            return "Region: " + value;
        }
    };

    class NumberComponent : public Component
    {
    public:
        NumberComponent(int number) : value{ number } {}
        int value;

        std::string ToString() override
        {
            return "Number: " + std::to_string(value);
        }
    };

    class CollectionsComponent : public Component
    {
    public:
        CollectionsComponent(std::string collections) : value{ collections } {}
        std::string value;

        std::string ToString() override
        {
            return "Collections: " + value;
        }
    };

    class AnimalsComponent : public Component
    {
    public:
        AnimalsComponent(std::string animals) : value{ animals } {}
        std::string value;

        std::string ToString() override
        {
            return "Animals: " + value;
        }
    };

    class ActivitiesComponent : public Component
    {
    public:
        ActivitiesComponent(std::string activties) : value{ activties } {}
        std::string value;

        std::string ToString() override
        {
            return "Activities: " + value;
        }
    };
}
