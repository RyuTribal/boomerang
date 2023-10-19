#include "Deck.h"

#include <chrono>
#include <random>

namespace Boomerang
{
	void Deck::AddToDeck(std::unique_ptr<Card> card)
	{
		m_Deck.push_back(std::move(card));
		untakenCardsQueue.push(m_Deck.size() - 1);
		next_card_id++;
	}

	Card* Deck::TakeCard()
	{
		if (untakenCardsQueue.empty()) {
			return nullptr;
		}

		int index = untakenCardsQueue.front();
		untakenCardsQueue.pop();
		return m_Deck.at(index).get();
	}

	Card* Deck::GetCardById(int id)
	{
		for(auto &card: m_Deck)
		{
			if(card->id() == id)
			{
				return card.get();
			}
		}

		return nullptr;
	}

	void Deck::f_ResetDeck()
	{
		std::queue<int>().swap(untakenCardsQueue);
		for (size_t i = 0; i < m_Deck.size(); ++i) {
			untakenCardsQueue.push(i);
		}
	}

	void Deck::ShuffleDeck()
	{
		const long long seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::default_random_engine engine((unsigned int) seed);

		std::shuffle(m_Deck.begin(), m_Deck.end(), engine);

		f_ResetDeck();
	}
}
