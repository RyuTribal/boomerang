#pragma once
#include "Core/Player/Player.h"

namespace Boomerang
{
	class BotPlayer : public Player
	{
	public:
		BotPlayer(int id) : Player(id) {}
		std::tuple<int, int> ChooseCard(std::vector<Card*>& cards, bool automatic_choice) override;
	};
}
