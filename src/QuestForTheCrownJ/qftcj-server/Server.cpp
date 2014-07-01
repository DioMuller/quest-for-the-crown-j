#include "Server.h"
#include "Log.h"
#include "Exceptions.h"
#include "StructBase.h"
#include <string>
#include <sstream>

#include "AuthStructs.h"
#include "ClientStructs.h"

using namespace qfcserver;
using namespace qfcbase;

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
			Log::Error("Falha no receive");
			continue;
		}
		buffer[r] = '\0';
		auto header = (s_header*)&buffer;
		switch (header->type)
		{
			case LAUNCHER_LOGIN_INFO:
				auto login_info = (s_launcher_login_info*)&buffer;
				Log::Message("Login: " + login_info->login);
				break;
			/*case CLIENT_CHARACTER_POSITION:
				auto client_char_pos = (s_client_position*)&buffer;
				break;
			case CLIENT_CHARACTER_STATUS:
				auto client_char_status = (s_client_character_status*)&buffer;
				break;
			case CLIENT_CHARACTER_ITEM:
				auto client_char_item = (s_client_character_item*)&buffer;
				break;
			case CLIENT_BATTLE_BEGIN:
				auto client_char_battle_begin = (s_client_character_battle_begin*)&buffer;
				break;
			case CLIENT_BATTLE_NEXT_TURN:
				auto client_character_next_turn = (s_client_character_next_turn*)&buffer;
				break;
			case CLIENT_BATTLE_COMMAND:
				auto client_character_command = (s_client_character_command*)&buffer;
				break;*/
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