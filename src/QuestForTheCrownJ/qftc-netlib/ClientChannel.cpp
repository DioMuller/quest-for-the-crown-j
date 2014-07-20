#include "ClientChannel.h"
using namespace qfcnet;
#include <exception>

ClientChannel::ClientChannel(std::string serverAddress, int port)
	: server_socket(0)
{
	WSADATA wsaData;
	int r = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2)
		throw std::exception("Não foi possível encontrar uma dll WinSock que possa ser utilizada.");

	server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

ClientChannel::~ClientChannel()
{
	if (server_socket)
		closesocket(server_socket);
	WSACleanup();
}

std::string ClientChannel::Login(std::string user, std::string password)
{
	return "stub";
}

PlayerInfo ClientChannel::GetPlayerInfo(std::string auth_token)
{
	return PlayerInfo();
}

std::vector<EntityInfo> ClientChannel::GetEntities(std::string screen_name)
{
	return std::vector<EntityInfo>();
}
