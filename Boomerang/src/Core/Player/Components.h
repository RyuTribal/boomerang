#pragma once
#include "Core/Card/Card.h"
#include "Core/ECS/Component.h"

namespace Boomerang
{
	class HandComponent : public Component
	{
	public:
		std::unique_ptr<std::vector<Card*>> value = std::make_unique<std::vector<Card*>>();
		std::string type_name() override { return "HandComponent"; }
	};

	class DraftComponent : public Component
	{
	public:
		std::vector<Card*> value{};
		std::string type_name() override { return "DraftComponent"; }
	};

	class RoundScoreComponent : public Component
	{
	public:
		std::vector<int> value{};
		std::string type_name() override { return "RoundScoreComponent"; }
	};


}
