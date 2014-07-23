#include "ClientChannel.h"
using namespace qfcnet;

#include <memory>
#include <exception>
#include "Hero.h"
#include "Slime.h"
#include "AuthStructs.h"
#include "Log.h"

#define BUFFER_SIZE 2000

ClientChannel::ClientChannel(std::string serverIPAddress, int port)
	: server_socket(0)
{
	WSADATA wsaData;
	int r = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2)
		throw std::exception("Não foi possível encontrar uma dll WinSock que possa ser utilizada.");

	server_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	ZeroMemory(&server_addr, sizeof(server_addr));

	server_addr.sin_addr.s_addr = inet_addr(serverIPAddress.c_str());
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr_size = sizeof(server_addr);

	listen_thread = std::thread([this](){
		Listen(cancellation_source.get_token());
	});
}

ClientChannel::~ClientChannel()
{
	cancellation_source.cancel();

	if (server_socket)
		closesocket(server_socket);

	WSACleanup();
}

void qfcnet::ClientChannel::Listen(concurrency::cancellation_token cancellation)
{
	cancellation.register_callback([this]() {
		closesocket(server_socket);
	});

	char buffer[BUFFER_SIZE];
	int r;
	while (!cancellation.is_canceled())
	{
		r = recvfrom(server_socket, (char*)&buffer, sizeof(buffer), 0, (SOCKADDR*)&server_addr, &server_addr_size);
		if (cancellation.is_canceled())
			return;

		Header* header = (Header*)&buffer;
		switch (header->type)
		{
		case PacketType::CLIENT_CHARACTER_POSITION:
			if (onEntity)
			{

			}
			break;
		default:
			qfcbase::Log::Debug("Unknown message type: " + header->type);
			break;
		}
	}
}

#pragma region Requests
void ClientChannel::Login(std::string user, std::string password)
{
	s_launcher_login_info login;
	login.header.type = PacketType::LAUNCHER_LOGIN_INFO;
	strcpy(login.login, user.c_str());
	strcpy(login.hashedPassword, password.c_str());

	int r = sendto(server_socket, (char*)&login, sizeof(login), 0, (SOCKADDR*)&server_addr, server_addr_size);
	if (r <= 0)
		throw std::exception("Send error");

	s_launcher_login_response response;
	ZeroMemory(&response, sizeof(response));

	r = recvfrom(server_socket, (char*)&response, sizeof(response), 0, (SOCKADDR*)&server_addr, &server_addr_size);

	if (r <= 0)
		throw std::exception("Receive error");

	if (r != sizeof(response))
		throw std::exception("Invalid response");

	if (!response.authenticated)
		throw std::exception("Invalid username or password");

	auth_token = response.authKey;
}

PlayerInfo ClientChannel::GetPlayerInfo()
{
	return PlayerInfo();
}

void ClientChannel::GetEntities(std::string screen_name)
{
	
}
#pragma endregion Requests
