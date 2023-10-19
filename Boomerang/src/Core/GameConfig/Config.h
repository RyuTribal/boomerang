#pragma once
#include "Components.h"
#include "Core/ECS/Entity.h"

namespace Boomerang
{
	class GameConfig : public Entity
	{
	public:
		GameConfig()
		{
			AddComponent<RoundsComponent>();
			AddComponent<CardsPerHandComponent>();
		}

		std::string type_name() override { return "GameConfig"; }
	};
}
