#pragma once
#include "Core/Player/Player.h"
#include "Util/MessageParser.h"

namespace Boomerang
{
	class PlayablePlayer : public Player
	{
	public:
		PlayablePlayer(int id) : Player(id) {}
		std::tuple<int, int> ChooseCard(std::vector<Card*>& cards, bool automatic_choice) override;

		void SendMessageToClient(std::string message, Status status);
		void HandleInvalidAnswers(char const& err);

	};
}
