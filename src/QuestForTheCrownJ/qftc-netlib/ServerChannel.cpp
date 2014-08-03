#include "ServerChannel.h"

#include <exception>
#include <sstream>

#include "Log.h"
#include "NetDefinitions.h"
#include "AuthStructs.h"
#include "NetHelper.h"

using namespace qfcbase;
using namespace qfcnet;

ServerChannel::ServerChannel(int port)
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

	listen_thread = std::thread(&ServerChannel::Listen, this, port);
}

ServerChannel::~ServerChannel()
{
	stop_listen = true;

	if (channel_socket)
		closesocket(channel_socket);

	if (listen_thread.joinable())
		listen_thread.join();

	WSACleanup();
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
		auto sender = std::make_shared<sockaddr_in>();
		int sender_size = sizeof(sockaddr_in);
		ZeroMemory(sender.get(), sender_size);

		size = recvfrom(channel_socket, buffer, NET_BUFFER_SIZE, 0, (SOCKADDR*)sender.get(), &sender_size);
		if (stop_listen)
			return;

		if (size < 0)
		{
			Log::Error(std::string("Error on receive: ") + std::to_string(WSAGetLastError()));
			std::this_thread::sleep_for(std::chrono::microseconds((long long)((NET_SECONDS_PER_FRAME)* (1000 * 1000))));
			continue;
		}

		ClientHeader* clientHeader = (ClientHeader*)buffer;

		if (onClientMessage)
			onClientMessage(*clientHeader, sender, sender_size);

		switch (clientHeader->header.type)
		{
		case PacketType::LAUNCHER_LOGIN_INFO:
			if (size != sizeof(LauncherLoginInfo))
			{
				Log::Error((std::string)"Invalid packet size for LAUNCHER_LOGIN_INFO: " + std::to_string(size));
				continue;
			}
			if (!handleLoginInfo)
			{
				Log::Debug("No handler for LAUNCHER_LOGIN_INFO");
				continue;
			}
			else {
				auto data = (LauncherLoginInfo*)buffer;
				auto resp = handleLoginInfo(*data, sender, sender_size);
				resp.header.type = PacketType::LAUNCHER_LOGIN_RESPONSE;

				Send(resp, sender, sender_size);
			}
			break;
		case PacketType::CLIENT_REQUEST_PLAYER_INFO:
			if (size != sizeof(ClientRequestPlayerInfo))
			{
				Log::Error("Invalid packet size for CLIENT_REQUEST_PLAYER_INFO: " + size);
				continue;
			}
			if (!handleRequestPlayer)
			{
				Log::Debug("No handler for CLIENT_REQUEST_PLAYER_INFO");
				continue;
			}
			else {
				auto data = (ClientRequestPlayerInfo*)buffer;
				handleRequestPlayer(*data);
			}
			break;
		case PacketType::CLIENT_REQUEST_ENTITIES:
			if (size != sizeof(ClientRequestEntities))
			{
				Log::Error("Invalid packet size for CLIENT_REQUEST_ENTITIES: " + size);
				continue;
			}
			if (!handleRequestEntities)
			{
				Log::Debug("No handler for CLIENT_REQUEST_ENTITIES");
				continue;
			}
			else {
				auto data = (ClientRequestEntities*)buffer;
				handleRequestEntities(*data);
			}
			break;
		case PacketType::CLIENT_SEND_PLAYER_POSITION:
			if (size != sizeof(ClientSendPlayerPosition))
			{
				Log::Error("Invalid packet size for CLIENT_SEND_PLAYER_POSITION: " + size);
				continue;
			}
			if (!handlePlayerPosition)
			{
				Log::Debug("No handler for CLIENT_SEND_PLAYER_POSITION");
				continue;
			}
			else {
				auto data = (ClientSendPlayerPosition*)buffer;
				handlePlayerPosition(*data);
			}
			break;
		default:
			Log::Debug(std::string("Unknown message type: ") + std::to_string(clientHeader->header.type));
			break;

			/*case PacketType::CLIENT_CHARACTER_STATUS:
				auto client_char_status = (s_client_character_status*)buffer;
				break;
				case PacketType::CLIENT_CHARACTER_ITEM:
				auto client_char_item = (s_client_character_item*)buffer;
				break;
				case PacketType::CLIENT_BATTLE_BEGIN:
				auto client_char_battle_begin = (s_client_character_battle_begin*)buffer;
				break;
				case PacketType::CLIENT_BATTLE_NEXT_TURN:
				auto client_character_next_turn = (s_client_character_next_turn*)buffer;
				break;
				case PacketType::CLIENT_BATTLE_COMMAND:
				auto client_character_command = (s_client_character_command*)buffer;
				break;*/
		}
	}
}

void ServerChannel::SendEntity(int map_id, int id, EntityType type, sf::Vector2f pos, std::string animation, std::shared_ptr<sockaddr_in> addr, int addr_size)
{
	ServerSendEntity data;
	data.entity.info.type = type;
	data.entity.info.id = id;
	data.entity.location.map_id = map_id;
	data.entity.location.position = pos;
	data.entity.view.animation = NetHelper::EncodeAnimation(animation);
	Send(data, addr, addr_size);
}
