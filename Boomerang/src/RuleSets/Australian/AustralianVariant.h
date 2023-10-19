#pragma once
#include <memory>

#include "Core/GameVariant.h"

namespace Boomerang
{
	class AustralianVariant : public GameVariant
	{
	public:
		AustralianVariant() : GameVariant() {}
		void BuildRules() override;

		void EndRound(int round) override;

		std::tuple<Direction, int>* HandOverHand(int draft_turn) override;
		int BreakTie(std::vector<Card*>* player1_draft, int player1_id, std::vector<Card*>* player2_draft, int player2_id) override;

	private:
		const int ROUNDS = 4;
		const int CARDS_PER_HAND = 7;
	};
}
