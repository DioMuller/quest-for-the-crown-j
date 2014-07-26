#include "ServerChannel.h"

#include <exception>
#include <sstream>

#include "Log.h"
#include "NetDefinitions.h"
#include "AuthStructs.h"
#include "TaskHelper.h"

using namespace qfcbase;
using namespace qfcnet;

ServerChannel::ServerChannel()
	: stop_listen(false)
{
	WSADATA wsaData;
	int r = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2)
		throw std::exception("Invalid WinSock version.");

	channel_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (channel_socket == INVALID_SOCKET)
		throw std::exception(("Error while creating channel_socket: " + std::to_string(WSAGetLastError())).c_str());
}

ServerChannel::~ServerChannel()
{
	stop_listen = true;

	if (channel_socket)
		closesocket(channel_socket);

	WSACleanup();
}

std::future<void> ServerChannel::ListenAsync(int port)
{
	std::promise<void> listen;
	on_promise(true, listen, [&] { Listen(port); });
	return listen.get_future();
}

void ServerChannel::Listen(int port)
{
	sockaddr_in serverAddress;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);

	if (bind(channel_socket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
	{
		std::stringstream errorBuilder;
		errorBuilder << "Failed to bind socket to port " << port << ". " << WSAGetLastError();
		throw std::exception(errorBuilder.str().c_str());
	}

	char buffer[NET_BUFFER_SIZE];
	int size;

	while (!stop_listen)
	{
		sockaddr_in sender;
		int sender_size = sizeof(sender);

		size = recvfrom(channel_socket, buffer, NET_BUFFER_SIZE - 1, 0, (SOCKADDR*)&sender, &sender_size);
		if (stop_listen)
			return;

		if (size < 0)
		{
			Log::Error("Error on receive");
			continue;
		}

		buffer[size] = '\0';

		Header* header = (Header*)&buffer;
		switch (header->type)
		{
		case PacketType::LAUNCHER_LOGIN_INFO:
			if (size != sizeof(LauncherLoginInfo))
			{
				Log::Error("Invalid packet size for LAUNCHER_LOGIN_INFO: " + size);
				continue;
			}
			if (!handleLoginInfo)
			{
				Log::Debug("No handler for LAUNCHER_LOGIN_INFO");
				continue;
			}
			else {
				auto login_info = (LauncherLoginInfo*)&buffer;
				auto resp = handleLoginInfo(*login_info);
				resp.header.type = PacketType::LAUNCHER_LOGIN_RESPONSE;

				size = sendto(channel_socket, (char*)&resp, sizeof(resp), 0, (SOCKADDR*)&sender, sender_size);
				if (size != sizeof(resp))
				{
					Log::Error("Error while sending LAUNCHER_LOGIN_INFO response.");
					continue;
				}
			}
			break;
		default:
			Log::Debug("Unknown message type: " + header->type);
			break;
		/*case PacketType::CLIENT_CHARACTER_POSITION:
			auto client_char_pos = (s_client_position*)&buffer;
			break;
		case PacketType::CLIENT_CHARACTER_STATUS:
			auto client_char_status = (s_client_character_status*)&buffer;
			break;
		case PacketType::CLIENT_CHARACTER_ITEM:
			auto client_char_item = (s_client_character_item*)&buffer;
			break;
		case PacketType::CLIENT_BATTLE_BEGIN:
			auto client_char_battle_begin = (s_client_character_battle_begin*)&buffer;
			break;
		case PacketType::CLIENT_BATTLE_NEXT_TURN:
			auto client_character_next_turn = (s_client_character_next_turn*)&buffer;
			break;
		case PacketType::CLIENT_BATTLE_COMMAND:
			auto client_character_command = (s_client_character_command*)&buffer;
			break;*/
		}
	}
}

