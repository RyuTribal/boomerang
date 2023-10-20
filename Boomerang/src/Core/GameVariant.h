#pragma once

#include <map>
#include <tuple>
#include <unordered_map>
#include <vector>
#include "Directions.h"
#include "ScoringSystem.h"
#include "Card/Card.h"
#include "ECS/Entity.h"
#include "GameConfig/Config.h"
#include "Player/Player.h"
#include "PlayerTypes/PlayerFactory.h"

namespace Boomerang
{
	class GameVariant
	{
	public:
		GameVariant()
		{
			game_config = std::make_unique<GameConfig>();
			b_RulesInitialized = true;
		}
		virtual std::tuple<int, int> CalculateRoundScores(int round, std::vector<Card*>* draft, int player_id)
		{
			int total_score = 0;
			for(auto &system : scoring_rules)
			{
				total_score += system->UpdateWithArgs(*draft, player_id, *game_config, players);
			}

			return { player_id, total_score };
		}
		virtual std::tuple<Direction, int>* HandOverHand(int round) = 0;
		virtual void BuildRules() = 0;
		virtual int BreakTie(std::vector<Card*>* player1_draft, int player1_id, std::vector<Card*>* player2_draft, int player2_id) = 0;

		virtual void EndRound(int round) = 0;

		void CheckRulesValidity() const
		{
			if (!b_RulesInitialized || !b_CardsPerHandInitialized || !b_RoundsInitialized || players.size() < m_MinPlayers || players.size() > m_MaxPlayers)
			{
				std::string err_message = "";
				if (!b_RulesInitialized) {
					err_message += "Forgot to initialize the base class constructor,"
						"make sure to call on it like so 'YourConstructor() : GameVariant() {}'\n";
				}
				if(!b_CardsPerHandInitialized)
				{
					err_message += "Forgot to initialize how many cards per hand each player get. Please call SetCardsPerHand(int cards_per_hand) in order to set it \n";
				}

				if(!b_RoundsInitialized)
				{
					err_message += "Forgot to initialize how many rounds there is in a game. Please call SetRounds(int rounds) in order to set it \n";
				}

				if (players.size() < m_MinPlayers)
				{
					err_message += "Too few players \n";
				}

				if(players.size() > m_MaxPlayers)
				{
					err_message += "Too many players \n";
				}

				HANDLE_ERROR(ErrorType::BOOMERANG, err_message);
			}
		}
		GameConfig* GetGameConfig() { return game_config.get(); }
		void SetRounds(int rounds)
		{
			b_RoundsInitialized = true;
			GetGameConfig()->GetValueOfComponent<RoundsComponent>() = rounds;
			m_Rounds = rounds;
		}
		void SetCardsPerHand(int cards_per_hand)
		{
			b_CardsPerHandInitialized = true;
			GetGameConfig()->GetValueOfComponent<CardsPerHandComponent>() = cards_per_hand;
			m_CardsPerHand = cards_per_hand;
		}
		int AddPlayer(PlayerTypes type)
		{
			std::unique_ptr<Player> player = PlayerFactory::CreatePlayerClass(type);
			int player_id = player->getID();
			players[player_id] = std::move(player);
			return player_id;
		}

		int AddPlayer(PlayerTypes type, int id)
		{
			std::unique_ptr<Player> player = PlayerFactory::CreatePlayerClass(type, id);
			players[id] = std::move(player);
			return id;
		}

		void SetMinPlayers(int min_players) { m_MinPlayers = min_players; }
		void SetMaxPlayers(int max_players) { m_MaxPlayers = max_players; }

		int GetMinPlayers() { return m_MinPlayers; }
		int GetMaxPlayers() { return m_MaxPlayers; }

		void AddScoringRules(std::unique_ptr<ScoringSystem> rule) { scoring_rules.push_back(std::move(rule)); }

		std::map<int, std::unique_ptr<Player>>& GetPlayers() { return players; }

		int Rounds() { return m_Rounds; }

		int HandSize() { return m_CardsPerHand; }

	protected:
		std::vector<std::tuple<Direction, int>> m_HandOverRules;
		int m_Rounds;
		int m_CardsPerHand;
		int m_MinPlayers = 2;
		int m_MaxPlayers = 4;

		bool b_RoundsInitialized = false;
		bool b_CardsPerHandInitialized = false;


		std::map<int, std::unique_ptr<Player>> players{};
		std::unique_ptr<GameConfig> game_config;
		std::vector<std::unique_ptr<ScoringSystem>> scoring_rules{};

		bool b_RulesInitialized = false;
	};
}
