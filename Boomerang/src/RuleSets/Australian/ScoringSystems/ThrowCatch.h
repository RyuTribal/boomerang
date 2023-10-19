#pragma once
#include "Core/ScoringSystem.h"
#include "RuleSets/Australian/Cards/Components.h"

namespace Boomerang
{
	class ThrowCatchScoringSystem : public ScoringSystem
	{
	public:
		int UpdateWithArgs(std::vector<Card*>& draft, int player_id, Entity& game_config, std::map<int, std::unique_ptr<Player>>& players) override
		{
			/*
			 * Throw and catch score: Compare the number on your Throw(first) card and the Catch(final) card and
			 * calculate the difference(the absolute value) : this is your Throw and catch score
			*/

			Card* firstCard = draft.at(0);
			Card* lastCard = draft.at(draft.size() - 1);

			return std::abs(firstCard->GetValueOfComponent<NumberComponent>() - lastCard->GetValueOfComponent<NumberComponent>());
		}
	};
}
