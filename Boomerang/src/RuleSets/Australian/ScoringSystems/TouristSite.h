#pragma once
#include "Core/ScoringSystem.h"
#include "RuleSets/Australian/GameConfig/Components.h"
#include "RuleSets/Australian/PlayerState/Components.h"

namespace Boomerang
{
	class TouristSiteScoringSystem : public ScoringSystem
	{
	public:
		int UpdateWithArgs(std::vector<Card*>& draft, int player_id, Entity& game_config, std::map<int, std::unique_ptr<Player>>& players) override
		{
			/*
			 *	Tourist sites score: Each card also has a letter (A-Ö) corresponding to a site in Australia. The sites
			 *	are divided into Australia’s seven regions: Western Australia (A-D), Northern Territory (E-H),
			 *	Queensland (I-L), South Australia (M-P), New South Wales (Q-T), Victoria (U-X), and Tasmania (Y-Z,
			 *	*, -). Score one point for each site visited (the letters on the played cards this round) and note down
			 *	which sites have been visited as they do not score points again in coming rounds, but they count
			 *	towards completing a region (see below).
			 *	i. If you are the first player to complete a region you gain 3 bonus points
			 *	ii. If more than one player completes a region at the same time they all gain the 3 bonus points
			 */
			int tourist_points = 0;
			std::vector<std::string> card_regions{};
			for (auto& card : draft)
			{
				if (!players[player_id]->GetValueOfComponent<SitesVisitedComponent>()[card->GetValueOfComponent<SiteComponent>()])
				{
					players[player_id]->GetValueOfComponent<SitesVisitedComponent>()[card->GetValueOfComponent<SiteComponent>()] = true;
					tourist_points++;
				}

				bool has_been_completed = UpdateRegionCompleted(card->GetValueOfComponent<RegionComponent>(), *players[player_id], game_config);

				if (has_been_completed)
				{
					card_regions.push_back(card->GetValueOfComponent<RegionComponent>());
				}
			}

			for (std::string region : card_regions)
			{
				if (!RegionHasBeenCompleted(region, players))
				{
					tourist_points += 3;
				}
			}

			return tourist_points;
		}

		bool UpdateRegionCompleted(std::string region, Player& player, Entity& game_object)
		{
			if (player.GetValueOfComponent<RegionsCompletedComponent>()[region])
			{
				return false;
			}
			for (char letter : game_object.GetValueOfComponent<RegionSiteMapComponent>()[region])
			{
				if (!player.GetValueOfComponent<SitesVisitedComponent>()[letter])
				{
					return false;
				}
			}

			player.GetValueOfComponent<CurrentRoundRegionsComponent>()[region] = true;
			return true;
		}

		bool RegionHasBeenCompleted(std::string region, std::map<int, std::unique_ptr<Player>>& players)
		{
			for (auto & player : players)
			{
				if (player.second->GetValueOfComponent<RegionsCompletedComponent>()[region])
				{
					return false;
				}
			}

			return true;
		}
	};
}
