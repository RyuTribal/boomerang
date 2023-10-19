#pragma once
#include "BotPlayer.h"
#include "PlayablePlayer.h"


namespace Boomerang
{
	enum PlayerTypes
	{
		HUMAN,
		BOT
	};

	class PlayerFactory
	{
	public:
        static std::unique_ptr<Player> CreatePlayerClass(PlayerTypes type) {
            int new_id = m_PlayerIDCounter--;

            switch (type) {
            case PlayerTypes::HUMAN:
                return std::make_unique<PlayablePlayer>(new_id);
            case PlayerTypes::BOT:
                return std::make_unique<BotPlayer>(new_id);
            default:
                return nullptr;
            }
        }
        static std::unique_ptr<Player> CreatePlayerClass(PlayerTypes type, int id) {
            switch (type) {
            case PlayerTypes::HUMAN:
                return std::make_unique<PlayablePlayer>(id);
            case PlayerTypes::BOT:
                return std::make_unique<BotPlayer>(id);
            default:
                return nullptr;
            }
        }

	private:
		static int m_PlayerIDCounter;
	};
}
