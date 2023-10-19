#pragma once
#include <functional>
#include <memory>
#include <string>

namespace Boomerang
{

    struct Client_Info
    {
        Client_Info()
            : id(s_NextID++)
        {}
        static void Init() { s_NextID = 1; }
        int id;

        static int s_NextID;
        
    };

    class Network {
    public:
        virtual ~Network() = default;
        virtual bool Initialize(int port) = 0;
        virtual bool AcceptClient() = 0;
        virtual void WaitForClients(int amount_of_clients) = 0;
        virtual void BroadcastToClients(const std::string& message) = 0;
        virtual bool BroadcastToAClient(int client_id, const std::string& message) = 0;
        virtual std::string AwaitClient(int client_id) = 0;

        std::unordered_map<int, std::unique_ptr<Client_Info>>* GetClients() { return &m_Clients; }

        std::shared_ptr<std::vector<std::string>> AwaitMessageForClients();

    protected:
        std::unordered_map<int, std::unique_ptr<Client_Info>> m_Clients;
    };
}
