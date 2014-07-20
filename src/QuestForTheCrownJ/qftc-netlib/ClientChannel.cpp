#include "ClientChannel.h"
using namespace qfcnet;

#include <exception>
#include "Hero.h"
#include "Slime.h"

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

void ClientChannel::Login(std::string user, std::string password)
{
	// TODO: Login
}

PlayerInfo ClientChannel::GetPlayerInfo()
{
	return PlayerInfo();
}

std::vector<std::shared_ptr<qfcbase::Entity>> ClientChannel::GetEntities(std::string screen_name)
{
	return std::vector<std::shared_ptr<qfcbase::Entity>>();
}

std::shared_ptr<qfcbase::Entity> ClientChannel::CreateEntity(EntityInfo info)
{
	std::shared_ptr<qfcbase::Entity> entity;

	switch (info.type)
	{
	case EntityType::ENTITY_HERO:
		entity = std::make_shared<qfcbase::Hero>();
		break;
	case EntityType::ENTITY_SLIME:
		entity = std::make_shared<qfcbase::Slime>();
		break;
	}

	if (entity)
		entity->Sprite->Position = sf::Vector2f(info.x, info.y);

	return entity;
}