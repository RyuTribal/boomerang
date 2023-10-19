#include "AustralianVariant.h"
#include "Core/Directions.h"
#include "ScoringSystems/Activities.h"
#include "Util/MessageParser.h"
#include "ScoringSystems/Systems.h"

namespace Boomerang
{
	void AustralianVariant::BuildRules()
	{
		SetRounds(ROUNDS);
		SetCardsPerHand(CARDS_PER_HAND);
		GetGameConfig()->AddComponent<ActivityScoreMapComponent>();
		GetGameConfig()->AddComponent<AnimalScoreMapComponent>();
		GetGameConfig()->AddComponent<CollectionScoreMapComponent>();
		GetGameConfig()->AddComponent<CollectionMaxComponent>();
		GetGameConfig()->AddComponent<ListActivitiesComponent>();
		GetGameConfig()->AddComponent<RegionSiteMapComponent>();

		AddScoringRules(std::make_unique<ThrowCatchScoringSystem>());
		AddScoringRules(std::make_unique<TouristSiteScoringSystem>());
		AddScoringRules(std::make_unique<ColletionScoringSystem>());
		AddScoringRules(std::make_unique<AnimalsScoringSystem>());
		AddScoringRules(std::make_unique<ActivitiesScoringSystem>());

		SetCardsPerHand(game_config->GetValueOfComponent<CardsPerHandComponent>());
		SetRounds(game_config->GetValueOfComponent<RoundsComponent>());

		for(int i = 0; i < m_CardsPerHand; i++)
		{
			if(i == m_CardsPerHand - 1)
			{
				m_HandOverRules.emplace_back(std::make_tuple(Direction::LEFT, 1));
			}
			else {
				m_HandOverRules.emplace_back(std::make_tuple(Direction::RIGHT, 1));
			}
		}

		for(auto &player : GetPlayers())
		{
			player.second->AddComponent<ActivitiesTakenComponent>();
			player.second->AddComponent<CurrentRoundRegionsComponent>();
			player.second->AddComponent<PointsEachRoundComponent>();
			player.second->AddComponent<RegionsCompletedComponent>();
			player.second->AddComponent<SitesVisitedComponent>();
		}
		
	}
	void AustralianVariant::EndRound(int round)
	{
		for(auto &player: GetPlayers())
		{
			player.second->GetValueOfComponent<RegionsCompletedComponent>() = player.second->GetValueOfComponent<CurrentRoundRegionsComponent>();
		}
	}
	std::tuple<Direction, int>* AustralianVariant::HandOverHand(int draft_turn)
	{
		if(draft_turn < 0)
		{
			return nullptr;
		}
		return &m_HandOverRules.at(draft_turn);
	}

	int AustralianVariant::BreakTie(std::vector<Card*>* player1_draft, int player1_id, std::vector<Card*>* player2_draft, int player2_id)
	{
		ThrowCatchScoringSystem score{};
		int player1_score = score.UpdateWithArgs(*player1_draft, player1_id, *game_config, GetPlayers());
		int player2_score = score.UpdateWithArgs(*player2_draft, player2_id, *game_config, GetPlayers());
		if(player1_score > player2_score)
		{
			return player1_id;
		}else
		{
			return player2_id;
		}
	}
	
}
