#pragma once
#include "MainGame.h"
#include "StructBase.h"
#include "ClientStructs.h"

namespace qfcgame
{
	class MultiplayerGame :
		public MainGame
	{
		////////////////////////////////////////
		// Attributes
		////////////////////////////////////////
	private:
		qfcnet::ClientChannel clientChannel;
		int player_entity_id;
		std::mutex ent_update_mutex;

		/////////////////////////////////////
		// Constructors
		/////////////////////////////////////
	public:
		MultiplayerGame();
		~MultiplayerGame();

		/////////////////////////////////////
		// Methods
		/////////////////////////////////////
		void Connect(std::string server_addr, std::string auth_token);
		void RefreshSceneFromServer();
		std::shared_ptr<qfcbase::Entity> CreateEntity(EntityHeader info, sf::Vector2f position);
		//void SetEntities(std::vector<ServerEntityInfo> entities);
		void StartConfront(std::shared_ptr<qfcbase::Entity> e1, std::shared_ptr<qfcbase::Entity> e2);

		void SendTurn(const ClientCharacterBattleCommand& turn);
		void RequestTurn(const ClientCharacterBattleNextTurn& request);
	};
}
