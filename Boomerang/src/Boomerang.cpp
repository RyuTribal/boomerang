#include "Boomerang.h"
#include "Util/Log.h"
#include "Platform/NetInstance.h"
#include "Util/ThreadPool.h"


namespace Boomerang
{
	Boomerang::Boomerang(Deck* deck, GameVariant* variant) :m_GameVariant{ variant }, m_Deck{ deck }
	{
		m_Deck->BuildDeck();
	}
	void Boomerang::CopyPlayerDecks(std::map<int, std::unique_ptr<std::vector<Card*>>>& copy_map)
	{
		for (const auto& [key, player] : variant()->GetPlayers())
		{
			copy_map[key] = std::move(player->MoveHand());
		}
	}
	Player* Boomerang::GetPlayerById(int id)
	{
		for(auto &player : variant()->GetPlayers())
		{
			if(player.second->getID() == id)
			{
				return player.second.get();
			}
		}
		return nullptr;
	}
	void Boomerang::run()
	{
		variant()->CheckRulesValidity();
		NetworkInstance::GetInstance().BroadcastToClients("Game started!");
		GAME_INFO("Game has started");
		int current_round = 1;
		while(current_round <= variant()->Rounds())
		{
			NetworkInstance::GetInstance().BroadcastToClients("Start of round " + std::to_string(current_round));
			DealCards();
			GoThroughCards();
			HandleScoring(current_round);
			ShowRoundScore(current_round);
			variant()->EndRound(current_round);
			current_round++;
		}

		ShowEndScores();
		int winner_id = CheckWinner();

		Message msg;
		msg.status = Status::END;
		msg.message = "Winner: Player " + std::to_string(winner_id);

		NetworkInstance::GetInstance().BroadcastToClients(MessageParser::EncodeMessage(msg));
		GAME_INFO("Game ended!");
	}

	void Boomerang::GoThroughCards()
	{
		int card_selection_count = 0;
		while (card_selection_count < variant()->HandSize()) {

				PlayersSelect();
				if (!variant()->GetPlayers().begin()->second->GetHand()->empty())
				{
					PassHands(card_selection_count);
					PrintLatestCards();
				}

				card_selection_count++;
		}
	}

	void Boomerang::DealCards()
	{

		deck()->ShuffleDeck();
		for (auto& player : variant()->GetPlayers())
		{
			for (int i = 0; i < variant()->HandSize(); i++) {
				player.second->AddToHand(deck()->TakeCard());
			}
		}
	}

	void Boomerang::PlayersSelect()
	{
		ThreadPool<std::tuple<int, int>> pool(variant()->GetPlayers().size());
		for (auto& player : variant()->GetPlayers())
		{
			pool.submit_task([&player]() -> std::tuple<int, int> {
				return player.second->ChooseCard(*(player.second->GetHand()), true);
			});
		}

		std::shared_ptr <std::vector<std::tuple<int, int>>> responses = pool.run_tasks();

		GAME_INFO("Responses: {0}", responses->size());

		for(std::tuple<int, int> answer : *responses)
		{
			GetPlayerById(std::get<0>(answer))->AddToDraft(deck()->GetCardById(std::get<1>(answer)));
			GAME_INFO("Player {0} chose: {1}", std::get<0>(answer), std::get<1>(answer));
		}
	}

	void Boomerang::PassHands(int card_selection_count)
	{
		std::tuple<Direction, int>* hand_over_move = variant()->HandOverHand(card_selection_count);
		if (hand_over_move == nullptr)
		{
			GAME_ERROR("Received NULL on the hand over move");
			return;
		}
		Direction direction = std::get<0>(*hand_over_move);
		int steps = std::get<1>(*hand_over_move);

		std::map<int, std::unique_ptr<std::vector<Card*>>> old_hands;
		CopyPlayerDecks(old_hands);

		std::vector<int> playerOrder;
		for (const auto& [id, _] : variant()->GetPlayers())
		{
			playerOrder.push_back(id);
		}

		int num_players = playerOrder.size();

		for (int i = 0; i < num_players; ++i)
		{
			int new_index = 0;
			if (direction == Direction::RIGHT)
			{
				new_index = (i + steps) % num_players;
			}
			else
			{
				new_index = (i - steps + num_players) % num_players;
			}

			int currentID = playerOrder[i];
			int newID = playerOrder[new_index];

			variant()->GetPlayers()[newID]->SetHand(std::move(old_hands[currentID]));
		}
	}


