/*
 * Put definitions into this file as well as functions for picking platform specific stuff
 */

#pragma once
#include <iostream>
#include <string>

#include "Boomerang.h"
#include "Core.h"
#include "RuleSets/Australian/AustralianDeck.h"
#include "RuleSets/Australian/AustralianVariant.h"
#include "Util/Log.h"
#include "Platform/NetInstance.h"
#include "PlayerTypes/PlayerFactory.h"
#include "RuleSets/VariantFactory.h"

namespace Boomerang {

    enum class NetworkMode {
        Server,
        Client,
        Invalid
    };

    NetworkMode ChooseNetworkMode() {

        int response = MESSAGE_TO_INT(MESSAGE_AND_WAIT("Do you want to start as:\n[1] Server\n[2] Client"));
        if(response == NULL)
        {
            return NetworkMode::Invalid;
        }

        if (response == 1) {
            return NetworkMode::Server;
        }
        else if (response == 2) {
            return NetworkMode::Client;
        }
        else {
            return NetworkMode::Invalid;
        }
    }

    void StartServer() {

        int port = MESSAGE_TO_INT(MESSAGE_AND_WAIT_WITH_DEFAULT("Which port would you like to open? (default: 8080)", "8080"));

        if(!NetworkInstance::GetInstance().Initialize(port))
        {
            return;
        }
        GameVariantFactory::RegisterAllGameVariants();
        std::string variant_question = "Please select a game variant: \n";
        int index = 1;
        for (const auto& pair : GameVariantFactory::GetAllVariants()) {
            variant_question += std::to_string(index++) + ". " + pair.first + "\n";
        }

        int choice = MESSAGE_TO_INT(MESSAGE_AND_WAIT(variant_question));

        if (choice < 1 || choice > index - 1) {
            SERVER_ERROR("Invalid choice!");
            return;
        }

        auto it = GameVariantFactory::GetAllVariants().begin();
        std::advance(it, choice - 1);
        auto [game_variant, deck] = GameVariantFactory::CreateVariantAndDeck(it->first);

        int players = MESSAGE_TO_INT(MESSAGE_AND_WAIT_WITH_DEFAULT("How many real players should this lobby hold? (default: 2)", "2"));
        int bots = MESSAGE_TO_INT(MESSAGE_AND_WAIT_WITH_DEFAULT("How many bots should this lobby hold? (default: 2)", "2"));
       
        NetworkInstance::GetInstance().WaitForClients(players);
        auto size = NetworkInstance::GetInstance().GetClients()->size();
        for(auto i = NetworkInstance::GetInstance().GetClients()->begin(); i != NetworkInstance::GetInstance().GetClients()->end(); ++i)
        {
            game_variant->AddPlayer(PlayerTypes::HUMAN, i->second->id);
        }

        for (int i = 0; i < bots; i++)
        {
            game_variant->AddPlayer(PlayerTypes::BOT);
        }

        game_variant->BuildRules();

        Boomerang game = Boomerang(deck.get(), game_variant.get());

        game.run();
    }

    void StartClient() {

        if(!ClientInstance::GetInstance().Initialize())
        {
            return;
        }
        std::string address = MESSAGE_AND_WAIT_WITH_DEFAULT("What host would you like to connect to? (default: 127.0.0.1)", "127.0.0.1");

        int port = MESSAGE_TO_INT(MESSAGE_AND_WAIT_WITH_DEFAULT("What port does the host keep open? (default: 8080)", "8080"));

        if(!ClientInstance::GetInstance().Connect(address, port))
        {
            return;
        }


        while(true)
        {
			Message message = ClientInstance::GetInstance().WaitForServer();

            SERVER_TRACE(message.message);

            if(message.status == Status::CLIENTS_TURN)
            {
                ClientInstance::GetInstance().HandleInput();
            }
            else if(message.status == Status::END)
            {
                CLIENT_INFO("Game ended!");
                break;
            }
        }

        while(true)
        {
	        // This is just so it doesnt jump straight out of the program
        }
		
        
    }

}

