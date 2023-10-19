#pragma once
#include <string>
#include <unordered_map>

#include "Core/ECS/Component.h"

namespace Boomerang
{
    class SitesVisitedComponent : public Component {
    public:
		std::unordered_map<char, bool> value{
			{'A', false}, {'B', false}, {'C', false},
			{'D', false}, {'E', false}, {'F', false},
			{'G', false}, {'H', false}, {'I', false},
			{'J', false},{'K', false}, {'L', false},
			{'M', false},{'N', false}, {'O', false},
			{'P', false}, {'Q', false}, {'R', false},
			{'S', false},{'T', false}, {'V', false},
			{'W', false},{'X', false}, {'Y', false},
			{'Z', false},{'*', false}, {'-', false}
		};
    };

	class RegionsCompletedComponent : public Component {
	public:
		std::unordered_map<std::string, bool> value{
		{"Western Australia", false},
		{"Northern Territory", false},
		{"Queensland", false},
		{"South Australia", false},
		{"New South Wales", false},
		{"Victoria", false},
		{"Tasmania", false}
		};
	};

	class CurrentRoundRegionsComponent : public Component {
	public:
		std::unordered_map<std::string, bool> value{
		{"Western Australia", false},
		{"Northern Territory", false},
		{"Queensland", false},
		{"South Australia", false},
		{"New South Wales", false},
		{"Victoria", false},
		{"Tasmania", false}
		};
	};

	class ActivitiesTakenComponent : public Component {
	public:
		std::unordered_map<std::string, bool> value;
	};

	class PointsEachRoundComponent : public Component {
	public:
		std::vector<int> value;
	};

	
}
