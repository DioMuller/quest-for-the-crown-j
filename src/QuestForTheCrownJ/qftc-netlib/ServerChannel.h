#pragma once

#include <WinSock2.h>
#include <future>

#include "AuthStructs.h"
#include "ClientStructs.h"
#include "ServerStructs.h"
#include "Level.h"
#include "ChannelBase.h"

namespace qfcnet
{
	class ServerChannel :
		public ChannelBase
	{
	public:
		std::function<void(const ClientHeader&, std::shared_ptr<sockaddr_in>, int sender_size)> onClientMessage;
		std::function<LauncherLoginResponse(const LauncherLoginInfo&, std::shared_ptr<sockaddr_in>, int)> handleLoginInfo;
		std::function<void(const ClientRequestPlayerInfo&)> handleRequestPlayer;
		std::function<void(const ClientSendPlayerPosition&)> handlePlayerPosition;
		std::function<void(const ClientRequestEntities&)> handleRequestEntities;
		std::function<void(const ClientSendDisconnect&)> handlePlayerDisconnect;
		std::function<void(std::shared_ptr<sockaddr_in>)> handleConnectionClose;

		//Battle
		std::function<void(const ClientCharacterBattleNextTurn&)> handleCharacterRequestNextTurn;
		std::function<void(const ClientCharacterBattleCommand&)> handleCharacterCommand;
	public:
		ServerChannel(int port);
		~ServerChannel();

		template <typename T>
		int Send(T data, std::shared_ptr<sockaddr_in> addr, int addr_size)
		{
			int r = sendto(channel_socket, (char*)&data, sizeof(data), 0, (SOCKADDR*)addr.get(), addr_size);
			if (r <= 0)
				Log::Error("Send error: " + std::to_string(WSAGetLastError()));
			return r;
		}

		void SendEntity(std::shared_ptr<qfcbase::Level> level, std::shared_ptr<qfcbase::Entity> entity, std::shared_ptr<sockaddr_in> addr, int addr_size);
		void SendServerCommand(int turn_id, qfcbase::BattleAction command, int entity_id, int target_id, int additional_info, std::shared_ptr<sockaddr_in> addr, int addr_size);
	private:
		bool stop_listen;
		std::thread listen_thread;
		SOCKET channel_socket;

		void Listen(int port);
	};
}
