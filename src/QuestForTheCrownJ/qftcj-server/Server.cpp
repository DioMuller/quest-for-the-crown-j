#include "Server.h"
#include "Exceptions.h"
#include <string>
#include <sstream>
using namespace qfcserver;

#define BUFFER_SIZE 2000

Server::Server()
{
	WSADATA wsaData;
	int r = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
		throw SockError("WinSock 2.2 could not be initialized");
}


Server::~Server()
{
	WSACleanup();
}


void Server::Run(int port)
{
	char buffer[BUFFER_SIZE];
	SOCKET listenSocket = Start(port);

	sockaddr_in sender;
	int sender_size = sizeof(sender);
	int r;

	while (true)
	{
		r = recvfrom(listenSocket, buffer, BUFFER_SIZE, NULL, (SOCKADDR*)&sender, &sender_size);
		if (r < 0)
		{
			//Log::error("Falha no receive");
			continue;
		}
	}
}

SOCKET Server::Start(int port)
{
	SOCKET listenSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (listenSocket == INVALID_SOCKET) {
		std::stringstream errorBuilder;
		errorBuilder << "Failed to set soocket on listen. " << WSAGetLastError();
		throw SockError(errorBuilder.str());
	}

	sockaddr_in serverAddress;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);

	if (bind(listenSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
	{
		std::stringstream errorBuilder;
		errorBuilder << "Failed to bind socket to port " << port << ". " << WSAGetLastError();
		throw SockBindError(errorBuilder.str());
	}

	return listenSocket;
}
