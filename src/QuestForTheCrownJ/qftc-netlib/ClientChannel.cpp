#include "ClientChannel.h"

#include <exception>
#include <sstream>

#include "NetDefinitions.h"
#include "Log.h"
#include "Hero.h"
#include "Slime.h"
#include "ClientStructs.h"
#include "NetHelper.h"

using namespace qfcnet;
using namespace qfcbase;

ClientChannel::ClientChannel()
	: stop_listen(false), channel_socket(0)
{

}


ClientChannel::~ClientChannel()
{
	stop_listen = true;

	ClientSendDisconnect send;
	strcpy_s(send.header.authKey, sizeof(send.header.authKey), auth_token.c_str());
	Send(send);

	if (channel_socket)
		closesocket(channel_socket);

	if (listen_thread.joinable())
		listen_thread.join();

}

void qfcnet::ClientChannel::StartConnection(int localPort, std::string &serverAddress, int serverPort)
{
	WSADATA wsaData;
	int r = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2)
		throw std::exception("Invalid WinSock version.");
	channel_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (channel_socket == INVALID_SOCKET)
		throw std::exception(("Error while creating channel_socket: " + std::to_string(WSAGetLastError())).c_str());

	sockaddr_in listenAddress;
	listenAddress.sin_addr.s_addr = INADDR_ANY;
	listenAddress.sin_family = AF_INET;
	listenAddress.sin_port = htons(localPort);

	if (bind(channel_socket, (SOCKADDR*)&listenAddress, sizeof(listenAddress)) == SOCKET_ERROR)
	{
		std::stringstream errorBuilder;
		errorBuilder << "Failed to bind socket to port " << localPort << ". " << WSAGetLastError();
		throw std::exception(errorBuilder.str().c_str());
	}

	ZeroMemory(&server_addr, sizeof(server_addr));
	server_addr.sin_addr.s_addr = inet_addr(serverAddress.c_str());
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(serverPort);
	server_addr_size = sizeof(server_addr);
}

void qfcnet::ClientChannel::Listen()
{
	char buffer[NET_BUFFER_SIZE];
	int size;

	while (!stop_listen)
	{
		size = recvfrom(channel_socket, (char*)buffer, NET_BUFFER_SIZE - 1, 0, (SOCKADDR*)&server_addr, &server_addr_size);
		if (stop_listen)
			break;

		if (size < 0)
		{
			Log::Error(std::string("Error on receive: ") + std::to_string(WSAGetLastError()));
			std::this_thread::sleep_for(std::chrono::microseconds((long long)((NET_SECONDS_PER_FRAME) * (1000 * 1000))));
			continue;
		}

		buffer[size] = '\0';

		Header* header = (Header*)buffer;
		switch (header->type)
		{
		case PacketType::LAUNCHER_LOGIN_RESPONSE:
			HandlePacket("LAUNCHER_LOGIN_RESPONSE", on_login_response, buffer, size);
			on_login_response = nullptr;
			break;
		case PacketType::SERVER_RESPONSE_PLAYER_INFO:
			HandlePacket("SERVER_RESPONSE_PLAYER_INFO", on_playerinfo_response, buffer, size);
			on_playerinfo_response = nullptr;
			break;
		case PacketType::SERVER_SEND_ENTITY:
			HandlePacket("SERVER_SEND_ENTITY", onEntity, buffer, size);
			break;
		case PacketType::SERVER_SEND_ENTITY_REMOVED:
			HandlePacket("SERVER_SEND_ENTITY_REMOVED", onEntityRemoved, buffer, size);
			break;
		case PacketType::SERVER_SEND_ENTITY_NAME:
			HandlePacket("SERVER_SEND_ENTITY_NAME:", onEntityName, buffer, size);
			break;
		case PacketType::SERVER_SEND_BATTLE_START:
			HandlePacket("SERVER_SEND_BATTLE_START", onBattleStart, buffer, size);
			break;
		case PacketType::SERVER_BATTLE_TURN:
			HandlePacket("SERVER_BATTLE_TURN", onTurnReceived, buffer, size);
			break;
		default:
			Log::Debug((std::string)"Unknown message type: " + std::to_string(header->type));
			break;
		}
	}

	WSACleanup();
}

void qfcnet::ClientChannel::Connect(int localPort, std::string serverAddress, int serverPort)
{
	std::condition_variable initialized;
	std::mutex mtx;

	listen_thread = std::thread([&]() {
		StartConnection(localPort, serverAddress, serverPort);
		initialized.notify_all();
		Listen();
	});

	std::unique_lock<std::mutex> lck(mtx);
	initialized.wait(lck);
	listen_thread.detach();
}

#pragma region Requests
void ClientChannel::Login(std::string user, std::string password,
	std::function<void(std::string)> completed,
	std::function<void(std::exception&)> error)
{
	on_login_response = [=](s_launcher_login_response response) {
		if (!response.authenticated)
		{
			error(std::exception("Invalid username or password"));
			return;
		}
		completed(response.authKey);
	};

	std::thread([=]() {
		s_launcher_login_info data;
		data.header.type = PacketType::LAUNCHER_LOGIN_INFO;
		strcpy_s(data.login, sizeof(data.login), user.c_str());
		strcpy_s(data.hashedPassword, sizeof(data.hashedPassword), password.c_str());
		Send(data);
	}).detach();
}

void ClientChannel::GetPlayer(
	std::function<void(ServerResponsePlayerInfo&)> completed,
	std::function<void(std::exception&)> error)
{
	on_playerinfo_response = [=](ServerResponsePlayerInfo info) {
		completed(info);
	};

	std::thread([=]() {
		ClientRequestPlayerInfo data;
		strcpy_s(data.header.authKey, sizeof(data.header.authKey), auth_token.c_str());
		Send(data);
	}).detach();
}

void ClientChannel::SendPlayerPosition(std::string animation, int map_id, sf::Vector2f position)
{
	std::thread([=]() {
		ClientSendPlayerPosition data;
		strcpy_s(data.header.authKey, sizeof(data.header.authKey), auth_token.c_str());
		data.location.map_id = map_id;
		data.location.position = position;
		data.view.animation = NetHelper::EncodeAnimation(animation);
		Send(data);
	}).detach();
}

void ClientChannel::GetEntities()
{
	//std::thread([=]() {
		ClientRequestEntities data;
		Log::Debug("ClientRequestEntities");
		strcpy_s(data.header.authKey, sizeof(data.header.authKey), auth_token.c_str());
		Send(data);
	//}).detach();
}

void ClientChannel::SendPlayerCommand(int turn_id, qfcbase::BattleAction command, int target_id, int additional_info)
{
	ClientCharacterBattleCommand data;
	Log::Debug("SendPlayerCommand");
	strcpy_s(data.header.authKey, sizeof(data.header.authKey), auth_token.c_str());

	data.turn_id = turn_id;
	data.command = command;
	data.additional_info = additional_info;
	data.target_id = target_id;
	Send(data);
}

void ClientChannel::SendPlayerTurnRequest(int lastTurn)
{
	ClientCharacterBattleNextTurn data;
	Log::Debug("SendPlayerTurnRequest");
	strcpy_s(data.header.authKey, sizeof(data.header.authKey), auth_token.c_str());

	data.battle_id = 0;
	data.lastTurn = lastTurn;

	Send(data);
}
#pragma endregion Requests
