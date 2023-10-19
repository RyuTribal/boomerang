#pragma once
#include <memory>
#include <queue>
#include <vector>
#include "Card/Card.h"

namespace Boomerang
{
	template <typename T>
	struct TypeValuePair {
		using Type = T;
		T value;

		TypeValuePair(T v) : value(v) {}
	};

	template <typename T>
	TypeValuePair<T> MakePair(T value) {
		return TypeValuePair<T>(value);
	}

	class Deck
	{
	public:
		void AddToDeck(std::unique_ptr<Card> card);

		Card* TakeCard();

		Card* GetCardById(int id);

		void ShuffleDeck();

		virtual void BuildDeck() = 0;

		size_t size() { return m_Deck.size(); }

		static inline int next_card_id = 0;

		template<typename... Args>
		void GenerateCard(int id, Args... args) {
			auto card = std::make_unique<Card>(id);
			(card->AddComponent<typename Args::Type>(args.value), ...);
			m_Deck.push_back(std::move(card));
		}

	private:
		std::vector<std::unique_ptr<Card>> m_Deck;
		std::queue<int> untakenCardsQueue;

		void f_ResetDeck();
	};
}
