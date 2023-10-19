#include "PlayablePlayer.h"

#include <optional>

#include "Util/Log.h"
#include "Platform/NetInstance.h"

namespace Boomerang
{

	std::tuple<int, int> PlayablePlayer::ChooseCard(std::vector<Card*>& cards, bool automatic_choice)
	{
		std::optional<int> answer;

		if(GetHand()->size() == 1 && automatic_choice)
		{
			SendMessageToClient("Chose this card automatically: \n" + GetHand()->at(0)->ToString(), Status::WAITING);
			return { getID(), GetHand()->at(0)->id()};
		}
		
		SendMessageToClient("Here is your hand: \n" + GetHandString(), Status::CLIENTS_TURN);

		while (!answer) {
			Message response = MessageParser::DecodeMessage(NetworkInstance::GetInstance().AwaitClient(getID()));
			try
			{
				int chosen_id = std::stoi(response.message);
				if (ExistsInHand(chosen_id, cards)) {
					answer = std::stoi(response.message);
				}
				else
				{
					HandleInvalidAnswers(*"Card id not in hand");
				}
			}
			catch (const std::invalid_argument& ia) {
				HandleInvalidAnswers(*ia.what());
			}
			catch (const std::out_of_range& oor) {
				HandleInvalidAnswers(*oor.what());
			}
		}

		return {getID(), answer.value() };

	}
	void PlayablePlayer::SendMessageToClient(std::string message, Status status)
	{
		Message msg;
		msg.status = status;
		msg.message = message;

		std::string buffer = MessageParser::EncodeMessage(msg);

		NetworkInstance::GetInstance().BroadcastToAClient(getID(), buffer);
	}
	void PlayablePlayer::HandleInvalidAnswers(char const& err)
	{
		CLIENT_ERROR("Invalid argument: {0}", err);
		SendMessageToClient("Invalid answer: Please pick again!", Status::CLIENTS_TURN);
	}
}

