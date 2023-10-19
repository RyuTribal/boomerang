#include <iostream>

#include "EntryPoint.h"
#include "Util/Log.h"




int main()
{
    try
    {
        Boomerang::Log::Init();

        Boomerang::NetworkMode mode = Boomerang::ChooseNetworkMode();
        switch (mode) {
        case Boomerang::NetworkMode::Server:
            Boomerang::StartServer();
            break;
        case Boomerang::NetworkMode::Client:
            Boomerang::StartClient();
            break;
        default:
            GAME_ERROR("Invalid choice!");
            return 1;
        }
    }
    catch (const std::exception& e)
    {
        Boomerang::ErrorType err = ERROR_TYPE(e.what());
        switch (err)
        {
        case Boomerang::SERVER:
            SERVER_FATAL(e.what());
            break;
        case Boomerang::CLIENT:
            CLIENT_FATAL(e.what());
            break;
        default:
            GAME_FATAL(e.what());
        }
        return 1;
    }
    catch (...)
    {
        GAME_FATAL("Caught unknown exception!");
        return 1;
    }

    return 0;
}

// this code is halal certified