#include "WindowsClient.h"

#include <ws2tcpip.h>

#include "Core.h"
#include "Util/Log.h"

namespace Boomerang
{
	bool WindowsClient::Initialize()
	{
		if (WSAStartup(MAKEWORD(2, 2), &m_WsaData) != 0) {
			CLIENT_ERROR("WSAStartup failed.");
			return false;
		}
		return true;
	}
	bool WindowsClient::Connect(const std::string& address, int port)
	{
		m_ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (m_ClientSocket == INVALID_SOCKET) {
			CLIENT_ERROR("Socket creation failed: {0}", WSAGetLastError());
			WSACleanup();
			return false;
		}

		sockaddr_in serverAddress;
		serverAddress.sin_family = AF_INET;
		std::wstring wideAddress = std::wstring(address.begin(), address.end());

		if (InetPton(AF_INET, wideAddress.c_str(), &serverAddress.sin_addr) <= 0) {
			CLIENT_ERROR("Connection failed!");
			return false;
		}
		serverAddress.sin_port = htons(port);

		if (connect(m_ClientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
			CLIENT_ERROR("Failed to connect: {0}", WSAGetLastError());
			closesocket(m_ClientSocket);
			WSACleanup();
			return false;
		}

		CLIENT_INFO("Connected to: {0}:{1}", address, port);
		return true;
	}
	int WindowsClient::ReceiveData(char* buffer, int length)
	{
		return recv(m_ClientSocket, buffer, length, 0);
	}
	int WindowsClient::SendData(const std::string& message)
	{
		char buffer[BUFFER_SIZE];
		strncpy(buffer, message.c_str(), BUFFER_SIZE - 1);
		buffer[BUFFER_SIZE - 1] = '\0';
		return send(m_ClientSocket, buffer, sizeof(buffer), 0);
	}
	void WindowsClient::Cleanup()
	{
		closesocket(m_ClientSocket);
		WSACleanup();
	}
}
