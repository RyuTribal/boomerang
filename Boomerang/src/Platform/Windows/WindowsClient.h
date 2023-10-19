#pragma once

#include <string>
#include <winsock2.h>

#include "Net/Client.h"

namespace Boomerang {

    class WindowsClient : public Client {
    public:
        WindowsClient() : m_ClientSocket(INVALID_SOCKET){}
        ~WindowsClient() override { Cleanup(); }

        bool Initialize() override;
        bool Connect(const std::string& address, int port) override;
        int ReceiveData(char* buffer, int length) override;
        int SendData(const std::string& message) override;
        void Cleanup();

    private:
        SOCKET m_ClientSocket;
        WSADATA m_WsaData;
    };

}
