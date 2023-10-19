#pragma once
#include "Core/ScoringSystem.h"
#include "RuleSets/Australian/GameConfig/Components.h"

namespace Boomerang
{

	class ColletionScoringSystem : public ScoringSystem
	{
	public:
		ColletionScoringSystem() {}
		int UpdateWithArgs(std::vector<Card*>& draft, int player_id, Entity& game_config, std::map<int, std::unique_ptr<Player>>& players) override
		{
			/*
			 *  Collections score : Many cards also have a collection attribute.Each item has a value associated with
			 *	it : Leaves are 1, Wildflowers are 2, Shells are 3, and Souvenirs are 5. Add up all the values of each
			 *	item you collected on your cards this round.
			 *
			 *	i.If the total for your collections is 1 - 7 then you double that number as your score for that
			 *	round.
			 *	ii.If your value is over 7, you only score your collection number as points.
			*/
			int collection_points = 0;
			for (auto& card : draft)
			{

				if (game_config.GetValueOfComponent<CollectionScoreMapComponent>().find(card->GetValueOfComponent<CollectionsComponent>()) != game_config.GetValueOfComponent<CollectionScoreMapComponent>().end())
				{
					collection_points += game_config.GetValueOfComponent<CollectionScoreMapComponent>()[card->GetValueOfComponent<CollectionsComponent>()];
				}
			}
			return collection_points > game_config.GetValueOfComponent<CollectionMaxComponent>() ? collection_points : collection_points * 2;
		}
	};
	
}
