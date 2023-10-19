#pragma once
#include "Core/ScoringSystem.h"
#include "RuleSets/Australian/GameConfig/Components.h"

namespace Boomerang
{
	class AnimalsScoringSystem : public ScoringSystem
	{
	public:
		AnimalsScoringSystem(){}
		int UpdateWithArgs(std::vector<Card*>& draft, int player_id, Entity& game_config, std::map<int, std::unique_ptr<Player>>& players) override
		{
			/*
			 *  Animals score: For each pair of matching animals on the cards you drafted this round, you score
			 *	points marked on that animal (Kangaroos are 3, Emus are 4, Wombats are 5, Koalas are 7, and
			 *	Platypuses are 9). Total what you score for all the matching animal pairs you collected this round.
			 */

			int animal_score = 0;

			std::unordered_map<std::string, int> animals_found;

			for (auto& card : draft)
			{
				animals_found[card->GetValueOfComponent<AnimalsComponent>()]++;
			}

			for (auto i = animals_found.begin(); i != animals_found.end(); ++i)
			{
				if (animals_found[i->first] % 2 == 0)
				{
					if (game_config.GetValueOfComponent<AnimalScoreMapComponent>().find(i->first) != game_config.GetValueOfComponent<AnimalScoreMapComponent>().end())
					{
						animal_score += game_config.GetValueOfComponent<AnimalScoreMapComponent>()[i->first] * (i->second / 2);
					}
				}
			}

			return animal_score;
		}
	};
	
}
