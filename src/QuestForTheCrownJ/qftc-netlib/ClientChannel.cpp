#include "ClientChannel.h"
using namespace qfcnet;

#include <memory>
#include <exception>
#include "Hero.h"
#include "Slime.h"
#include "AuthStructs.h"

ClientChannel::ClientChannel(std::string serverIPAddress, int port)
	: server_socket(0)
{
	WSADATA wsaData;
	int r = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2)
		throw std::exception("Não foi possível encontrar uma dll WinSock que possa ser utilizada.");

	server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	ZeroMemory(&server_addr, sizeof(server_addr));

	server_addr.sin_addr.s_addr = inet_addr(serverIPAddress.c_str());
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr_size = sizeof(server_addr);
}

ClientChannel::~ClientChannel()
{
	if (server_socket)
		closesocket(server_socket);
	WSACleanup();
}

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

	r = recv(server_socket, (char*)&response, sizeof(response), 0);

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

std::vector<EntityInfo> ClientChannel::GetEntities(std::string screen_name)
{
	return std::vector<EntityInfo>();
}