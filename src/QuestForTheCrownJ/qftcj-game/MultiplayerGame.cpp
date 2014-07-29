#include "MultiplayerGame.h"

#include "Hero.h"
#include "Slime.h"
#include "Log.h"
#include "LevelLoader.h"

using namespace qfcgame;
using namespace qfcbase;

std::shared_ptr<qfcbase::Entity> CreateEntity(const ServerEntityInfo& info)
{
	std::shared_ptr<qfcbase::Entity> entity;

	switch (info.entity.type)
	{
	case EntityType::ENTITY_HERO:
		entity = std::make_shared<qfcbase::Hero>();
		break;
	case EntityType::ENTITY_SLIME:
		entity = std::make_shared<qfcbase::Slime>();
		break;
	}

	if (entity)
		entity->Sprite->Position = sf::Vector2f(info.position.x, info.position.y);

	return entity;
}

MultiplayerGame::MultiplayerGame()
{
}


MultiplayerGame::~MultiplayerGame()
{
}

void MultiplayerGame::Connect(int localPort, std::string auth_token)
{
	clientChannel.Connect(localPort, "127.0.0.1", 12345);
	clientChannel.auth_token = auth_token;
	clientChannel.onEntity = [this](const ServerEntityInfo& entityInfo) {
		auto entity = CreateEntity(entityInfo);
		currentScene->AddEntity(entity);
	};
}

void MultiplayerGame::RefreshSceneFromServer()
{
	clientChannel.GetPlayerInfo([&, this](ServerPlayerInfo player) {
		auto scene = LevelLoader::LoadMap(this->getptr(), 1, (std::string)"Content/maps/" + (std::string)player.map_name + (std::string)".tmx");
		LoadScene(scene, false);

		Log::Debug((std::string)"Map loaded: " + std::string(player.map_name));

		clientChannel.GetEntities(player.map_name);
	}, [&](std::exception& ex) {
		Log::Error(ex.what());
	});
}
