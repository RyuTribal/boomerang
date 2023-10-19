#pragma once
#include <iostream>
#include <vector>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>

#include "Net/Network.h"

#pragma comment(lib, "ws2_32.lib")

namespace Boomerang
{
	struct Windows_Client_Info : public Client_Info
	{
		SOCKET socket;
	};

	class WindowsNetwork : public Network
	{
	public:
		WindowsNetwork() : m_ListenSocket(INVALID_SOCKET) {}
		~WindowsNetwork() override { Cleanup(); }
		bool Initialize(int port) override;
		bool AcceptClient() override;
		void WaitForClients(int amount_of_clients) override;

		bool CreateSocket();
		bool BindSocket(int port);
		bool ListenForClient(int port);

		void BroadcastToClients(const std::string& message) override;
		bool BroadcastToAClient(int client_id, const std::string& message) override;
		std::string AwaitClient(int client_id) override;

		void Cleanup();

	private:
		WSADATA m_WsaData;
		SOCKET m_ListenSocket;
	};
}