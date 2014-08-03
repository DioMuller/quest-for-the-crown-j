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
			if (size != sizeof(s_launcher_login_response))
			{
				Log::Error((std::string)"Invalid packet size for LAUNCHER_LOGIN_RESPONSE: " + std::to_string(size));
				continue;
			}
			if (!on_login_response)
			{
				Log::Error("Unexpected LAUNCHER_LOGIN_RESPONSE");
				continue;
			}
			on_login_response(*(s_launcher_login_response*)(buffer));
			on_login_response = nullptr;
			break;
		case PacketType::SERVER_RESPONSE_PLAYER_INFO:
			if (size != sizeof(ServerResponsePlayerInfo))
			{
				Log::Error((std::string)"Invalid packet size for SERVER_RESPONSE_PLAYER_INFO: " + std::to_string(size));
				continue;
			}
			if (!on_playerinfo_response)
			{
				Log::Debug("No handler for SERVER_RESPONSE_PLAYER_INFO");
				continue;
			}
			else {
				auto info = (ServerResponsePlayerInfo*)buffer;
				on_playerinfo_response(*info);
				on_playerinfo_response = nullptr;
			}
			break;
		case PacketType::SERVER_SEND_ENTITY:
			if (size != sizeof(ServerSendEntity))
			{
				Log::Error((std::string)"Invalid packet size for SERVER_SEND_ENTITY: " + std::to_string(size));
				continue;
			}
			if (!onEntity)
			{
				Log::Debug("No handler for SERVER_ENTITY_INFO");
				continue;
			}
			else {
				auto info = (ServerSendEntity*)buffer;
				onEntity(*info);
			}
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

void ClientChannel::SendPlayerPosition(std::string animation, int x, int y)
{
	std::thread([=]() {
		ClientSendPlayerPosition data;
		strcpy_s(data.header.authKey, sizeof(data.header.authKey), auth_token.c_str());
		data.position.x = x;
		data.position.y = y;
		data.animation = NetHelper::EncodeAnimation(animation);
		Send(data);
	}).detach();
}

void ClientChannel::SendPlayerFullPosition(std::string map_name, int x, int y)
{
	ClientSendPlayerFullPosition data;
	std::thread([=]() {
		ClientSendPlayerFullPosition data;
		strcpy_s(data.header.authKey, sizeof(data.header.authKey), auth_token.c_str());
		strcpy_s(data.player.map_name, sizeof(data.player.map_name), map_name.c_str());
		data.player.position.x = x;
		data.player.position.y = y;
		Send(data);
	}).detach();
}

void ClientChannel::GetEntities(std::string screen_name)
{
	//std::thread([=]() {
		ClientRequestEntities data;
		Log::Debug("ClientRequestEntities");
		strcpy_s(data.header.authKey, sizeof(data.header.authKey), auth_token.c_str());
		Send(data);
	//}).detach();
}
#pragma endregion Requests