	void Boomerang::PrintLatestCards()
	{
		
		for (auto& curr_player : variant()->GetPlayers())
		{
			Message msg;
			msg.status = Status::WAITING;
			msg.message = "\nHere is what people have picked: \n";
			for (auto& player : variant()->GetPlayers())
			{
				auto test = player.second->GetHand();
				if(curr_player.second->getID() != player.second->getID())
				{
					msg.message += "Player " + std::to_string(player.second->getID()) + ": \n" + player.second->DraftToString(true) + "\n";
				}
				else
				{
					msg.message += "Your hand: \n" + player.second->DraftToString(false) + "\n";
				}
				
			}
			NetworkInstance::GetInstance().BroadcastToAClient(curr_player.second->getID(), MessageParser::EncodeMessage(msg));
		}
		
	}

	void Boomerang::HandleScoring(int round)
	{
		ThreadPool<std::tuple<int, int>> pool(variant()->GetPlayers().size());
		for (auto& player : variant()->GetPlayers())
		{
			int player_id = player.second->getID();
			std::vector<Card*>* player_draft = player.second->GetDraft();
			pool.submit_task([this, player_id, player_draft, round]() -> std::tuple<int, int> {
				return this->variant()->CalculateRoundScores(round, player_draft, player_id);
			});
		}

		std::shared_ptr<std::vector<std::tuple<int, int>>> answers = pool.run_tasks();

		for (std::tuple<int, int> answer : *answers)
		{

			GetPlayerById(std::get<0>(answer))->AddRoundScore(std::get<1>(answer));
		}
	}

	void Boomerang::ShowRoundScore(int round)
	{
		std::string msg = "The score for round " + std::to_string(round) + " is:\n";
		for(auto &player: variant()->GetPlayers())
		{
			msg += "Player " + std::to_string(player.second->getID()) + " scored: " + std::to_string(player.second->GetScore(round)) + ", ";
		}
		msg.pop_back();
		msg.pop_back();
		Message message;
		message.status = Status::WAITING;
		message.message = msg;

		NetworkInstance::GetInstance().BroadcastToClients(MessageParser::EncodeMessage(message));
	}

	void Boomerang::ShowEndScores()
	{
		std::string player_header = "";
		std::vector<std::string> round_scores{};
		std::string total_scores = "Total scores: ";

		for(int i = 0; i < variant()->Rounds(); i++)
		{
			round_scores.push_back("Round " + std::to_string(i + 1) + ": ");
		}
		for (auto& player : variant()->GetPlayers())
		{
			player_header += "\t Player " + std::to_string(player.second->getID());

			for(int i = 0; i < round_scores.size(); i++)
			{
				round_scores.at(i) += std::to_string(player.second->GetScore(i + 1)) + "\t";
			}

			total_scores += std::to_string(player.second->GetTotalScore()) + "\t";
		}

		Message msg;

		msg.status = Status::WAITING;
		msg.message = player_header + "\n";

		for(std::string round_score : round_scores)
		{
			msg.message += round_score + "\n";
		}

		msg.message += total_scores;

		NetworkInstance::GetInstance().BroadcastToClients(MessageParser::EncodeMessage(msg));
	}

	int Boomerang::CheckWinner()
	{
		int highest_score = 0;
		int winner_id = 0;

		for(auto &player: variant()->GetPlayers())
		{
			int total_score = player.second->GetTotalScore();
			if(total_score == highest_score)
			{
				winner_id = variant()->BreakTie(GetPlayerById(winner_id)->GetDraft(), winner_id, player.second->GetDraft(), player.second->getID());
			}
			else if(total_score > highest_score)
			{
				highest_score = total_score;
				winner_id = player.second->getID();
			}
		}

		return winner_id;
	}

	

}
