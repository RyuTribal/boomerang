#include "WindowsNetwork.h"

#include "Core.h"
#include "Util/Log.h"
#include "Util/ThreadPool.h"

namespace Boomerang
{

	int Client_Info::s_NextID = 1;

	bool WindowsNetwork::Initialize(int port)
	{
		int result = WSAStartup(MAKEWORD(2,2), &m_WsaData);

		if (result != 0) {
			SERVER_ERROR("WSAStartup failed: {0}", result);
			return false;
		}
		return CreateSocket() && BindSocket(port) && ListenForClient(port);
	}
	bool WindowsNetwork::CreateSocket()
	{
		m_ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (m_ListenSocket == INVALID_SOCKET) {
			SERVER_ERROR("Socket creation failed: {0}", WSAGetLastError());
			WSACleanup();
			return false;
		}
		return true;
	}
	bool WindowsNetwork::AcceptClient()
	{
		Windows_Client_Info newClient;
		newClient.socket = accept(m_ListenSocket, NULL, NULL);
		if (newClient.socket == INVALID_SOCKET) {
			SERVER_ERROR("Accept failed: {0}", WSAGetLastError());
			return false;
		}
		m_Clients[newClient.id] =  std::make_unique<Windows_Client_Info>(newClient);
		SERVER_INFO("Client connected!");
		return true;
	}
	void WindowsNetwork::WaitForClients(int amount_of_clients)
	{
		SERVER_TRACE("Waiting for players...");
		int clients_in_lobby = 0;
		while (clients_in_lobby < amount_of_clients)
		{
			if (AcceptClient())
			{
				clients_in_lobby++;
				if (clients_in_lobby < amount_of_clients) {
					std::string message = "Waiting for more players [" + std::to_string(clients_in_lobby) + "/" + std::to_string(amount_of_clients) + "]";
					SERVER_TRACE(message);
					BroadcastToClients(message);
				}
				else
				{
					SERVER_INFO("All players slots filled!");
				}
			}
		}
	}

	bool WindowsNetwork::BindSocket(int port)
	{
		sockaddr_in serverAddress;
		serverAddress.sin_family = AF_INET;
		serverAddress.sin_addr.s_addr = INADDR_ANY;
		serverAddress.sin_port = htons(port);

		if (bind(m_ListenSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
			SERVER_ERROR("Bind failed: {0}", WSAGetLastError());
			closesocket(m_ListenSocket);
			WSACleanup();
			return false;
		}
		return true;
	}
	bool WindowsNetwork::ListenForClient(int port)
	{
		if (listen(m_ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
			SERVER_ERROR("Listen failed: {0}", WSAGetLastError());
			closesocket(m_ListenSocket);
			WSACleanup();
			return false;
		}
		
		SERVER_INFO("Server started at port {0}", port);
		return true;
	}
	void WindowsNetwork::BroadcastToClients(const std::string& message)
	{
		ThreadPool<> thread_pool(m_Clients.size());
		for (auto i = m_Clients.begin(); i != m_Clients.end(); ++i) {
			int client_id = i->first;
			thread_pool.submit_task([this, client_id, message]() -> void {
				this->BroadcastToAClient(client_id, message);
			});
		}
		thread_pool.run_tasks();
	}

	bool WindowsNetwork::BroadcastToAClient(int client_id, const std::string& message)
	{
		auto it = m_Clients.find(client_id);
		if (it != m_Clients.end()) {
			Windows_Client_Info* client = static_cast<Windows_Client_Info*>(it->second.get());
			if (client == nullptr)
			{
				return false;
			}
			char buffer[BUFFER_SIZE];
			strncpy(buffer, message.c_str(), BUFFER_SIZE - 1);
			buffer[BUFFER_SIZE - 1] = '\0';
			send(client->socket, buffer, sizeof(buffer), 0);

			return true;
		}

		return false;
	}

	std::string WindowsNetwork::AwaitClient(int client_id) {
		char buffer[BUFFER_SIZE];
		//TODO: add timeout functionality so the server doesn't become stuck
		while (true) {
			auto it = m_Clients.find(client_id);
			if (it != m_Clients.end()) {
				Windows_Client_Info* client = static_cast<Windows_Client_Info*>(it->second.get());
				int bytesReceived = recv(client->socket, buffer, sizeof(buffer), 0);
				if (bytesReceived > 0) {
					buffer[bytesReceived - 1] = '\0';
					return buffer;
				}
			}
			else {
				return "";
			}
			
		}
	}

	void WindowsNetwork::Cleanup()
	{
		for (auto i = m_Clients.begin(); i != m_Clients.end(); i++) {
			Windows_Client_Info* client = static_cast<Windows_Client_Info*>(i->second.get());
			closesocket(client->socket);
		}
		closesocket(m_ListenSocket);
		WSACleanup();
	}
}
