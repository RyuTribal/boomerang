#pragma once
#include "Core/Card/Card.h"
#include "Core/ECS/Component.h"

namespace Boomerang
{
	class HandComponent : public Component
	{
	public:
		std::unique_ptr<std::vector<Card*>> value = std::make_unique<std::vector<Card*>>();
	};

	class DraftComponent : public Component
	{
	public:
		std::vector<Card*> value{};
	};

	class RoundScoreComponent : public Component
	{
	public:
		std::vector<int> value{};
	};


}
