#pragma once
#include <memory>

#include "Core/Deck.h"
#include "Core/GameVariant.h"

namespace Boomerang
{
	class Boomerang
	{
	public:
		Boomerang(Deck* deck, GameVariant* rules);
		void CopyPlayerDecks(std::map<int, std::unique_ptr<std::vector<Card*>>>& copy_map);

		Player* GetPlayerById(int id);

		void run();
		void GoThroughCards();
		void DealCards();
		void PlayersSelect();
		void PassHands(int card_selection_count);
		void HandleScoring(int round);
		void ShowRoundScore(int round);
		void ShowEndScores();
		int CheckWinner();
		void PrintLatestCards();

		GameVariant* variant() { return m_GameVariant; }
		Deck* deck() { return m_Deck; }
	private:
		GameVariant* m_GameVariant;
		Deck* m_Deck;
	};
}
