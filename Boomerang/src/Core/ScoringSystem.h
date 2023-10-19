#pragma once
#include "Card/Card.h"
#include "ECS/System.h"
#include "Player/Player.h"


namespace Boomerang {
    class ScoringSystem : public System<int, std::vector<Card*>&, int, Entity&, std::map<int, std::unique_ptr<Player>>&> {
    public:
        virtual int UpdateWithArgs(std::vector<Card*>& draft, int player_id, Entity& game_config, std::map<int, std::unique_ptr<Player>>& players) = 0;
    };
}