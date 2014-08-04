#pragma once
#include "MainGame.h"

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
		std::shared_ptr<qfcbase::Entity> CreateEntity(int id, EntityType type, sf::Vector2f position);
		//void SetEntities(std::vector<ServerEntityInfo> entities);
	};
}
