#pragma once
#include <string>
#include <unordered_map>

#include "Core/ECS/Component.h"


namespace Boomerang
{
    class AnimalScoreMapComponent : public Component {
    public:
        std::unordered_map<std::string, int> value = {
            {"Kangaroos", 3},
            {"Emus", 4},
            {"Wombats", 5},
            {"Koalas", 7},
            {"Platypuses", 9}
        };

        std::string type_name() override { return "AnimalScoreMap"; }
    };

    class CollectionScoreMapComponent : public Component {
    public:
        std::unordered_map<std::string, int> value = {
            {"Leaves", 1},
            {"Wildflowers", 2},
            {"Shells", 3},
            {"Souvenirs", 5}
        };

        std::string type_name() override { return "CollectionScoreMapComponent"; }
    };

    class ActivityScoreMapComponent : public Component {
    public:
        std::unordered_map<int, int> value = {
            {1, 0},
            {2, 2},
            {3, 4},
            {4, 7},
            {5, 10},
            {6, 15}
        };

        std::string type_name() override { return "ActivityScoreMapComponent"; };
    };

    class RegionSiteMapComponent : public Component {
    public:
        std::unordered_map<std::string, std::string> value = {
            {"Western Australia", "ABCD"},
            {"Northern Territory", "EFGH"},
            {"Queensland", "IJKL"},
            {"South Australia", "MNOP"},
            {"New South Wales", "QRST"},
            {"Victoria", "UVWX"},
            {"Tasmania", "YZ*-"}
        };

        std::string type_name() override { return "RegionSiteMapComponent"; }
    };

    class CollectionMaxComponent : public Component {
    public:
        int value = 7;

        std::string type_name() override { return "CollectionMaxComponent"; }
    };

    class ListActivitiesComponent : public Component
    {
    public:
        std::vector<std::string> value = {
            "Indigenous Culture",
            "Sightseeing",
            "Bushwalking",
            "Swimming"
        };

        std::string type_name() override { return "ListActivitiesComponent"; }
    };
}
