#pragma once
#include <unordered_set>

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
            int tourist_points = 0;
            std::unordered_set<std::string> regionsCompletedThisRound;

            auto& playerSitesVisited = players[player_id]->GetValueOfComponent<SitesVisitedComponent>();
            auto& playerRegionsCompleted = players[player_id]->GetValueOfComponent<RegionsCompletedComponent>();

            // 1. Marking the sites that the player has visited in this round and calculating the points.
            for (auto& card : draft)
            {
                const auto& site = card->GetValueOfComponent<SiteComponent>();
                if (!playerSitesVisited[site])
                {
                    playerSitesVisited[site] = true;
                    tourist_points++;
                }

                const auto& region = card->GetValueOfComponent<RegionComponent>();
                if (UpdateRegionCompleted(region, playerSitesVisited, game_config.GetValueOfComponent<RegionSiteMapComponent>()) && !playerRegionsCompleted[region])
                {
                    regionsCompletedThisRound.insert(region);
                }
            }

            // 2. Checking if the player is the first to complete the region or if others also completed it in this round.
            for (const auto& region : regionsCompletedThisRound)
            {
                if (!RegionHasBeenCompletedThisRound(region, players))
                {
                    tourist_points += 3;
                    playerRegionsCompleted[region] = true;
                }
            }

            return tourist_points;
        }

        bool UpdateRegionCompleted(const std::string& region, std::unordered_map<char, bool>& playerSitesVisited, std::unordered_map<std::string, std::string>& regionSiteMap) {
            for (char letter : regionSiteMap[region]) {
                if (!playerSitesVisited[letter]) {
                    return false;
                }
            }
            return true;
        }

        bool RegionHasBeenCompletedThisRound(const std::string& region, std::map<int, std::unique_ptr<Player>>& players) {
            for (auto& player : players) {
                if (player.second->GetValueOfComponent<CurrentRoundRegionsComponent>()[region]) {
                    return true;
                }
            }
            return false;
        }
	};
}