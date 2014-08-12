#pragma once

#include <functional>
#include <WinSock2.h>
#include <string>
#include <vector>
#include <memory>
#include <future>
#include <thread>

#include "ServerStructs.h"
#include "ClientStructs.h"
#include "AuthStructs.h"
#include "ChannelBase.h"

namespace qfcnet
{
	class ClientChannel :
		public ChannelBase
	{
	public:
		std::string auth_token;
		std::function<void(ServerSendEntity&)> onEntity;
		std::function<void(ServerSendEntityRemoved&)> onEntityRemoved;

		std::function<void(ServerBattleStart&)> onBattleStart;
		std::function<void(ServerBattleTurn&)> onTurnReceived;

	public:
		ClientChannel();

		void Connect(int localPort, std::string serverAddress, int serverPort);

		~ClientChannel();

		void Login(std::string user, std::string password,
			std::function<void(std::string)> completed,
			std::function<void(std::exception&)> error);

		void GetPlayer(
			std::function<void(ServerResponsePlayerInfo&)> completed,
			std::function<void(std::exception&)> error);
		void SendPlayerPosition(std::string animation, int map_id, sf::Vector2f position);
		void GetEntities();

		void SendPlayerCommand(int turn_id, qftcbase::BattleCommand command, int additional_info);
		void SendPlayerTurnRequest(int lastTurn);

		template <typename T>
		void Send(T data)
		{
			int r = sendto(channel_socket, (char*)&data, sizeof(data), 0, (SOCKADDR*)&server_addr, server_addr_size);
			if (r <= 0)
				Log::Error("Send error: " + std::to_string(WSAGetLastError()));
		}

	private:
		std::thread listen_thread;
		bool stop_listen;
		SOCKET channel_socket;

		sockaddr_in server_addr;
		int server_addr_size;

		std::function<void(LauncherLoginResponse&)> on_login_response;
		std::function<void(ServerResponsePlayerInfo&)> on_playerinfo_response;
		void Listen();

		void StartConnection(int localPort, std::string &serverAddress, int serverPort);
	};
}
