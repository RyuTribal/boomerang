#include "BotPlayer.h"

#include "Util/Random.h"

namespace Boomerang
{
	std::tuple<int, int> BotPlayer::ChooseCard(std::vector<Card*>& cards, bool automatic_choice)
	{
		if(cards.empty())
		{
			return {};
		}
		std::vector<int> card_ids{};
		for(auto &card : cards)
		{
			card_ids.push_back(card->id());
		}

		Random r_generator;

		int random_card_id = r_generator.GetInt(card_ids);

		return {getID(), random_card_id};
	}
}
