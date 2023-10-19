#pragma once
#include "Core/ScoringSystem.h"
#include "Platform/NetInstance.h"
#include "RuleSets/Australian/Cards/Components.h"
#include "RuleSets/Australian/GameConfig/Components.h"
#include "RuleSets/Australian/PlayerState/Components.h"
#include "Util/MessageParser.h"


namespace Boomerang
{
	class ActivitiesScoringSystem : public ScoringSystem
	{
	public:
		ActivitiesScoringSystem(){}
		int UpdateWithArgs(std::vector<Card*>& draft, int player_id, Entity& game_config, std::map<int, std::unique_ptr<Player>>& players) override
		{
			/*
			 *  Activities score: Scoring activities is optional. For any single activity you would like to score each
			 *	round, count how many of that activity you have on your seven cards and score the corresponding
			 *	points:
			 *	# Activity cards 1 2 3 4 5 6
			 *	# Score 0 2 4 7 10 15
			 *	i. You may only score one Activity per round.
			 *	ii. You may only score each Activity once per game (collection of rounds).
			 */

			auto activityCounts = CountActivities(draft, player_id, players);

			std::string chosen_activity = GetChosenActivity(player_id, activityCounts, players);

			return ScoreActivity(chosen_activity, activityCounts, game_config, players, player_id);
		}

		std::unordered_map<std::string, int> CountActivities(const std::vector<Card*>& draft, int player_id, std::map<int, std::unique_ptr<Player>>& players)
		{
			std::unordered_map<std::string, int> activityCounts;
			for (auto& card : draft)
			{
				std::string activity = card->GetValueOfComponent<ActivitiesComponent>();
				if (!activity.empty() && !players[player_id]->GetValueOfComponent<ActivitiesTakenComponent>()[activity]) {
					activityCounts[activity]++;
					
				}
			}
			return activityCounts;
		}

		std::string GetChosenActivity(int player_id, const std::unordered_map<std::string, int>& activityCounts, std::map<int, std::unique_ptr<Player>>& players)
		{
			Message msg;
			msg.status = Status::CLIENTS_TURN;
			msg.message = "Here are your activities that you can choose this round:\n";

			for (const auto& activity : activityCounts)
			{
				msg.message += activity.first + " [" + std::to_string(activity.second) + "] \n";
			}

			// Remove the trailing comma and space
			msg.message.pop_back();
			msg.message.pop_back();

			msg.message += "\n\n If don't want to choose press [n/N]";

			bool sent = NetworkInstance::GetInstance().BroadcastToAClient(player_id, MessageParser::EncodeMessage(msg));
			std::string chosen_activity = "";
			if (sent)
			{
				while (true) {
					Message response = MessageParser::DecodeMessage(NetworkInstance::GetInstance().AwaitClient(player_id));

					if (response.message == "N" || response.message == "n")
					{
						break;
					}else if (activityCounts.find(response.message) != activityCounts.end())
					{
						chosen_activity = response.message;
						break;
					}else
					{
						Message error_msg;
						error_msg.status = Status::CLIENTS_TURN;
						error_msg.message = "Invalid answer, please choose again!";
						NetworkInstance::GetInstance().BroadcastToAClient(player_id, MessageParser::EncodeMessage(error_msg));
					}
				}

			}
			else
			{
				std::vector<Card*> activity_having_cards{};
				auto playerHand = players[player_id]->GetHand();

				for (Card* card : *playerHand)
				{
					if (activityCounts.find(card->GetValueOfComponent<ActivitiesComponent>()) != activityCounts.end())
					{
						activity_having_cards.push_back(card);
					}
				}

				int chosen_card_id = std::get<1>(players[player_id]->ChooseCard(activity_having_cards, false));
				for(Card* card : activity_having_cards)
				{
					if(card->id() == chosen_card_id)
					{
						chosen_activity = card->GetValueOfComponent<ActivitiesComponent>();
					}
				}
			}

			return chosen_activity;
		}

		int ScoreActivity(const std::string& chosen_activity, std::unordered_map<std::string, int> activity_count, Entity& game_config, std::map<int, std::unique_ptr<Player>>& players, int player_id)
		{
			auto& activitiesList = game_config.GetValueOfComponent<ListActivitiesComponent>();
			auto& activityScoreMap = game_config.GetValueOfComponent<ActivityScoreMapComponent>();

			if (std::find(activitiesList.begin(), activitiesList.end(), chosen_activity) != activitiesList.end())
			{
				players[player_id]->GetValueOfComponent<ActivitiesTakenComponent>()[chosen_activity] = true;
				return activityScoreMap[activity_count[chosen_activity]];
			}
			return 0;
		}

	};
	
}
