#pragma once
#include "Core/ECS/Component.h"

namespace Boomerang
{
	class CardsPerHandComponent : public Component
	{
	public:
		int value;
		std::string type_name() override { return "CardsPerHandComponent"; }
	};

	class RoundsComponent : public Component
	{
	public:
		int value;
		std::string type_name() override { return "RoundsComponent"; }
	};
}
