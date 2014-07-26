#include "ClientChannel.h"

#include <exception>

#include "NetDefinitions.h"
#include "Log.h"
#include "Hero.h"
#include "Slime.h"

using namespace qfcnet;
using namespace qfcbase;

ClientChannel::ClientChannel(std::string serverAddress, int port)
	: stop_listen(false), channel_socket(0)
{
	WSADATA wsaData;
	int r = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2)
		throw std::exception("Invalid WinSock version.");

	channel_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (channel_socket == INVALID_SOCKET)
		throw std::exception(("Error while creating channel_socket: " + std::to_string(WSAGetLastError())).c_str());

	ZeroMemory(&server_addr, sizeof(server_addr));
	server_addr.sin_addr.s_addr = inet_addr(serverAddress.c_str());
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr_size = sizeof(server_addr);

	listen_thread = std::thread(&ClientChannel::Listen, this);
}


ClientChannel::~ClientChannel()
{
	stop_listen = true;

	if (channel_socket)
		closesocket(channel_socket);

	if (listen_thread.joinable())
		listen_thread.join();
	WSACleanup();
}

void qfcnet::ClientChannel::Listen()
{
	char buffer[NET_BUFFER_SIZE];
	int size;
	while (!stop_listen)
	{
		size = recvfrom(channel_socket, (char*)&buffer, NET_BUFFER_SIZE - 1, 0, (SOCKADDR*)&server_addr, &server_addr_size);
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
		case PacketType::LAUNCHER_LOGIN_RESPONSE:
			if (!get_login_response)
			{
				Log::Error("Unexpected LAUNCHER_LOGIN_RESPONSE");
				continue;
			}
			get_login_response->set_value(*(s_launcher_login_response*)(&buffer));
			break;
		case PacketType::SERVER_ENTITY_INFO:
			if (size != sizeof(ServerEntityInfo))
			{
				Log::Error("Invalid packet size for SERVER_ENTITY_INFO: " + size);
				continue;
			}
			if (!onEntity)
			{
				Log::Debug("No handler for SERVER_ENTITY_INFO");
				continue;
			}
			else {
				auto info = (ServerEntityInfo*)&buffer;
				onEntity(*info);
			}
			break;
		default:
			Log::Debug("Unknown message type: " + header->type);
			break;
		}
	}
}

#pragma region Requests
void ClientChannel::Login(std::string user, std::string password)
{
	s_launcher_login_info login;
	login.header.type = PacketType::LAUNCHER_LOGIN_INFO;
	strcpy_s(login.login, sizeof(login.login), user.c_str());
	strcpy_s(login.hashedPassword, sizeof(login.hashedPassword), password.c_str());

	get_login_response = std::make_shared<std::promise<s_launcher_login_response>>();

	int r = sendto(channel_socket, (char*)&login, sizeof(login), 0, (SOCKADDR*)&server_addr, server_addr_size);
	if (r <= 0)
		throw std::exception(("Send error: " + std::to_string(WSAGetLastError())).c_str());

	auto response = get_login_response->get_future().get();
	get_login_response = nullptr;

	if (!response.authenticated)
		throw std::exception("Invalid username or password");

	auth_token = response.authKey;
}

ServerPlayerInfo ClientChannel::GetPlayerInfo()
{
	return ServerPlayerInfo();
}

void ClientChannel::GetEntities(std::string screen_name)
{
	
}
#pragma endregion Requests
