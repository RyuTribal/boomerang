#pragma once
#include "Core/ECS/Component.h"

namespace Boomerang
{
	class CardsPerHandComponent : public Component
	{
	public:
		int value;
	};

	class RoundsComponent : public Component
	{
	public:
		int value;
	};
}
