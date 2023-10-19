#pragma once
#include <string>


#include "Util/MessageParser.h"


namespace Boomerang {

    class Client {
    public:
        virtual ~Client() = default;

        virtual bool Initialize() = 0;
        virtual bool Connect(const std::string& address, int port) = 0;
        virtual int ReceiveData(char* buffer, int length) = 0;
        virtual int SendData(const std::string& message) = 0;

        Message WaitForServer();
        void HandleInput();
    };

}
