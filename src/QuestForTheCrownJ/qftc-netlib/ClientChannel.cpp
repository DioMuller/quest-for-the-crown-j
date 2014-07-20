#include "ClientChannel.h"
using namespace qfcnet;
#include <WinSock2.h>
#include <exception>

ClientChannel::ClientChannel(std::string serverAddress, int port)
	: server_socket(0)
{
	WSADATA wsaData;
	int r = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2)
		throw std::exception("N�o foi poss�vel encontrar uma dll WinSock que possa ser utilizada.");

	server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

ClientChannel::~ClientChannel()
{
	if (server_socket)
		closesocket(server_socket);
	WSACleanup();
}

PlayerInfo ClientChannel::Login(std::string user, std::string password)
{

}

std::vector<EntityInfo> ClientChannel::GetEntities(std::string screen_name)
{

}
